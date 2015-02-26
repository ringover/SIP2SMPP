#ifdef __cplusplus
extern "C"{
#endif 

#ifndef SMPP_H
#define SMPP_H

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <unistd.h>
#include <time.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>

#ifdef __linux__
#include <stdint.h>
#endif

#include "./libsmpp34/smpp34.h"
#include "./libsmpp34/smpp34_structs.h"
#include "./libsmpp34/smpp34_params.h"

#include "../net_struct.h"

#include "../../tools.h"
#include "../../log/log.h"

#define RESP 0x80000000

//Connect
int do_smpp_bind(socket_t *sock, int bind, unsigned char *user, unsigned char *passwd, unsigned char *system_type, int ton_src, int npi_src);
int do_smpp_bind_transceiver(socket_t *sock, unsigned char *user, unsigned char *passwd, unsigned char *system_type, int ton_src, int npi_src);
int do_smpp_bind_receiver(socket_t *sock, unsigned char *user, unsigned char *passwd, unsigned char *system_type, int ton_src, int npi_src);
int do_smpp_bind_transmitter(socket_t *sock, unsigned char *user, unsigned char *passwd, unsigned char *system_type, int ton_src, int npi_src);
int do_smpp_bind_server(socket_t *sock, char *ip_host, int port_host);

//Client
int do_smpp_wait_client(socket_t *sock, char **ip_remote, int *port_remote, int *bind);

//Send / Receive SMS
int do_smpp_send_sms(socket_t *sock, unsigned char *from_msisdn, unsigned char *to_msisdn, unsigned char *message, int ton_src, int npi_src, int ton_dst, int npi_dst);
int do_smpp_receive_sms(socket_t *sock, unsigned char **from_msisdn, unsigned char **to_msisdn, unsigned char **message);

//Close smpp
int do_smpp_unbind(socket_t *sock);

#endif

#ifdef __cplusplus
}
#endif

