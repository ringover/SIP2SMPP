#ifdef __cplusplus
extern "C"{
#endif 

#ifndef SMPP_H
#define SMPP_H

#include <stdio.h>
#include <stdlib.h>
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

#include "../../tools.h"
#include "../../log/log.h"

int32_t do_smpp_connect_transceiver(int sock_tcp, uint8_t *user, uint8_t *passwd, uint8_t *system_type, uint8_t ton_src, uint8_t npi_src);
int32_t do_smpp_connect_receiver(int sock_tcp, uint8_t *user, uint8_t *passwd, uint8_t *system_type, uint8_t ton_src, uint8_t npi_src);
int32_t do_smpp_connect_transmitter(int sock_tcp, uint8_t *user, uint8_t *passwd, uint8_t *system_type, uint8_t ton_src, uint8_t npi_src);

int32_t do_smpp_send_message(int sock_tcp, uint8_t *src_addr, uint8_t *dst_addr, uint8_t *message, uint8_t ton_src, uint8_t npi_src, uint8_t ton_dst, uint8_t npi_dst);

int32_t do_smpp_receive_message(int sock_tcp, uint8_t **src_addr, uint8_t **dst_addr, uint8_t **message);

int32_t do_smpp_close(int sock_tcp);

#endif

#ifdef __cplusplus
}
#endif

