
#ifndef INIT_MAPS_INCLUDE_H
#define INIT_MAPS_INCLUDE_H

#include <string.h>
#include "str.h"
#include "smpp_io.h"
#include "sip_io.h"
#include "linked_list/map.h"

///////////////////////
// map declaration
/////

extern map map_msg_sip;     //<str(call_id), sip_data_t>
extern map map_msg_smpp;    //<uint(sequence_number), smpp_data_t>
extern map map_sm;          //<uint(id_sm), sm_data_t>
extern map map_iface_sip;   //<str(name_interface), smpp_socket_t>
extern map map_iface_smpp;  //<str(name_interface), sip_socket_t>

///////////////////////
// sip_data_t save sip info
/////

typedef struct sip_data_t{
    //unsigned char *call_id;//Key
    unsigned char *method;
//    unsigned char *from_user;
//    unsigned char *from_host;
//    unsigned int   from_port;
//    unsigned char *from_tag;
//    unsigned char *to_user;
//    unsigned char *to_host;
//    unsigned int   to_port;
//    unsigned char *to_tag;
    long long int  sm_id;
} sip_data_t;

void init_sip_data(sip_data_t **p_p_sip, unsigned char *method, long long int sm_id);
#define new_sip_data_t()    (sip_data_t*)calloc(1, sizeof(sip_data_t))

void  free_sip_data(void **data);
void* copy_sip_data(const void *data);
int   compare_sip_data(const void *data1, const void *data2);

///////////////////////
// smpp_data_t save smpp info
/////

typedef struct smpp_data_t{
    //unsigned int   sequence_number;//Key
    unsigned int   command_id;
    long long int  sm_id;
} smpp_data_t;

void init_smpp_data(smpp_data_t **p_p_smpp, unsigned char command_id, long long int sm_id);

#define new_smpp_data_t()    (smpp_data_t*)calloc(1, sizeof(smpp_data_t))

void  free_smpp_data(void **data);
void* copy_smpp_data(const void *data);
int   compare_smpp_data(const void *data1, const void *data2);

///////////////////////
// sm_data_t
/////

typedef struct sm_data_t{
    //ID sm DB
    long long int  id;//Key 
    //SIP info
    unsigned char *call_id;
    //SMPP info
    unsigned int   sequence_number;
    //Interface origin
    unsigned char *interface;
    unsigned char *ip_origin;
    unsigned int   port_origin;
    //sm
    unsigned char *src;
    unsigned char *dst;
    unsigned char *msg;
} sm_data_t;

void init_sm_data(sm_data_t **p_p_sm, long long int id, unsigned char *call_id, unsigned int sequence_number, unsigned char *interface, unsigned char *ip_origin, unsigned int port_origin, unsigned char *src, unsigned char *dst, unsigned char *msg);
#define new_sm_data_t()    (sm_data_t*)calloc(1, sizeof(sm_data_t))

void  free_sm_data(void **data);
void* copy_sm_data(const void *data);
int   compare_sm_data(const void *data1, const void *data2);

///////////////////////
// char* (string)
/////

#define new_string(len)    (unsigned char*)calloc(len+1,sizeof(unsigned char))

void  free_string(void **s);
void* copy_string(const void *s);
int   compare_string(const void *a, const void *b);

///////////////////////
// uint32 (unsigned int)
/////

#define new_uint32()    (unsigned int*)calloc(1,sizeof(unsigned int))

void free_uint32(void **data);
void* copy_uint32(const void *data);
int compare_uint32(const void *data1, const void *data2);


#endif /* INIT_MAPS_INCLUDE_H */

