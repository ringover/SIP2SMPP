
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

#include "routing_def.h"

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
// Config SMPP
//////

typedef struct _config_client_smpp{
    char         *name;           //Interface name
    socket_t     *sock;           //socket smpp
    char         *ip;             //Remote is client/Host if server
    unsigned int  port;           //Remote is client/Host if server
    char         *system_id;      //User ID
    char         *password;       //Passwd
    char          npi;            //numeric_plan_indicator
    char          ton;            //type_of_number
    char         *routing_to;     //Routing to
} config_client_smpp_t;
#define new_config_client_smpp()   (config_client_smpp_t*)calloc(1, sizeof(config_client_smpp_t))

typedef struct _config_smpp{
    char         *name;           //Interface name
    sock_model_t  model;          //client | server
    bool          status;         //connect/Disconnect
    socket_t     *sock;           //socket smpp
    char         *ip;             //Remote is client/Host if server
    unsigned int  port;           //Remote is client/Host if server
    char         *system_id;      //User ID
    char         *password;       //Passwd
    char          npi;            //numeric_plan_indicator
    char          ton;            //type_of_number
    char         *system_type;    //WWW | EMAIL | VMS | OTA ...
    char         *service_type;   //The service_type parameter can be used to indicate the SMS Application service associated with the message
    int           command_id;     //bind : transceiver | receiver | transmitter

    char         *routing_to;     //Routing to - Only if client model

    //Client(s) for server model
    map          *list_c_smpp;    // <str, config_client_smpp_t>
} config_smpp_t;
#define new_config_smpp()   (config_smpp_t*)calloc(1, sizeof(config_smpp_t))

inline void destroy_config_client_smpp(config_client_smpp_t *c_smpp);
inline void destroy_config_smpp(config_smpp_t *smpp);

void free_config_client_smpp(void **c_smpp);
void free_config_smpp(void **smpp);

int compare_config_client_smpp(const void *c_smpp1, const void *c_smpp2);
int compare_config_smpp(const void *smpp1, const void *smpp2);

inline void display_config_client_smpp(config_client_smpp_t *c_smpp);
inline void display_config_smpp(config_smpp_t *smpp);

extern map  *cfg_smpp; // <str, config_smpp_t>

///////////////////////

extern map *map_session_smpp;//<uint(sequence_number), smpp_session_t>

int smpp_start_connection(config_smpp_t *p_config_smpp);
int smpp_restart_connection(config_smpp_t *p_config_smpp);
int smpp_end_connection(config_smpp_t *p_config_smpp);

int smpp_recv_processing_request_sm(socket_t *sock, char *interface, char *ip_remote, unsigned int port_remote, void *data);
int smpp_recv_processing_request(socket_t *sock, const void *req);
int smpp_recv_processing_response(void *res);
static void* smpp_recv_processing(void *data);
int smpp_engine(config_smpp_t *p_config_smpp);

int send_sms_to_smpp(unsigned char* interface_name, sm_data_t *p_sm);

/**
 * Used for MAP and LIST
 */

void  free_config_smpp(void **p_p_data);
void* copy_config_smpp(const void *p_data);
int   compare_config_smpp(const void *p_data1, const void *p_data2);


#endif /*SMPP_IO_INCLUDE_H*/

