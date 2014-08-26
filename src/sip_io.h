/**
* In this file, all SIP function are regrouped.
*/
#ifdef __cplusplus
extern "C"{
#endif

#ifndef SIP_MNG_H
#define SIP_MNG_H


#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

#include "log/log.h"
#include "net/udp/udp_methode.h"

typedef enum _error_sip{
	SIP_ERROR_NO         = 0x000,
	SIP_ERROR_SEND       = 0x001,
	SIP_ERROR_RECEIVE    = 0x002,
	SIP_ERROR_IP         = 0x004,
	SIP_ERROR_PORT       = 0x008,
	SIP_ERROR_BIND       = 0x010,
	SIP_ALREADY_CONNECT  = 0x020,
	SIP_IS_NOT_CONNECT   = 0x040,
	SIP_ERROR_CONNECT    = 0x080,
  SIP_NO_MESSAGE       = 0x100,
	SIP_ERROR_GENERAL    = 0xFFF
} error_sip;

typedef enum _sip_bind_status{
	SIP_CONNECT    = true,
	SIP_DISCONNECT = false
} sip_bind_status;

typedef enum _type_bind_sip_enum{
	SIP_BIND_NONE         = 0,
	SIP_BIND_RECEIVER     = 1,
	SIP_BIND_TRANSMITTER  = 2,
	SIP_BIND_TRANSCEIVER  = 3
} type_bind_sip_enum;

type_bind_sip_enum type_bind_sip_str_to_enum(uint8_t *str_in);

typedef struct _sip_socket{
	sip_bind_status     status;
	type_bind_sip_enum  type_bind;
	int       sock;
	uint8_t  *ip_addr;
	uint16_t  port;
}sip_socket;

sip_socket* new_sip_socket(uint8_t *ip_addr, uint16_t port, type_bind_sip_enum type_bind);
void free_sip_socket(sip_socket **p_sip_socket);

error_sip sip_start_connection(sip_socket *p_sip_socket);
error_sip sip_restart_connection(sip_socket *p_sip_socket);
error_sip sip_end_connection(sip_socket *p_sip_socket);
		
error_sip sip_send_message(sip_socket *p_sip_socket, uint8_t *str_in, uint8_t *ip_addr, uint8_t *port);
error_sip sip_receive_message(sip_socket *p_sip_socket, uint8_t **str_out);

#endif /* SIP_MNG_H*/

#ifdef __cplusplus
}
#endif

