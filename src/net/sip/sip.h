
#ifndef SIP_H
#define SIP_H

#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <stdbool.h>
#include "sip_callid.h"
#include "sip_header.h"
#include "sip_response.h"
#include "sip_request.h"
#include "sip_parser.h"
#include "sip_struct.h"
#include "../tcp/tcp.h"

//////////////////////////
// Receive SIP
//////

inline int sip_receive(socket_t *sock, char *buffer, size_t buffer_len, char **remote_ip, unsigned int *remote_port);

#define sip_parser(buffer,p_sip) \
    sip_parser_message(p_sip, buffer)

int sip_scan_sock(socket_t *sock, sip_message_t *p_sip, char **remote_ip, unsigned int *remote_port)

//////////////////////////
// Send SIP
//////

int sip_send_request(socket_t *sock, sip_message_t *p_sip);

int sip_send_response(socket_t *sock, sip_message_t *p_sip);


#endif /*SIP_H*/

