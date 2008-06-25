/* this tool for testing tcp server performance */

#include <iostream>

#ifdef _WIN32
# include <winsock2.h>
# define strcasecmp(s, t) _stricmp(s, t)
# define strncasecmp(s, t,n) _strnicmp(s, t, n)
# define sleep(n) Sleep((n)*1000)
# define usleep(n) Sleep((n))
#else
# include <unistd.h>
# include <netinet/in.h>
# include <pthread.h>
#endif

#include "timer.hxx"
#include "session.hxx"
#include "timeout_service.hxx"

using namespace inet;

/* globle variable define */
static char server_address[64] = {0};
static char dstip[16] = {0};
static unsigned short dstport = 0;
static int thread_counts = 1;
static int connection_counts_per_thread = 1;
static int connect_timeout = 5;
static int echo_packet_size = 100;
static int echo_frequency = 1000;
static char loadfiledir[128] = {0};
static int receive_print = 0;
static int log_level = 2;
static char *sendbuf = NULL;
static int exit_time = 100;
const char* helpinfo = 
   "\nThis is tool for testing tcp server performance.\n\n"
   "Usage: benchmark [OPTIONS]\n"
   "    -h, display this message then exit.\n"
   "    -v, display version information then exit.\n"
   "    -d, debug log onoff\n"
   "    -p, print receive packet onoff\n"
   "    -s, server address, [ip:port]\n"
   "    -n, thread counts, default value:1\n"
   "    -c, tcp connection counts per thread, default value:1\n"
   "    -o, tcp connect timeout time [second], default value:10s\n"
   "    -l, echo tcp packet size [bytes], default value:100\n" 
   "    -t, echo frequency per tcp connection [millisecond], default value:1000\n"
   "    -f, open file name that you want to send\n"
   "    -e, exit program after [n] seconds. default: 100s\n"
   "Report bugs to <hlxxxx@google.com>\n";

static int 
parse_options(int argc, char *argv[])
{
    int c;
    extern char *optarg;
    extern int optind, opterr;

    opterr = 0;

    if (argc == 1) return -1;

    while ((c = getopt(argc, argv, "hvpds:n:c:l:o:t:f:e:")) != -1) {
        switch(c) {
        case 'h':
            printf(helpinfo);
            return 1;
        case 'v':
            printf("benchmark-0.0.1\n");
            return 1;
        case 'p':
            receive_print = 1;
            break;
        case 'd':
            log_level = 0;
            break;
        case 's':
            if (strlen(optarg) > 64) return -1;
            strcpy(server_address, optarg);
            break;
        case 'n':
            thread_counts = atoi(optarg);
            break;
        case 'c':
            connection_counts_per_thread = atoi(optarg);
            break;
        case 'o':
            connect_timeout = atoi(optarg);
            break;
        case 'l':
            echo_packet_size = atoi(optarg);
            break;
        case 't':
            echo_frequency = atoi(optarg);
            break;
        case 'f':
            if (strlen(optarg) > 128) return -1;
            strcpy(loadfiledir, optarg);
            break;
        case 'e':
            exit_time = atoi(optarg); 
            break;
        default:
            printf("Unknown input arguments [%c].\n", c);
            return -1;
        }
    }

   if (optind < argc) {
        printf("Unexpected argument %s\n", argv[optind]);
        return -1;
   }

   return 0;
}

/* data collect */
struct thread_data {
        unsigned conn_count;
        unsigned fail_conn_count;
        unsigned lost_conn_count;
        unsigned send_bytes; 
        unsigned recv_bytes;
};

class client : public session
{
public:
    client(service& service, struct thread_data& data) 
        : session(service, inet::tcp), data_(data), timer_(service)
    {
        INET_REGISTER_CONNECTED(this, this, &client::on_connected);
        INET_REGISTER_CONNECT_FAILED(this, this, &client::on_connect_failed);
        INET_REGISTER_CONNECT_BROKEN(this, this, &client::on_connect_broken);
        INET_REGISTER_RECEIVED(this, this, &client::on_received);
    }

    ~client() {}

    void connect(const char* ip, unsigned short port)
    {
        async_connect(ip, port);
    }

    void on_connected(session*, buffer& istream, buffer& ostream)
    {
        data_.conn_count++;
        if (echo_frequency == 0)
        {
            on_timer(NULL);
        }
        else
        {
            timer_.set(echo_frequency, TIMEOUT_EVENT_CB(this, &client::on_timer), NULL);
        }
    }

    void on_connect_failed(session*, buffer& istream, buffer& ostream)
    {
        data_.fail_conn_count++;
    }

    void on_connect_broken(session*, buffer& istream, buffer& ostream)
    {
        data_.lost_conn_count++;
    }

    void on_received(session*, buffer& istream, buffer& ostream)
    {
        data_.recv_bytes += istream.length();

        if (receive_print)
        {
            istream.print();
        }

        if (echo_frequency == 0)
        {
            data_.send_bytes += istream.length(); 
            
            buffer::node* node;
            while (node = istream.pop_node())
            {
                ostream.push_node(node);
            }
            async_send();
        }
        else
        {
            istream.clear();
        }
    }

    void on_timer(void* args)
    {
        if (echo_packet_size > 0) 
        {
            if (sendbuf)
            {
                send_buffer_.write(sendbuf, echo_packet_size); 
            }

            data_.send_bytes += (echo_packet_size);
            async_send(); 
        }
    }
    
