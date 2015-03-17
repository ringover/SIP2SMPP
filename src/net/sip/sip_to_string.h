
#ifndef SIP_TO_STRING_H
#define SIP_TO_STRING_H

#include <stdlib.h>
#include <stdbool.h>
#include "sip_header.h"
#include "sip_response.h"
#include "sip_request.h"
#include "sip_struct.h"
#include "../../str.h"

/////////////////
// TO STRING
//////
int sip_from_to_string(sip_from_t *p_from, char **p_buffer);
int sip_to_to_string(sip_to_t *p_to, char **p_buffer);
int sip_contact_to_string(char *contact, char **p_buffer);
int sip_content_type_to_string(char *content_type, char **p_buffer);
int sip_content_length_to_string(int content_length, char **p_buffer);
int sip_cseq_to_string(sip_cseq_t *p_cseq, char **p_buffer);
int sip_call_id_to_string(sip_call_id_t *p_call_id, char **p_buffer);
int sip_via_to_string(sip_via_t *p_via, char **imp, short *i);

int sip_message_to_string(sip_message_t *p_sip, char **p_buffer, bool request);
#define sip_message_request_to_string(p_sip,p_buffer) \
    sip_message_to_string(p_sip, p_buffer, true);

#define sip_message_response_to_string(p_sip,p_buffer) \
    sip_message_to_string(p_sip, p_buffer, false);

#endif /*SIP_TO_STRING_H*/

