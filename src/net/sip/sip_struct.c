
#include "sip_struct.h"

//////////////////////////:
// TO STRING
/////

int sip_from_to_string(sip_from_t *p_from, char **p_buffer){
    if(p_from && p_buffer && *p_buffer){
        short position = 0;
        sprintf(*p_buffer, FROM_STR": %s <sip:%s@%s", p_from->username, p_from->username, p_from->host);
        position=strlen((char*)*p_buffer);
        if(p_from->port > 0){
            sprintf(*p_buffer+position, ":%d>", p_from->port);
            position=strlen((char*)*p_buffer);
        }
        if(p_from->tag && strlen((char*)p_from->tag) > 0){
            sprintf(*p_buffer+position, ">;tag=%s", p_from->tag);
        }else{
            sprintf(*p_buffer+position, ">");
        }
        return (int) 0;
    }
    return (int) -1;
}

int sip_to_to_string(sip_to_t *p_to, char **p_buffer){
    if(p_to && p_buffer && *p_buffer){
        short position = 0;
        sprintf(*p_buffer, TO_STR": %s <sip:%s@%s", p_to->username, p_to->username, p_to->host);
        position=strlen((char*)*p_buffer);
        if(p_to->port > 0){
            sprintf(*p_buffer+position, ":%d>", p_to->port);
            position=strlen((char*)*p_buffer);
        }
        if(p_to->tag && strlen((char*)p_to->tag) > 0){
            sprintf(*p_buffer+position, ";tag=%s", p_to->tag);
        }else{
            sprintf(*p_buffer+position, ">");
        }
        return (int) 0;
    }
    return (int) -1;
}

int sip_content_type_to_string(char *content_type, char **p_buffer){
    if(content_type && p_buffer && *p_buffer){
        sprintf(*p_buffer, CONTENT_TYPE_STR": %s", content_type);
        return (int) 0;
    }
    return (int) -1;
}

int sip_content_length_to_string(int content_length, char **p_buffer){
    if(p_buffer && *p_buffer){
        sprintf(*p_buffer, CONTENT_LENGTH_STR": %d", content_length);
        return (int) 0;
    }
    return (int) -1;
}

int sip_cseq_to_string(sip_cseq_t *p_cseq, char **p_buffer){
    if(p_cseq && p_buffer && *p_buffer){
        sprintf(*p_buffer, CSEQ_STR": %d %s", p_cseq->number, p_cseq->method);
        return (int) 0;
    }
    return (int) -1;
}

int sip_callid_to_string(sip_callid_t *p_callid, char **p_buffer){
    if(p_callid && p_buffer && *p_buffer){
        sprintf(*p_buffer, CALL_ID_STR": %s", p_callid->number);
        if(p_callid->host){
            sprintf(*p_buffer+strlen((char*)*p_buffer), "@%s", p_callid->host);
        }
        return (int) 0;
    }
    return (int) -1;
}

static int _sip_first_line_to_string(sip_message_t *p_sip, char **p_buffer, bool request){
    if(p_sip && p_buffer && *p_buffer){
        if(request){
            sprintf(*p_buffer, "%s sip:%s@%s", p_sip->method, p_sip->uri.username, p_sip->uri.host);
            if(p_sip->uri.port > 0){
                sprintf(*p_buffer+strlen((char*)*p_buffer), ":%d %s", p_sip->uri.port, p_sip->version);
            }else{
                sprintf(*p_buffer+strlen((char*)*p_buffer), " %s", p_sip->version);
            }
        }else{//Response  
            sprintf(*p_buffer, "%s %d %s", p_sip->version, p_sip->status_code, p_sip->reason_phrase);
        }
        return (int) 0;
    }
    return (int) -1;
}

int sip_message_to_string(sip_message_t *p_sip, char **p_buffer, bool request){
    if(p_sip && p_buffer && *p_buffer == NULL){
        char **imp = NULL;
        short i = 0;
        short max = request ? 9 : 7;
        imp = (char**)calloc(10, sizeof(char*));
        for(i=0;i<max;i++){
            imp[i] = calloc(128, sizeof(char));
        }
        _sip_first_line_to_string(p_sip, &imp[0], request);
        sip_from_to_string(&p_sip->from, &imp[1]);
        sip_to_to_string(&p_sip->to, &imp[2]);
        sip_cseq_to_string(&p_sip->cseq, &imp[3]);
        sip_callid_to_string(&p_sip->call_id, &imp[4]);
        sip_content_type_to_string(p_sip->content_type, &imp[5]);
        sip_content_length_to_string(p_sip->content_length, &imp[6]);
        if(request && strcmp(p_sip->method, MESSAGE_STR) == 0){
            //spacement
            strcpy(imp[8], p_sip->message);
        }
        implode(imp, "\r\n", p_buffer);
        for(i=0;i<max;i++){
            free(imp[i]);
        }
        free(imp);
        return (int) 0;
    }
    return (int) -1;
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

void destroy_sip_callid(sip_callid_t *p_callid){
    if(p_callid->number){
        free(p_callid->number);
    }
    if(p_callid->host){
        free(p_callid->host);
    }
    return;
}

void destroy_sip_cseq(sip_cseq_t *p_cseq){
    if(p_cseq->method){
        free(p_cseq->method);
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
    if(p_sip->content_type){
        free(p_sip->content_type);
    }
    if(p_sip->message){
        free(p_sip->message);
    }
    destroy_sip_cseq(&p_sip->cseq);
    destroy_sip_callid(&p_sip->call_id);
    destroy_sip_to(&p_sip->to);
    destroy_sip_from(&p_sip->from);
    destroy_sip_ruri(&p_sip->uri);
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
FREE_SIP_STRUCT(callid)
FREE_SIP_STRUCT(message)

#undef MAKE_FN_FREE_STRUCT
#undef FREE_SIP_STRUCT

