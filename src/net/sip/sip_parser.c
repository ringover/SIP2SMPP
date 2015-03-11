
#include "sip_parser.h"

#define CONTENT -2

#ifndef _strncpy
#define _strncpy(dst, src, size) \
    dst = (char*)calloc(size+1, sizeof(char)); \
    strncpy(dst, src, size)
#endif /*_strncpy*/

#define ignore_angle_brackets(start,end) \
    if(*start == '<'){ \
        start++; \
    } \
    if(*end == '>'){ \
        end--; \
    } 

/////////////
// Parsing Function
//////

int sip_what_is_the_header(char *buffer){
    if(buffer){
        sip_header_t h = START_HEADER;
        short size_b = strlen((char*)buffer);
        if(size_b == 0){
            //next it is a content
            return (int) CONTENT;
        }
        do{
            short size_h = strlen((char*)tab_sip_header[h]);
            if(size_b >= size_h && strncmp((char*)buffer, (char*)tab_sip_header[h], size_h) == 0){
                return (int) h;
            }
        }while(++h != END_HEADER);
    }
    return (int) -1;
}

int sip_what_is_the_method(char *buffer){
    short size_b = 0;
    if(buffer && (size_b = strlen((char*)buffer))){
        sip_request_methods_t m = START_REQUEST;
        do{
            short size_m = strlen((char*)tab_sip_request_methods[m]);
            if(size_b >= size_m && strncmp((char*)buffer, (char*)tab_sip_request_methods[m], size_m) == 0){
                return (int) m;
            }
        }while(m++ != END_REQUEST);//?
    }
    return (int) -1;
}

int sip_msg_is_response(char *buffer){
    short size = strlen((char*)SIP_VERSION);
    if(buffer && strlen((char*)buffer) >  0 && strncmp(buffer, SIP_VERSION, size) == 0){
        return (int) atoi(buffer + size + 1);
    }
    return (int) -1;
}

#define _sip_parser_uri(start,end,scheme,username,host,port) \
    { \
        char *at    = NULL; \
        char *tmp   = NULL; \
        /*ignore '<' et '>'*/ \
        ignore_angle_brackets(start, end) \
        at = strchr(start, '@'); \
        if(at){ \
            /*Before @*/ \
            tmp = strchr(start, ':'); \
            if(tmp && tmp < at){ \
                /*sip:user@*/ \
                _strncpy(scheme, start, tmp-start); \
                _strncpy(username, ++tmp, at-tmp); \
            }else{ \
                /*user@*/ \
                _strncpy(username, start, at-start); \
            } \
            /*After @*/ \
            at++; \
            tmp = strchr(at, ':'); \
            if(tmp){ \
                /*domain:port*/ \
                _strncpy(host, at, tmp-at); \
                port = atoi(++tmp); \
            }else{ \
                /*domain*/ \
                _strncpy(host, at, end-at+1); \
            } \
        }else{ \
            ERROR(LOG_SCREEN, "URI Syntax Error") \
        } \
    } \

int sip_parser_ruri(sip_ruri_t *p_ruri, char *buffer){
    //MESSAGE <sip:user2@192.168.10.1:5060> SIP/2.0
    //or: <sip:user2@192.168.10.1:5060>
    if(p_ruri && buffer){
        char *start = NULL;
        char *end   = NULL;
        if((start = strchr(buffer, ' ')) != NULL && (end = strchr(++start, ' ')) != NULL){
            end--;
        }else{
            start = buffer;
            end   = buffer + strlen((char*)buffer);
        }
        _sip_parser_uri(start,end,p_ruri->scheme,p_ruri->username,p_ruri->host,p_ruri->port);
    }else{
        ERROR(LOG_SCREEN, "R-URI Syntax error");
        return (int) -1;
    }
    return (int) 0;
}

