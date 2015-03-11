
#ifndef SIP_PARSER_H
#define SIP_PARSER_H

#include <stdbool.h>
#include <string.h>
#include <stdlib.h>
#include "sip_header.h"
#include "sip_response.h"
#include "sip_request.h"
#include "sip_struct.h"
#include "../../str.h"
#include "../../log/log.h"

#define SIP_VERSION   "SIP/2.0"

//Return header enum (FROM, TO ..)
int sip_what_is_the_header(char *buffer);
//Return method enum (MESSAGE, INVITE ..)
int sip_what_is_the_method(char *buffer);
//Return the status code
int sip_msg_is_response(char *buffer);
#define sip_msg_is_request(buffer) \
    sip_what_is_the_method(buffer)

int sip_parser_ruri(sip_ruri_t *p_ruri, char *buffer);
int sip_parser_from(sip_from_t *p_from, char *buffer);
int sip_parser_to(sip_to_t *p_to, char *buffer);
int sip_parser_callid(sip_callid_t *p_callid, char *buffer);
int sip_parser_cseq(sip_cseq_t *p_cseq, char *buffer);
int sip_parser_content_length(int *p_content_lenght, char *buffer);
int sip_parser_content_type(char** p_content_type, char *buffer);
int sip_parser_message(sip_message_t *p_sip, char *buffer);

///////////////////////
// MSG IS METHOD ?
/////

#define MSG_IS_METHOD(buffer,method) \
    (buffer && method && strncmp(buffer, method, strlen((char*)method))==0)

#define MSG_IS_MESSAGE(buffer) \
    MSG_IS_METHOD(buffer, MESSAGE_STR)

#define MSG_IS_INVITE(buffer) \
    MSG_IS_METHOD(buffer, INVITE_STR)

#define MSG_IS_ACK(buffer) \
    MSG_IS_METHOD(buffer, ACK_STR)

#define MSG_IS_OPTIONS(buffer) \
    MSG_IS_METHOD(buffer, OPTIONS_STR)

#define MSG_IS_BYE(buffer) \
    MSG_IS_METHOD(buffer, BYE_STR)

#define MSG_IS_PRACK(buffer) \
    MSG_IS_METHOD(buffer, PRACK_STR)

#define MSG_IS_CANCEL(buffer) \
    MSG_IS_METHOD(buffer, CANCEL_STR)

#define MSG_IS_INFO(buffer) \
    MSG_IS_METHOD(buffer, INFO_STR)

#define MSG_IS_REGISTER(buffer) \
    MSG_IS_METHOD(buffer, REGISTER_STR)

#define MSG_IS_SUBSCRIBE(buffer) \
    MSG_IS_METHOD(buffer, SUBSCRIBE_STR)

#define MSG_IS_NOTIFY(buffer) \
    MSG_IS_METHOD(buffer, NOTIFY)

#define MSG_IS_PUBLISH(buffer) \
    MSG_IS_METHOD(buffer, PUBLISH)

#define MSG_IS_REFER(buffer) \
    MSG_IS_METHOD(buffer, REFER_STR)

#define MSG_IS_UPDATE(buffer) \
    MSG_IS_METHOD(buffer, UPDATE_STR)

//////////////////////////
// MSG IS STATUS CODE ?
/////

#define MSG_IS_STATUS_CODE(buffer, num) \
    (num == sip_msg_is_response(buffer))

#endif /*SIP_PARSER_H*/

