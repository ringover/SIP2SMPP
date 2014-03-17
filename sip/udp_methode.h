/**
*       udp_methode
*/
#ifdef __cplusplus
extern "C" {
#endif

#ifndef UDP_METHODE_H
#define UDP_METHODE_H


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/types.h>


void do_udp_close(int sock);

int do_udp_connect(int *sock,char *addrIP, int port);

int udp_receive(int sock, char* buffer[], size_t buffer_len, int verbos);

int udp_send(int sock, char* buffer, size_t buffer_len, char* addrIP, int port, int verbos);

#endif

#ifdef __cplusplus
}
#endif
