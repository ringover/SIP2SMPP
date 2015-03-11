/**
*       udp_methode
*/
#ifdef __cplusplus
extern "C" {
#endif

#ifndef UDP_H
#define UDP_H


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/time.h>
#include <pthread.h>
#include <unistd.h>

#include "../net_struct.h"
#include "../../log/log.h"

int udp_socket(socket_t *sock, unsigned char *ip_host, unsigned int port_host);
int udp_close(socket_t *sock);

int do_udp_receive(socket_t *sock, unsigned char *buffer, size_t buffer_len, struct sockaddr_in *from);
int do_udp_send(socket_t *sock, unsigned char *buffer, size_t buffer_len, char *ip_remote, unsigned int port_remote);

#endif /* UDP_H */

#ifdef __cplusplus
}
#endif

