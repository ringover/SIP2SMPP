#ifdef __cplusplus
extern "C"{
#endif

#ifndef TCP_H
#define TCP_H


#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include "structSMPP.h"

int do_tcp_connect(TcpConnect p, int *s);

int do_tcp_close(int sock_tcp);

#endif

#ifdef __cplusplus
}
#endif
