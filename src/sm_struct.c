
#include "sm_struct.h"

#ifndef _strcpy
#define _strcpy(dst, src) \
    dst = (char*)calloc(strlen((char*)src)+1, sizeof(char)); \
    strcpy(dst, src)
#endif /*_strcpy*/

#ifndef _strncpy
#define _strncpy(dst, src, size) \
    dst = (char*)calloc(size+1, sizeof(char)); \
    strncpy(dst, src, size)
#endif /*_strncpy*/

map *map_sm;          //<uint(id_sm), sm_data_t>

///////////////////////
// sm_data_t
//////

void init_sm_data_t(sm_data_t *p_sm, socket_t *sock, long long int id, type_interface_t type, unsigned char *ip_origin, unsigned int port_origin, void *p_msg_origin, unsigned char *src, unsigned char *dst, unsigned char *msg){
    if(p_sm){
        if(id > 0){
            p_sm->id = id;
        }
        p_sm->sock = sock;
        p_sm->type = type;
        _strcpy(p_sm->ip_origin, ip_origin);
        p_sm->port_origin = port_origin;
        if(src){
            _strcpy(p_sm->src, src);
        }
        if(dst){
            _strcpy(p_sm->dst, dst);
        }
        if(msg){
            _strcpy(p_sm->msg, msg);
        }
        p_sm->p_msg_origin = p_msg_origin;
    }
    return;
}
void  free_sm_data(void **data){
    if(data && *data){
        sm_data_t *p_sm = (sm_data_t*)*data;
        if(p_sm->ip_origin){
            free(p_sm->ip_origin);
        }
        //sock free not her
        //p_msg_origin free not her
//        if(p_sm->type == I_SIP){
//            free_sip_message(p_sm->p_msg_origin);
//        }else if(p_sm->type == I_SMPP){
//            free(p_sm->p_msg_origin);
//        }else if(p_sm->type == I_SIGTRAN){
//            //TODO
//        }
        //SM info
        if(p_sm->src){
            free(p_sm->src);
        }
        if(p_sm->dst){
            free(p_sm->dst);
        }
        //message
        if(p_sm->msg){
            free(p_sm->msg);
        }
        free(p_sm);
        *data = NULL;
    }
    return;
}


