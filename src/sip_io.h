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

#include <osip2/osip.h>
#include <osipparser2/osip_parser.h>

#include "log/log.h"
#include "net/udp/udp.h"

#include "linked_list/map.h"

//Status SIP
#define   SIP_CONNECT     true
#define   SIP_DISCONNECT  false

typedef struct _sip_socket{
	socket_t      *sock;
  int           status;
  unsigned char *interface_name;
  //IP/port binded on this socket
  unsigned char *ip_host;
  unsigned int  port_host;
}sip_socket;

sip_socket* new_sip_socket(unsigned char *interface_name, unsigned char *ip_host, unsigned int port_host);
void sip_socket_free(sip_socket **p_sip_socket);

int sip_start_connection(sip_socket *p_sip_socket);
int sip_end_connection(sip_socket *p_sip_socket);
int sip_restart_connection(sip_socket *p_sip_socket);

int sip_send_message(sip_socket *p_sip_socket, unsigned char *buffer, unsigned char *ip_remote, unsigned int port_remote);
int sip_receive_message(sip_socket *p_sip_socket, unsigned char **buffer, unsigned int *buffer_len, unsigned char **ip_remote, unsigned int *port_remote);

int sip_receive_sms(sip_socket *p_sip_socket, unsigned char **from_msisdn, unsigned char **to_msisdn, unsigned char **message);


int send_sms_to_sip(unsigned char *interface_name, unsigned char *from_msisdn, unsigned char *to_msisdn, unsigned char *message, unsigned char *ip_remote, unsigned int port_remote);
int send_sms_to_sip_interface(unsigned char *interface_name_src, unsigned char *from_msisdn, unsigned char *to_msisdn, unsigned char *message, unsigned char *interface_name_dst);

/**
 * Used for MAP and LIST
 */
extern map *map_str_sip;

void  free_sip_socket(void **p_p_data);
void* copy_sip_socket(const void *p_data);
int   compare_sip_socket(const void *p_data1, const void *p_data2);


#endif /* SIP_MNG_H*/

#ifdef __cplusplus
}
#endif