    INET_LIST_ENTRY(client) entries_;

private:
    struct thread_data& data_;
    timer timer_;
};

#ifdef _WIN32
DWORD WINAPI
thread_proc(LPVOID arg)
#else
void*
thread_proc(void *arg)
#endif
{
    service service;
    struct thread_data* data = (struct thread_data *)arg;

    INET_LIST_HEAD(client_list, client) clients;

    for (int i = 0; i < connection_counts_per_thread; i++)
    {
        client * cli = new client(service, *data); 
        INET_LIST_INSERT_HEAD(&clients, cli, entries_);
        cli->connect(dstip, dstport); 
    }

    service.run();

    client* cli;
    while (cli = INET_LIST_FIRST(&clients))
    {
        INET_LIST_REMOVE_HEAD(&clients, entries_);
        delete cli;
    }
    
exit:
#ifdef _WIN32
    return 0;
#else
    return NULL;
#endif
}

int main(int argc, char* argv[])
{
    int i, ret;
    char* pos;
    struct thread_data **pptd; 
    unsigned total_conn_count = 0; 
    unsigned total_fail_conn_count = 0; 
    unsigned total_lost_conn_count = 0;
    unsigned total_send_bytes = 0;
    unsigned total_recv_bytes = 0;

#ifdef _WIN32
    WSADATA wsaData;        
    WORD version = MAKEWORD(2, 0);        
    WSAStartup(version, &wsaData);
#else
    pthread_t tid;
#endif

    ret = parse_options(argc, argv);
    if (ret) 
    {
        if (ret == -1) printf(helpinfo);
        exit(-1);
    }
               
    printf("\n------Benchmark start infomation------\n"
        "server address: %s\n"
        "thread counts: %d\n"
        "connection counts per thread: %d\n" 
        "connect timeout time: %ds\n"
        "echo packet size: %d bytes\n"
        "echo frequency: %d ms\n"
        "the file dir: %s\n"
        "-------Exit Ctrl+C-------------------\n\n",
        server_address, 
        thread_counts, 
        connection_counts_per_thread,
        connect_timeout,
        echo_packet_size,
        echo_frequency,
        loadfiledir);

    pos = strstr(server_address, ":");
    if (pos == NULL)
    {
        printf("Invalid server address infomation.\n");
        exit(-1);
    }

    strncpy(dstip, server_address, pos - server_address);
    dstport = atoi(++pos);

    if (strlen(dstip) <= 0 || dstport <= 0) 
    {
        printf("Invalid server ip or port.\n");
        exit(-1);
    }

    if (strlen(loadfiledir) > 0) {
        FILE* fp = fopen(loadfiledir, "r");
        if (fp == NULL) 
        {
            printf("Failed to open file %s\n", loadfiledir);
             exit(-1);
        }
        sendbuf = (char *)calloc(1, 1024 * 64); /* max 64 k */
        echo_packet_size = (int)fread(sendbuf, 1, 1024 * 64, fp);

        if (echo_packet_size <= 0) 
        {
            printf("Failed to read %s file\n", loadfiledir);
            exit(-1);
        }

        printf("File Content: \n\n%s\n\n", sendbuf);
        fclose(fp);
    }
    else
    {
        sendbuf = (char *)calloc(1, echo_packet_size); 
        memset(sendbuf, 'a', echo_packet_size);
    }

    //log_set_silence_level(log_level);
        
    pptd = (struct thread_data**)calloc(thread_counts, sizeof(void*));

    for (i = 0; i < thread_counts; i++) 
    {
        pptd[i] = (struct thread_data*)calloc(1, sizeof(struct thread_data));
        assert(pptd[i]);
#ifdef _WIN32
        if (!CreateThread(NULL, 0, thread_proc, (LPVOID)pptd[i], 0, 0)) {
#else
        if(pthread_create(&tid, NULL, thread_proc, pptd[i])) 
        {
#endif
            printf("Failed to create thread.\n");
            exit(-1);
        }
    }

    { /* waiting for testing */
        int ntimes = exit_time;
        printf("Testing"); 
        while (ntimes--) 
        {
            sleep(1); 
            printf(".");
            fflush(stdout);
        }
        printf("\n");
    }

    { /* waiting for worker thread exit */
        int ntimes = 6;
        printf("Data collecting"); 
        while (ntimes--) 
        {
            sleep(1); 
            printf(".");
            fflush(stdout);
        }
        printf("\n");
    }
                     
    for (i = 0; i < thread_counts; i++) 
    {
        total_conn_count += pptd[i]->conn_count; 
        total_fail_conn_count += pptd[i]->fail_conn_count;
        total_lost_conn_count += pptd[i]->lost_conn_count;
        total_send_bytes += pptd[i]->send_bytes;
        total_recv_bytes += pptd[i]->recv_bytes;
        free(pptd[i]);
    }

    printf("total time: %ds\n"
           "total thread count: %d\n"
           "total connection count: %d\n"
           "success connection count: %d\n"
           "fail connection count: %d\n"
           "lost connection count: %d\n"
           "total send bytes: %d\n"
           "total recv bytes: %d\n"
           "average bandwith: %d bytes/second\n",
           exit_time, thread_counts, 
           thread_counts*connection_counts_per_thread,
           total_conn_count, total_fail_conn_count, 
           total_lost_conn_count, total_send_bytes, 
           total_recv_bytes, total_recv_bytes/exit_time);

    if (sendbuf) free(sendbuf);
    if (pptd) free(pptd);

    printf("Echo client exit.\n");
    return 0;
}