int sip_parser_from(sip_from_t *p_from, char *buffer){
    //From: sip:user1@domain.com;tag=49583
    //From: alice <sip:alice@atlanta.example.com>;tag=9fxced76sl 
    if(p_from && buffer && strncmp(buffer, FROM_STR, strlen((char*)FROM_STR)) == 0){
        char *start = strchr(buffer, ' ');
        char *end = strchr(++start, ' ');
        if(end){
            start = ++end;
        }
        end = strchr(start, ';');
        if(end){
            //;tag=123456789
            _strncpy(p_from->tag, end+6, strlen(end--)-5);
        }else{
            end = start+strlen((char*)start);
        }
        _sip_parser_uri(start,end,p_from->scheme,p_from->username,p_from->host,p_from->port);
    }else{
        ERROR(LOG_SCREEN, "From Syntax error");
        return (int) -1;
    }
    return (int) 0;
}

int sip_parser_to(sip_to_t *p_to, char *buffer){
    //To: sip:user2@domain.com
    //or To: alice <sip:alice@atlanta.example.com>;tag=9fxced76sl 
    if(p_to && buffer && strncmp(buffer, TO_STR, strlen((char*)TO_STR)) == 0){
        char *start = strchr(buffer, ' ');
        char *end = strchr(++start, ' ');
        if(end){
            start = ++end;
        }
        end = strchr(start, ';');
        if(end){
            //;tag=123456789
            _strncpy(p_to->tag, end+6, strlen(end--)-5);
        }else{
            end = start+strlen((char*)start);
        }
        _sip_parser_uri(start,end,p_to->scheme,p_to->username,p_to->host,p_to->port);
    }else{
        ERROR(LOG_SCREEN, "To Syntax error");
        return (int) -1;
    }
    return (int) 0;
}

int sip_parser_callid(sip_callid_t *p_callid, char *buffer){
    //Call-ID: asd88asd77a@1.2.3.4
    //Call-ID: asd88asd77a
    if(p_callid && buffer && strncmp(buffer, CALL_ID_STR, strlen((char*)CALL_ID_STR)) == 0){
        char *start = strchr(buffer, ' ');
        char *at    = strchr(++start, '@');
        if(at){
            _strncpy(p_callid->number, start, at-start);
            _strncpy(p_callid->host, ++at, strlen((char*)at));
        }else{
            _strncpy(p_callid->number, start, strlen((char*)start));
        }
    }else{
        ERROR(LOG_SCREEN, "Call-ID Syntax error");
        return (int) -1;
    }
    return (int) 0;
}

int sip_parser_content_type(char** p_content_type, char *buffer){
    //Content-Type: text/plain
    if(p_content_type && buffer && strncmp(buffer, CONTENT_TYPE_STR, strlen((char*)CONTENT_TYPE_STR)) == 0){
        char *start = strchr(buffer, ' ');
        _strncpy(*p_content_type, ++start, strlen((char*)start));
    }else{
        ERROR(LOG_SCREEN, "Content-Type Syntax error");
        return (int) -1;
    }
    return (int) 0;
}

int sip_parser_content_length(int *p_content_length, char *buffer){
    //Content-Length: 18
    if(p_content_length && buffer && strncmp(buffer, CONTENT_LENGTH_STR, strlen((char*)CONTENT_LENGTH_STR)) == 0){
        char *start = strchr(buffer, ' ');
        *p_content_length = atoi(++start);
    }else{
        ERROR(LOG_SCREEN, "Content-Length Syntax error");
        return (int) -1;
    }
    return (int) 0;
}

int sip_parser_cseq(sip_cseq_t *p_cseq, char *buffer){
    //CSeq: 1 MESSAGE
    if(p_cseq && buffer){
        char *start = strchr(buffer, ' ');
        p_cseq->number = atoi(++start);
        start = strchr(start, ' ');
        start++;
        _strncpy(p_cseq->method, start, strlen((char*)start));
    }else{
        ERROR(LOG_SCREEN, "CSEQ Syntax error");
        return (int) -1;
    }
    return (int) 0;
}

