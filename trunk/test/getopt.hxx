#ifndef __GETOPT_H
#define __GETOPT_H

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

#endif /* __GETOPT_H */
