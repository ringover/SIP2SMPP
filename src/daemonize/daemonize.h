#ifdef __cplusplus
extern "C"{
#endif

#ifndef DAEMONIZE_H
#define DAEMONIZE_H

#include <sys/types.h>
#include <sys/resource.h>
#include <sys/time.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include "../log/log.h"

//char* pid_file = (char*)DEFAULT_PIDFILE;
extern char* pid_file;

int daemonize(int nofork);

#endif /*DAEMONIZE_H*/

#ifdef __cplusplus
}
#endif

