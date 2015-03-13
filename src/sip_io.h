
#ifndef SIP_IO_H
#define SIP_IO_H

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

#include "net/sip/sip.h"
#include "net/sip/sip_callid.h"
#include "net/smpp/smpp.h"

#include "sip_io.h"
#include "smpp_io.h"

#include "log/log.h"
#include "net/udp/udp.h"
#include "sm_struct.h"
#include "str.h"

#include "linked_list/map.h"
#include "linked_list/keys.h"

///////////////////////
// Used save session SIP info
/////

typedef struct sip_session_t{
    sip_message_t *p_msg_sip; //MSG SIP parsed
    sm_data_t     *p_sm;        //sm_data_t
} sip_session_t;

void init_sip_session(sip_session_t **p_p_sip, sip_message_t *p_msg_sip, void *p_sm);
#define new_sip_session_t()    (sip_session_t*)calloc(1, sizeof(sip_session_t))

void  free_sip_session(void **data);

extern map *map_session_sip; //<str(call_id), sip_session_t>

///////////////////////
// Used to save socket SIP info
/////

typedef struct _sip_socket_t{
	  socket_t      *sock;
    unsigned char *interface_name;
} sip_socket_t;
#define new_sip_socket_t()   (sip_socket_t*)calloc(1,sizeof(sip_socket_t))

inline void init_sip_socket_t(sip_socket_t *p_sip_socket,unsigned char *interface_name, unsigned char *ip_host, unsigned int port_host);

inline void free_sip_socket(void **p_p_data);

extern map *map_iface_sip;   //<str(name_interface), smpp_socket_t>

///////////////////////

int sip_start_connection(sip_socket_t *p_sip_socket);
int sip_end_connection(sip_socket_t *p_sip_socket);
int sip_restart_connection(sip_socket_t *p_sip_socket);

int sip_engine(sip_socket_t *p_sip_sock);

///////////////////////
// Send SIP MESSAGE
/////

int send_sms_to_sip(unsigned char *interface_name, sm_data_t *p_sm, unsigned char *ip_remote, unsigned int port_remote);

///////////////////////
// Used for MAP and LIST
/////

void  free_sip_socket(void **p_p_data);
void* copy_sip_socket(const void *p_data);
int   compare_sip_socket(const void *p_data1, const void *p_data2);


#endif /* SIP_IO_H*/


