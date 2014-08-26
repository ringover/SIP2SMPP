/**
*	Connection SMPP
*/
#ifdef __cplusplus
extern "C"{
#endif

#ifndef SMPP_IO_INCLUDE_H
#define SMPP_IO_INCLUDE_H

//#include <thread>
//#include <queue>
//#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <unistd.h>
#include <string.h>
#include <stdbool.h>

#include "net/tcp/tcp.h"
#include "net/smpp/smpp.h"
#include "net/smpp/libsmpp34/smpp34.h"
#include "net/smpp/libsmpp34/smpp34_structs.h"
#include "net/smpp/libsmpp34/smpp34_params.h"

typedef enum error_smpp{
        SMPP_ERROR_NO         = 0x00,
        SMPP_ERROR_SEND       = 0x01,
        SMPP_ERROR_RECEIVE    = 0x02,
        SMPP_ERROR_BIND       = 0x04,
        SMPP_ALREADY_CONNECT  = 0x08,
        SMPP_IS_NOT_CONNECT   = 0x10,
        SMPP_ERROR_CONNECT    = 0x20,
        SMPP_NO_MESSAGE       = 0x40,
        SMPP_ERROR_GENERAL    = 0xFF
} error_smpp;

typedef enum _smpp_bind_status{
        SMPP_CONNECT    = true,
        SMPP_DISCONNECT = false
} smpp_bind_status;

typedef enum _type_bind_smpp_enum{
        SMPP_BIND_NONE         = 0,
        SMPP_BIND_RECEIVER     = 1,
        SMPP_BIND_TRANSMITTER  = 2,
        SMPP_BIND_TRANSCEIVER  = 3
} type_bind_smpp_enum;

type_bind_smpp_enum type_bind_smpp_str_to_enum(uint8_t *str_in);
uint8_t* type_bind_smpp_enum_to_str(type_bind_smpp_enum num);

typedef struct _smpp_socket{
    smpp_bind_status status;
    uint16_t  type_bind;
    int       sock;
    uint8_t  *ip_addr;
    uint16_t  port;
    uint8_t  *user;
    uint8_t  *passwd;
    uint8_t  *system_type;
    int8_t   ton_src;
    int8_t   npi_src;
    int8_t   ton_dst;
    int8_t   npi_dst;
} smpp_socket;

smpp_socket* new_smpp_socket(uint8_t *ip_addr, uint16_t port, uint8_t *user, uint8_t *passwd, type_bind_smpp_enum type_bind, uint8_t *system_type, int8_t ton_src, int8_t npi_src, int8_t ton_dst, int8_t npi_dst);

void free_smpp_socket(smpp_socket **p_smpp_socket);

error_smpp smpp_start_connection(smpp_socket *p_smpp_socket);
error_smpp smpp_restart_connection(smpp_socket *p_smpp_socket);
error_smpp smpp_end_connection(smpp_socket *p_smpp_socket);

error_smpp smpp_send_message(smpp_socket *p_smpp_socket, uint8_t *from_msisdn, uint8_t *to_msisdn, uint8_t *msg);
error_smpp smpp_receive_message(smpp_socket *p_smpp_socket, uint8_t **from_msisdn, uint8_t **to_msisdn, uint8_t **msg);

void display_smpp_message(uint8_t *from_msisdn, uint8_t *to_msisdn, uint8_t *msg);

#endif

#ifdef __cplusplus
}
#endif

