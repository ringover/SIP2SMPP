
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
#include "net/net.h"
#include "net/udp/udp.h"
#include "sm_struct.h"
#include "str.h"

#include "linked_list/map.h"
#include "linked_list/keys.h"

#include "config/config.h"
#include "routing_def.h"

///////////////////////
// Used save session SIP info
/////

typedef struct _sip_session{
    sip_message_t *p_msg_sip; //MSG SIP parsed
    sm_data_t     *p_sm;        //sm_data_t
} sip_session_t;

void init_sip_session(sip_session_t **p_p_sip, sip_message_t *p_msg_sip, void *p_sm);
#define new_sip_session_t()    (sip_session_t*)calloc(1, sizeof(sip_session_t))

void  free_sip_session(void **data);

extern map *map_session_sip; //<str(call_id), sip_session_t>

///////////////////////
// Config SIP
/////

typedef struct _config_sip{
    char          *name;          //Interface name
	  socket_t      *sock;
    char          *ip;            //IP host
    unsigned int   port;          //Port Host
    char          *routing_to;     //Routing to
} config_sip_t;
#define new_config_sip()   (config_sip_t*)calloc(1, sizeof(config_sip_t))

extern map  *cfg_sip;  // <str, config_sip_t>

inline void destroy_config_sip(config_sip_t *sip);
void free_config_sip(void **sip);
int compare_config_sip(const void *sip1, const void *sip2);
inline void display_config_sip(config_sip_t *sip);

///////////////////////

int sip_start_connection(config_sip_t *p_config_sip);
int sip_end_connection(config_sip_t *p_config_sip);
int sip_restart_connection(config_sip_t *p_config_sip);

int sip_engine(config_sip_t *p_config_sip);

///////////////////////
// Send SIP MESSAGE
/////

int send_sms_to_sip(unsigned char *interface_name, sm_data_t *p_sm, unsigned char *ip_remote, unsigned int port_remote);

#endif /* SIP_IO_H*/

