
#ifndef SM_STRUCT_H
#define SM_STRUCT_H

#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#include "linked_list/map.h"
#include "linked_list/keys.h"

#include "net/net.h"

extern map *map_sm;  //<uint32(id_sm), sm_data_t>

///////////////////////
// sm_data_t
/////

typedef enum type_interface_t {
    I_SIP,
    I_SMPP,
    I_SIGTRAN
} type_interface_t;

typedef struct sm_data_t{
    long long int     id;
    //origin
    socket_t         *sock;
    type_interface_t  type;
    //Socket / origin msg
    unsigned char    *ip_origin;
    unsigned int      port_origin;
    void             *p_msg_origin;//p_msg_sip || p_msg_smpp(submit_sm_t, submit_sm_t ...)
    //sm
    unsigned char *src;
    unsigned char *dst;
    unsigned char *msg;
} sm_data_t;

void init_sm_data_t(sm_data_t *p_sm, socket_t *sock, long long int id, type_interface_t type, unsigned char *ip_origin, unsigned int port_origin, void *p_msg_origin, unsigned char *src, unsigned char *dst, unsigned char *msg);
#define new_sm_data_t()    (sm_data_t*)calloc(1, sizeof(sm_data_t))

void  free_sm_data(void **data);


#endif /* SM_STRUCT_H */

