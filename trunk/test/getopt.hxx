#ifndef __LIBINET_GETOPT_H__
#define __LIBINET_GETOPT_H__

#ifndef _WIN32
# include <unistd.h>
#else

#ifdef __cplusplus
extern "C" {
#endif

int getopt(int argc, char *argv[], char *opstring);

#ifdef __cplusplus
}
#endif
#endif

#endif /* __LIBINET_GETOPT_H__ */
