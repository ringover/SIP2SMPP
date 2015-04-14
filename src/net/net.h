#ifndef NETWORK_H_
#define NETWORK_H_

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
//  unsigned char *ip;
//  unsigned int   port;
  pthread_mutex_t mutex;
} socket_t;
#define new_socket()   (socket_t*)calloc(1, sizeof(socket_t))


typedef enum _sock_model{
    MODEL_CLIENT = 0,
    MODEL_SERVER = 1
} sock_model_t;

#define STR_MODEL_CLIENT  "CLIENT"
#define STR_MODEL_SERVER  "SERVER"

inline int str_to_sock_model(const char *str);
extern char* str_model[];

#endif /* NETWORK_H_ */
