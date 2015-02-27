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

typedef struct _socket{
  int  socket;
  //TODO : map with <ip:port,csocket>
  int csocket; //With session (TCP-SCTP)
  pthread_mutex_t mutex;
} socket_t;

//void display_trame(char *buffer, size_t buffer_len);

#endif /* NET_STRUCT_H_ */
