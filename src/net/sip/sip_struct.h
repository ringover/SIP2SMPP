
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
#define new_sip_ruri_t()   (sip_ruri_t*)calloc(1,sizeof(sip_ruri_t));
void init_sip_ruri_t(sip_ruri_t *p_ruri, char *scheme, char *username, char *host, unsigned int port);

typedef struct sip_from_t {
    sip_uri_t
    char *tag;
} sip_from_t;
#define new_sip_from_t()   (sip_from_t*)calloc(1,sizeof(sip_from_t));
void init_sip_from_t(sip_from_t *p_from, char *scheme, char *username, char *host, unsigned int port, char *tag);

typedef struct sip_to_t{
    sip_uri_t
    char *tag;
} sip_to_t;
#define new_sip_to_t()   (sip_to_t*)calloc(1,sizeof(sip_to_t));
void init_sip_to_t(sip_to_t *p_to, char *scheme, char *username, char *host, unsigned int port, char *tag);

typedef struct sip_via_t {
    char *via_str;
    struct sip_via_t *via_next; 
} sip_via_t;
#define new_sip_via_t()   (sip_via_t*)calloc(1,sizeof(sip_via_t));
void init_sip_via_t(sip_via_t *p_via, char *via_str, sip_via_t *p_via_next);

typedef struct sip_call_id_t {
    char *number;
    char *host;
} sip_call_id_t;
#define new_sip_call_id_t()   (sip_call_id_t*)calloc(1,sizeof(sip_call_id_t));
void init_sip_call_id_t(sip_call_id_t *p_call_id, char *number, char *host);

typedef struct sip_cseq_t {
    unsigned int number;
    char *method;
} sip_cseq_t;
#define new_sip_cseq_t()   (sip_cseq_t*)calloc(1,sizeof(sip_cseq_t));
void init_sip_cseq_t(sip_cseq_t *p_cseq, unsigned int number, char *method);

typedef struct sip_message_t {
    char          *version;
    //Request only
    sip_ruri_t     ruri;
    char          *method;
    //Response/Answer only
    int           status_code;
    char          *reason_phrase;

    sip_via_t     *via;

    sip_from_t    from;
    sip_to_t      to;
    sip_call_id_t  call_id;
    sip_cseq_t    cseq;    
    char          *contact;
    char          *content_type;
    int           content_length;
    char          *message;
} sip_message_t;
#define new_sip_message_t()   (sip_message_t*)calloc(1,sizeof(sip_message_t));
void init_sip_message_t(sip_message_t *p_message, char *version, char *method, int status_code, char *reason_phrase, char* contact, char *content_type, int content_length, char *message);

/////////////////
// Content_type
/////

#define TEXT_PLAIN_STR      "text/plain"


/////////////////
// DESTROY
//////
void destroy_sip_ruri(sip_ruri_t *p_ruri);
void destroy_sip_from(sip_from_t *p_from);
void destroy_sip_to(sip_to_t *p_to);
void destroy_sip_call_id(sip_call_id_t *p_call_id);
void destroy_sip_cseq(sip_cseq_t *p_cseq);
void destroy_sip_message(sip_message_t *p_sip);
void destroy_sip_via(sip_via_t *p_via);

/////////////////
// FREE
//////
void free_sip_ruri(sip_ruri_t **p_p_data);
void free_sip_from(sip_from_t **p_p_data);
void free_sip_to(sip_to_t **p_p_data);
void free_sip_call_id(sip_call_id_t **p_p_data);
void free_sip_cseq(sip_cseq_t **p_p_data);
void free_sip_via(sip_via_t **p_p_data);
void free_sip_message(sip_message_t **p_p_data);
 
#endif /*SIP_STRUCT_H*/

