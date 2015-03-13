#ifdef __cplusplus
extern "C"{
#endif 

#ifndef SMPP_H
#define SMPP_H

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <unistd.h>
#include <time.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>

#include "./libsmpp34/smpp34.h"
#include "./libsmpp34/smpp34_structs.h"
#include "./libsmpp34/smpp34_params.h"

#include "../net_struct.h"
#include "../tcp/tcp.h"

#include "../../log/log.h"


/////////////////////////////////////////
////                                /////
////   Short Message Peer-to-Peer   /////
////            RECV SMPP           /////
////                                /////
/////////////////////////////////////////

inline int smpp_receive(socket_t *sock, char *buffer, size_t buffer_len);

inline int smpp_parser(char *buffer, int len, void **data);

int smpp_scan_sock(socket_t *sock, void **data);

/////////////////////////////////////////
////                                /////
////   Short Message Peer-to-Peer   /////
////            SEND SMPP           /////
////                                /////
/////////////////////////////////////////

unsigned int get_sequence_number();

inline int smpp_send_generic(socket_t *sock, unsigned int command_id, unsigned int command_status, unsigned int *sequence_number);
#define smpp_send_response(sock,command_id,command_status,sequence_number) \
    smpp_send_generic(sock,command_id,command_status,sequence_number)

int smpp_send_enquire_link(socket_t *sock, unsigned int *sequence_number);

int smpp_send_deliver_sm(socket_t *sock, char *from, char *to, char *msg, unsigned int *sequence_number, int src_ton, int src_npi, int dst_ton, int dst_npi);

int smpp_send_submit_sm(socket_t *sock, char *from, char *to, char *msg, unsigned int *sequence_number, int src_ton, int src_npi, int dst_ton, int dst_npi);

int smpp_send_query_sm(socket_t *sock, unsigned int *sequence_number);

/**
 * SMPP BIND Server
 */

int smpp_send_bind_server(socket_t *sock, char *ip_host, unsigned int port_host);

int smpp_wait_client(socket_t *sock, char **ip_remote, int *port_remote);

/**
 * SMPP BIND Client
 */

int smpp_send_bind_client(socket_t *sock, int command_id, char *ip_remote, unsigned int port_remote, unsigned char *user, unsigned char *passwd, unsigned char *system_type, int ton_src, int npi_src, unsigned int *sequence_number);

#define smpp_send_bind_transceiver_client(sock, ip_remote, port_remote, user, passwd, system_type, ton_src, npi_src, sequence_number) \
    smpp_send_bind_client(sock, BIND_TRANSCEIVER, ip_remote, port_remote, user, passwd, system_type, ton_src, npi_src, sequence_number)

#define smpp_send_bind_receiver_client(sock, ip_remote, port_remote, user, passwd, system_type, ton_src, npi_src, sequence_number) \
    smpp_send_bind_client(sock, BIND_RECEIVER, ip_remote, port_remote, user, passwd, system_type, ton_src, npi_src, sequence_number)

#define smpp_send_bind_transmitter_client(sock, ip_remote, port_remote, user, passwd, system_type, ton_src, npi_src, sequence_number) \
    smpp_send_bind_client(sock, BIND_TRANSMITTER, ip_remote, port_remote, user, passwd, system_type, ton_src, npi_src, sequence_number)

/**
 * SMPP send unbind 
 */

int smpp_send_unbind(socket_t *sock, unsigned int *sequence_number);

////////////////////
// Response
// 
// List :
// |+BIND_RECEIVER_RESP    | QUERY_SM_RESP   |+UNBIND_RESP     |+ENQUIRE_LINK_RESP |
// |+BIND_TRANSMITTER_RESP |+SUBMIT_SM_RESP  |+REPLACE_SM_RESP | SUBMIT_MULTI_RESP |
// |+BIND_TRANSCEIVER_RESP |+DELIVER_SM_RESP |+CANCEL_SM_RESP  | DATA_SM_RESP      |
// |+DELIVER_SM_RESP       |                 |                 |                   |
// '+' : implemented
// '-' : not implemented
// ' ' : not allowed
/////

int smpp_send_unbind_resp(socket_t *sock, unsigned int sequence_number, unsigned int command_status);

int smpp_send_unquire_link_resp(socket_t *sock, unsigned int sequence_number, unsigned int command_status);

int smpp_send_cancel_sm_resp(socket_t *sock, unsigned int sequence_number, unsigned int command_status);

int smpp_send_replace_sm_resp(socket_t *sock, unsigned int sequence_number, unsigned int command_status);

int smpp_send_submit_sm_resp(socket_t *sock, unsigned char *message_id, unsigned int sequence_number, unsigned int command_status);

int smpp_send_deliver_sm_resp(socket_t *sock, unsigned char *message_id, unsigned int sequence_number, unsigned int command_status);

int smpp_send_bind_resp(socket_t *sock, unsigned int command_id, unsigned char *system_id, unsigned int sequence_number, unsigned int command_status, bool set_version);

#define smpp_send_bind_transceiver_resp(sock, system_id, sequence_number, command_status, set_version) \
    smpp_send_bind_resp(sock, BIND_TRANSCEIVER_RESP, system_id, sequence_number, command_status, set_version)

#define smpp_send_bind_transmitter_resp(sock, system_id, sequence_number, command_status, set_version) \
    smpp_send_bind_resp(sock, BIND_TRANSMITTER_RESP, system_id, sequence_number, command_status, set_version)

#define smpp_send_bind_receiver_resp(sock, system_id, sequence_number, command_status, set_version) \
    smpp_send_bind_resp(sock, BIND_RECEIVER_RESP, system_id, sequence_number, command_status, set_version)

#endif

#ifdef __cplusplus
}
#endif

