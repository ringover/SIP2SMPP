
#include "sip_struct.h"

#ifndef _strcpy
#define _strcpy(dst, src) \
    dst = (char*)calloc(strlen((char*)src)+1, sizeof(char)); \
    strcpy(dst, src)
#endif /*_strcpy*/

#ifndef _strncpy
#define _strncpy(dst, src, size) \
    dst = (char*)calloc(size+1, sizeof(char)); \
    strncpy(dst, src, size)
#endif /*_strncpy*/

////////////////////////
// Init Struct
//////

void init_sip_ruri_t(sip_ruri_t *p_ruri, char *scheme, char *username, char *host, unsigned int port){
    if(p_ruri){
        if(scheme){
            _strcpy(p_ruri->scheme, scheme);
        }
        if(username){
            _strcpy(p_ruri->username, username);
        }
        if(host){
            _strcpy(p_ruri->host, host);
        }
        if(port > 0){
            p_ruri->port = port;
        }
    }
    return;
}

void init_sip_from_t(sip_from_t *p_from, char *scheme, char *username, char *host, unsigned int port, char *tag){
    if(p_from){
        if(scheme){
            _strcpy(p_from->scheme, scheme);
        }
        if(username){
            _strcpy(p_from->username, username);
        }
        if(host){
            _strcpy(p_from->host, host);
        }
        if(port > 0){
            p_from->port = port;
        }
        if(tag){
            _strcpy(p_from->tag, tag);
        }
    }
    return;
}

void init_sip_to_t(sip_to_t *p_to, char *scheme, char *username, char *host, unsigned int port, char *tag){
    if(p_to){
        if(scheme){
            _strcpy(p_to->scheme, scheme);
        }
        if(username){
            _strcpy(p_to->username, username);
        }
        if(host){
            _strcpy(p_to->host, host);
        }
        if(port > 0){
            p_to->port = port;
        }
        if(tag){
            _strcpy(p_to->tag, tag);
        }
    }
    return;
}

void init_sip_via_t(sip_via_t *p_via, char *via_str, sip_via_t *p_via_next){
    if(p_via){
        if(via_str){
            _strcpy(p_via->via_str, via_str);
        }
        if(p_via_next){
            p_via->via_next = p_via_next;
        }
    }
    return;
}

void init_sip_call_id_t(sip_call_id_t *p_call_id, char *number, char *host){
    if(p_call_id){
        if(number){
            _strcpy(p_call_id->number, number);
        }
        if(host){
            _strcpy(p_call_id->host, host);
        }
    }
    return;
}

void init_sip_cseq_t(sip_cseq_t *p_cseq, unsigned int number, char *method){
    if(p_cseq){
        if(number > 0){
            p_cseq->number = number;
        }
        if(method){
            _strcpy(p_cseq->method, method);
        }
    }
    return;
}

void init_sip_message_t(sip_message_t *p_message, char *version, char *method, int status_code, char *reason_phrase, char* contact, char *content_type, int content_length, char *message){
    if(p_message){
        if(version){
            _strcpy(p_message->version, version);
        }
        if(method){
            _strcpy(p_message->method, method);
        }
        p_message->status_code = status_code;
        if(reason_phrase){
            _strcpy(p_message->reason_phrase, reason_phrase);
        }
        if(contact){
            _strcpy(p_message->contact, contact);
        }
        if(content_type){
            _strcpy(p_message->content_type, content_type);
        }
        p_message->content_length = content_length;
        if(message){
            _strcpy(p_message->message, message);
        }
    }
    return;
}

////////////////////////
// Destroy struct
/////

#define destroy_sip_uri(scheme, username, host) \
    if(scheme){ \
        free(scheme); \
    } \
    if(username){ \
        free(username); \
    } \
    if(host){ \
        free(host); \
    }

void destroy_sip_ruri(sip_ruri_t *p_ruri){
    destroy_sip_uri(p_ruri->scheme, p_ruri->username, p_ruri->host)
    return;
}

void destroy_sip_from(sip_from_t *p_from){
    destroy_sip_uri(p_from->scheme, p_from->username, p_from->host)
    if(p_from->tag){
        free(p_from->tag);
    }
    return;
}

void destroy_sip_to(sip_to_t *p_to){
    destroy_sip_uri(p_to->scheme, p_to->username, p_to->host)
    if(p_to->tag){
        free(p_to->tag);
    }
    return;
}

void destroy_sip_call_id(sip_call_id_t *p_call_id){
    if(p_call_id->number){
        free(p_call_id->number);
    }
    if(p_call_id->host){
        free(p_call_id->host);
    }
    return;
}

void destroy_sip_cseq(sip_cseq_t *p_cseq){
    if(p_cseq->method){
        free(p_cseq->method);
    }
    return;
}

void destroy_sip_via(sip_via_t *p_via){
    if(p_via->via_str){
        free(p_via->via_str);
    }
    if(p_via->via_next){
        destroy_sip_via(p_via->via_next);
        free(p_via->via_next);
        p_via->via_next = NULL;
    }
    return;
}

void destroy_sip_message(sip_message_t *p_sip){
    if(p_sip->version){
        free(p_sip->version);
    }
    if(p_sip->method){
        free(p_sip->method);
    }
    if(p_sip->reason_phrase){
        free(p_sip->reason_phrase);
    }
    if(p_sip->contact){
        free(p_sip->contact);
    }
    if(p_sip->content_type){
        free(p_sip->content_type);
    }
    if(p_sip->message){
        free(p_sip->message);
    }
    destroy_sip_cseq(&p_sip->cseq);
    destroy_sip_call_id(&p_sip->call_id);
    destroy_sip_to(&p_sip->to);
    destroy_sip_from(&p_sip->from);
    destroy_sip_ruri(&p_sip->ruri);
    return;
}

////////////////////////
// Free struct
/////

#define MAKE_FN_FREE_STRUCT(x) void  free_sip_##x(sip_##x##_t **p_p_data){ \
        destroy_sip_##x(*p_p_data); \
        free(*p_p_data); \
        *p_p_data = NULL; \
        return; \
    }

#define FREE_SIP_STRUCT(struct_t) MAKE_FN_FREE_STRUCT(struct_t)

FREE_SIP_STRUCT(cseq)
FREE_SIP_STRUCT(to)
FREE_SIP_STRUCT(from)
FREE_SIP_STRUCT(ruri)
FREE_SIP_STRUCT(call_id)
FREE_SIP_STRUCT(message)
FREE_SIP_STRUCT(via)


#undef MAKE_FN_FREE_STRUCT
#undef FREE_SIP_STRUCT

