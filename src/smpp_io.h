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

#include "type_projet.h"
#include "net/tcp/tcp.h"
#include "net/smpp/smpp.h"
#include "net/smpp/libsmpp34/smpp34.h"
#include "net/smpp/libsmpp34/smpp34_structs.h"
#include "net/smpp/libsmpp34/smpp34_params.h"

#include "linked_list/map.h"

//Status SMPP
#define   SMPP_CONNECT    true
#define   SMPP_DISCONNECT false

typedef struct _smpp_socket{
    //connect/Disconnect
    bool      status;
    //Transceiver/Receiver/Transmitter
    uint16_t  bind;
    //socket smpp
    socket_t  *sock;
    //Remote server
    unsigned char *ip_remote;
    unsigned int   port_remote;
    pthread_mutex_t mutex;
    //Login/Password
//    unsigned char user[16];
//    unsigned char passwd[9];
    unsigned char *user;
    unsigned char *passwd;
    //interface_name
    unsigned char *interface_name;
    //Other informations
    uint8_t  *system_type;//Type of ESME : WWW - EMAIL - VMS - OTA - etc
    int8_t   ton_src;//Type Of Number(0-6)
    int8_t   npi_src;//Numeric Plan Indicaor(0-18)
    int8_t   ton_dst;//Type Of Number(0-6)
    int8_t   npi_dst;//Numeric Plan Indicaor(0-18)
} smpp_socket;

smpp_socket* new_smpp_socket(unsigned char *interface_name, unsigned char *ip_remote, unsigned int port_remote, unsigned char *user, unsigned char *passwd, int bind, unsigned char *system_type, int ton_src, int npi_src, int ton_dst, int npi_dst);

void smpp_socket_free(smpp_socket **p_smpp_socket);

int smpp_start_connection(smpp_socket *p_smpp_socket);
int smpp_restart_connection(smpp_socket *p_smpp_socket);
int smpp_end_connection(smpp_socket *p_smpp_socket);

int smpp_send_sms(smpp_socket *p_smpp_socket, unsigned char *from_msisdn, unsigned char *to_msisdn, unsigned char *msg);
int smpp_receive_sms(smpp_socket *p_smpp_socket, unsigned char **from_msisdn, unsigned char **to_msisdn, unsigned char **msg);

void display_sms(char *from_msisdn, char *to_msisdn, char *msg);

int send_sms_to_smpp(unsigned char* interface_name, unsigned char *from_msisdn, unsigned char *to_msisdn, unsigned char *msg);
int send_sms_to_smpp_interface(unsigned char* interface_name_src, unsigned char *from_msisdn, unsigned char *to_msisdn, unsigned char *msg, unsigned char* interface_name_dst);

/**
 * Used for MAP and LIST
 */

extern map *map_str_smpp;

void  free_smpp_socket(void **p_p_data);
void* copy_smpp_socket(const void *p_data);
int   compare_smpp_socket(const void *p_data1, const void *p_data2);


#endif

#ifdef __cplusplus
}
#endif

