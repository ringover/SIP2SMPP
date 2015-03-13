#ifndef NET_STRUCT_H_
#define NET_STRUCT_H_

#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <net/ethernet.h>
#include <netinet/ip.h>
#include <netinet/in.h>
#include <netinet/ether.h>
#include <linux/if_packet.h>
#include <linux/if_ether.h>
#include <pthread.h>

typedef struct socket_t{
  int  socket;
  int csocket; //With session transport protocol (TCP-SCTP) for client connection
  unsigned char *ip;
  unsigned int   port;
  pthread_mutex_t mutex;
} socket_t;
#define new_socket_t()   (socket_t*)calloc(1, sizeof(socket_t))

//void display_trame(char *buffer, size_t buffer_len);

#endif /* NET_STRUCT_H_ */
