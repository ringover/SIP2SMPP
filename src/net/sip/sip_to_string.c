
#include "sip_to_string.h"

//////////////////////////
// TO STRING
/////

int sip_from_to_string(sip_from_t *p_from, char **p_buffer){
    if(p_from && p_buffer && *p_buffer){
        short position = 0;
        sprintf(*p_buffer, FROM_STR": %s <sip:%s@%s", p_from->username, p_from->username, p_from->host);
        position=strlen((char*)*p_buffer);
        if(p_from->port > 0){
            sprintf(*p_buffer+position, ":%d", p_from->port);
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
            sprintf(*p_buffer+position, ":%d", p_to->port);
            position=strlen((char*)*p_buffer);
        }
        if(p_to->tag && strlen((char*)p_to->tag) > 0){
            sprintf(*p_buffer+position, ">;tag=%s", p_to->tag);
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

int sip_contact_to_string(char *contact, char **p_buffer){
    if(p_buffer && *p_buffer){
        sprintf(*p_buffer, CONTACT_STR": %s", contact);
        return (int) 0;
    }
    return (int) -1;
}

int sip_via_to_string(sip_via_t *p_via, char **imp, short *i){
    if(p_via && imp && i){
        while(p_via){
            sprintf(*(imp + (*i)++), VIA_STR": %s", p_via->via_str);
            p_via = p_via->via_next;
        }
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

int sip_call_id_to_string(sip_call_id_t *p_call_id, char **p_buffer){
    if(p_call_id && p_buffer && *p_buffer){
        sprintf(*p_buffer, CALL_ID_STR": %s", p_call_id->number);
        if(p_call_id->host){
            sprintf(*p_buffer+strlen((char*)*p_buffer), "@%s", p_call_id->host);
        }
        return (int) 0;
    }
    return (int) -1;
}

static int _sip_first_line_to_string(sip_message_t *p_sip, char **p_buffer, bool request){
    if(p_sip && p_buffer && *p_buffer){
        if(request){
            sprintf(*p_buffer, "%s sip:%s@%s", p_sip->method, p_sip->ruri.username, p_sip->ruri.host);
            if(p_sip->ruri.port > 0){
                sprintf(*p_buffer+strlen((char*)*p_buffer), ":%d %s", p_sip->ruri.port, p_sip->version);
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
        if(strcmp(p_sip->method, MESSAGE_STR) == 0){
            max += 2;
        }
        if(p_sip->via){
            sip_via_t *p_via = p_sip->via;
            while(p_via){
                max++;
		p_via = p_via->via_next;
            }
        }
        if(p_sip->contact && strlen((char*)p_sip->contact) > 0){
            max++;
        }
        imp = (char**)calloc(max+1, sizeof(char*));
        for(i=0;i<max-1;i++){
            imp[i] = calloc(256, sizeof(char));
        }
        i = 0;

        _sip_first_line_to_string(p_sip, &imp[i++], request);
        if(p_sip->via){
            sip_via_to_string(p_sip->via, imp, &i);
        }
        sip_from_to_string(&p_sip->from, &imp[i++]);
        sip_to_to_string(&p_sip->to, &imp[i++]);
        sip_cseq_to_string(&p_sip->cseq, &imp[i++]);
        sip_call_id_to_string(&p_sip->call_id, &imp[i++]);
        if(p_sip->contact && strlen((char*)p_sip->contact) > 0){
            sip_contact_to_string(p_sip->contact, &imp[i++]);
        }
        sip_content_type_to_string(p_sip->content_type, &imp[i++]);
        sip_content_length_to_string(p_sip->content_length, &imp[i++]);
        if(request && strcmp(p_sip->method, MESSAGE_STR) == 0){
            //spacement
            i++;
            imp[i] = calloc(p_sip->content_length+1, sizeof(char));
            strncpy(imp[i], p_sip->message, p_sip->content_length);
        }
        implode(imp, "\r\n", p_buffer);
        for(i=0;i<max;i++){
            if(imp[i]) free(imp[i]);
        }
        free(imp);
        return (int) 0;
    }
    return (int) -1;
}

