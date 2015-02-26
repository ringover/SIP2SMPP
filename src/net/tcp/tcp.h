#ifdef __cplusplus
extern "C"{
#endif

#ifndef TCP_H
#define TCP_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/time.h>
#include <pthread.h>
//#include <netdb.h>
#include <errno.h>

#include "../../str.h"
#include "../net_struct.h"
#include "../../log/log.h"

int do_tcp_send(socket_t *sock, unsigned char *buffer, size_t buffer_len, int flag);

int do_tcp_recv(socket_t *sock, unsigned char *buffer, size_t buffer_len, int flags);

int tcp_socket_server(socket_t *sock, char *ip_host, int port_host);
int do_tcp_wait_client(socket_t *sock, char **ip_remote, int *port_remote);

int tcp_socket_client(socket_t *sock, unsigned char *ip_remote, int port_remote);

int tcp_close(socket_t *sock);

#endif

#ifdef __cplusplus
}
#endif
