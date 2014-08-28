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
#include <unistd.h>

#include "../../log/log.h"

void do_udp_close(int sock);

int32_t do_udp_connect(int *sock, uint8_t *addrIP, uint16_t port);

int32_t udp_receive(int sock, uint8_t** buffer, size_t buffer_len);

int32_t udp_send(int sock, uint8_t* buffer, size_t buffer_len, uint8_t* addrIP, uint16_t port);

#endif /* UDP_METHODE_H */

#ifdef __cplusplus
}
#endif