int sip_parser_first_line(sip_message_t *p_sip, char *buffer){
    char *c1 = NULL;
    char *c2 = NULL;
    c1 = buffer;
    c2 = strchr(buffer, ' ');
    if(c2 == NULL || c1 == NULL){
        ERROR(LOG_SCREEN | LOG_FILE, "SIP Syntax error")
        return (int) -1;
    }
    if(sip_msg_is_response(c1) == 0){
        //SIP/2.0 200 OK
        //SIP_VERSION
        _strncpy(p_sip->version,c1,c2-c1);
        c1 = strchr(++c2, ' ');
        if(c1 == NULL){
            ERROR(LOG_SCREEN | LOG_FILE, "SIP Syntax error")
            return (int) -1;
        }
        //SIP CODE
        p_sip->status_code = atoi(c2);
        c1++;
        //The reason phrase
        _strncpy(p_sip->reason_phrase,c1,strlen((char*)c1));
    }else if(sip_msg_is_request(buffer) >= 0){
        //MESSAGE sip:user2@domain.com SIP/2.0
        _strncpy(p_sip->method,c1,c2-c1);
        c1 = strchr(++c2, ' ');
        if(c1 == NULL){
            ERROR(LOG_SCREEN | LOG_FILE, "SIP Syntax error")
            return (int) -1;
        }
        //SIP RURI
        {   char *ruri = NULL;
            _strncpy(ruri,c2,c1-c2);
            if(sip_parser_ruri(&(p_sip->uri), ruri) == -1){
                return (int) -1;
            }
            free(ruri);
        }
        c1++;
        //SIP version
        _strncpy(p_sip->version, c1, strlen((char*)c1));
    }else{
        ERROR(LOG_SCREEN | LOG_FILE, "It's not SIP Syntax")
        return (int) -1;
    }
    return (int) 0;
}

int sip_parser_content(sip_message_t *p_sip, char **exp, short *i){
    if(p_sip && exp && i){
        _strncpy(p_sip->message, *(exp+(*i)), strlen((char*)*(exp+(*i))));
    }else{
        ERROR(LOG_SCREEN, "Content error");
        return (int) -1;
    }
    return (int) 0;
}

int sip_parser_message(sip_message_t *p_sip, char *buffer){
    char **exp = NULL;
    short i = 0;
    if(p_sip && buffer && strlen(buffer) > 0){
        exp = explode(buffer,"\r\n");
        if(exp != NULL){
            sip_parser_first_line(p_sip, *(exp + i++));
            while(*(exp+i)){
                switch(sip_what_is_the_header(*(exp+i))){
                    case FROM :
                        sip_parser_from(&p_sip->from, *(exp+i));
                        break;
                    case TO :
                        sip_parser_to(&p_sip->to, *(exp+i));
                        break;
                    case CALL_ID :
                        sip_parser_callid(&p_sip->call_id, *(exp+i));
                        break;
                    case CSEQ :
                        sip_parser_cseq(&p_sip->cseq, *(exp+i));
                        break;
                    case CONTENT_TYPE :
                        sip_parser_content_type(&p_sip->content_type, *(exp+i));
                        break;
                    case CONTENT_LENGTH :
                        sip_parser_content_length(&p_sip->content_length, *(exp+i));
                        break;
                    case CONTENT ://"\r\n" line
                        i++;
                        if(*(exp+i)){
                            sip_parser_content(p_sip, exp, &i);
                        }
                        break;
                }
                i++;
            }
            if(exp){
                i = 0;
                while(*(exp+i)){
                    free(*(exp + i++));
                }
                free(exp);
            }
        }else{
            ERROR(LOG_SCREEN | LOG_FILE, "SIP Syntax error")
            return (int) -1;
        }
        return (int) 0;
    }
    return (int) -1;
}

#undef ignore_angle_brackets

