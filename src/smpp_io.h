
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

#include "log/log.h"

#include "./net/smpp/libsmpp34/smpp34.h"
#include "./net/smpp/libsmpp34/smpp34_structs.h"
#include "./net/smpp/libsmpp34/smpp34_params.h"
#include "type_projet.h"
#include "net/sip/sip.h"
#include "sip_io.h"
#include "net/smpp/smpp.h"

#include "linked_list/map.h"
#include "linked_list/keys.h"
#include "sm_struct.h"
#include "threadpool/threadpool.h"

#include "routing.h"

//Status SMPP
#define   SMPP_CONNECT    true
#define   SMPP_DISCONNECT false

///////////////////////
// Used to save session SMPP info
/////

typedef struct _smpp_session_t{
    unsigned int   command_id;  //eg: SUBMIT_SM
    void          *p_msg_smpp;  //type eg: submit_sm_t
    sm_data_t     *p_sm;        //sm_data_t
} smpp_session_t;

void init_smpp_session_t(smpp_session_t **p_p_smpp, unsigned int command_id, void *p_msg_smpp, void *p_sm);

#define new_smpp_session_t()    (smpp_session_t*)calloc(1, sizeof(smpp_session_t))

void free_smpp_session(void **data);

///////////////////////
// Used to save socket SMPP info
//////

typedef struct _smpp_socket_t{
    //interface_name
    unsigned char *interface;
    //connect/Disconnect
    bool status;
    //BIND_TRANSCEIVER||BIND_RECEIVER||BIND_TRANSMITTER
    int  bind;
    //socket smpp
    socket_t  *sock;
    //Remote server
    unsigned char *ip_remote;
    unsigned int   port_remote;
    //Login/Password
    unsigned char *user;   //user[16];
    unsigned char *passwd; //passwd[9];
    //Other informations
    short  *system_type;//Type of ESME : WWW - EMAIL - VMS - OTA - etc
    short   ton_src;//Type Of Number(0-6)
    short   npi_src;//Numeric Plan Indicaor(0-18)
    short   ton_dst;//Type Of Number(0-6)
    short   npi_dst;//Numeric Plan Indicaor(0-18)
} smpp_socket_t;
#define new_smpp_socket_t()   (smpp_socket_t*)calloc(1, sizeof(smpp_socket_t))

void init_smpp_socket_t(smpp_socket_t *p_smpp_socket, unsigned char *interface_name, unsigned char *ip_host, unsigned int port_host, unsigned char *ip_remote, unsigned int port_remote, unsigned char *user, unsigned char *passwd, int bind, unsigned char *system_type, int ton_src, int npi_src, int ton_dst, int npi_dst);

inline void free_smpp_socket(void **p_p_data);

///////////////////////

extern map *map_session_smpp;//<uint(sequence_number), smpp_session_t>
extern map *map_iface_smpp;  //<str(name_interface), sip_socket_t>

int smpp_start_connection(smpp_socket_t *p_smpp_socket);
int smpp_restart_connection(smpp_socket_t *p_smpp_socket);
int smpp_end_connection(smpp_socket_t *p_smpp_socket);

int smpp_recv_processing_request_sm(socket_t *sock, char *interface, char *ip_remote, unsigned int port_remote, void *data);
int smpp_recv_processing_request(socket_t *sock, const void *req);
int smpp_recv_processing_response(void *res);
static void* smpp_recv_processing(void *data);
int smpp_engine(smpp_socket_t *p_smpp_sock);

int send_sms_to_client_smpp(unsigned char* interface_name, sm_data_t *p_sm);
int send_sms_to_server_smpp(unsigned char* interface_name, sm_data_t *p_sm, char *ip_client, unsigned int port_client);

/**
 * Used for MAP and LIST
 */

void  free_smpp_socket(void **p_p_data);
void* copy_smpp_socket(const void *p_data);
int   compare_smpp_socket(const void *p_data1, const void *p_data2);


#endif /*SMPP_IO_INCLUDE_H*/

