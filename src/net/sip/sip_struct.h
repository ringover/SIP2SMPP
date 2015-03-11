
#ifndef SIP_STRUCT_H
#define SIP_STRUCT_H

///////////////////////
// Struct define
/////

#include <stdlib.h>
#include <stdbool.h>
#include "sip_header.h"
#include "sip_response.h"
#include "sip_request.h"
#include "../../str.h"

#define sip_uri_t \
    char *scheme; \
    char *username; \
    char *host; \
    unsigned int port; \
    
typedef struct sip_ruri_t {
    sip_uri_t
} sip_ruri_t;

typedef struct sip_from_t {
    sip_uri_t
    char *tag;
} sip_from_t;

typedef struct sip_to_t{
    sip_uri_t
    char *tag;
} sip_to_t;

typedef struct sip_callid_t {
    char *number;
    char *host;
} sip_callid_t;

typedef struct sip_cseq_t {
    char *method;
    unsigned int number;
} sip_cseq_t;

typedef struct sip_message_t {
    char          *version;
    //Request only
    sip_ruri_t     uri;
    char          *method;
    //answer only
    int           status_code;
    char          *reason_phrase;

    sip_from_t    from;
    sip_to_t      to;
    sip_callid_t  call_id;
    sip_cseq_t    cseq;    
    char          *content_type;
    int           content_length;
    char          *message;
} sip_message_t;

/////////////////
// TO STRING
//////
int sip_from_to_string(sip_from_t *p_from, char **p_buffer);
int sip_to_to_string(sip_to_t *p_to, char **p_buffer);
int sip_content_type_to_string(char *content_type, char **p_buffer);
int sip_content_length_to_string(int content_length, char **p_buffer);
int sip_cseq_to_string(sip_cseq_t *p_cseq, char **p_buffer);
int sip_callid_to_string(sip_callid_t *p_callid, char **p_buffer);

int sip_message_to_string(sip_message_t *p_sip, char **p_buffer, bool request);
#define sip_message_request_to_string(p_sip,p_buffer) \
    sip_message_to_string(p_sip, p_buffer, true);

#define sip_message_response_to_string(p_sip,p_buffer) \
    sip_message_to_string(p_sip, p_buffer, false);

/////////////////
// DESTROY
//////
void destroy_sip_ruri(sip_ruri_t *p_ruri);
void destroy_sip_from(sip_from_t *p_from);
void destroy_sip_to(sip_to_t *p_to);
void destroy_sip_callid(sip_callid_t *p_callid);
void destroy_sip_cseq(sip_cseq_t *p_cseq);
void destroy_sip_message(sip_message_t *p_sip);

/////////////////
// FREE
//////
void free_sip_ruri(sip_ruri_t **p_p_data);
void free_sip_from(sip_from_t **p_p_data);
void free_sip_to(sip_to_t **p_p_data);
void free_sip_callid(sip_callid_t **p_p_data);
void free_sip_cseq(sip_cseq_t **p_p_data);
void free_sip_message(sip_message_t **p_p_data);
 
#endif /*SIP_STRUCT_H*/

