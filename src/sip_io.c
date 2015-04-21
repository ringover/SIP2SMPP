/**
 * In this file, all SIP function are regrouped.
 */
#include "sip_io.h"

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

map *map_session_sip; //<str(call_id), sip_data_t>

///////////////////////
////   sip_data_t  ////
///////////////////////

void init_sip_session(sip_session_t **p_p_sip, sip_message_t *p_msg_sip, void *p_sm){
    if(p_p_sip){
        sip_session_t *p_sip = NULL;
        if(*p_p_sip == NULL){
            *p_p_sip = new_sip_session_t();
        }
        p_sip = *p_p_sip;
        p_sip->p_msg_sip = p_msg_sip;
        p_sip->p_sm = p_sm;
    }
    return;
}

void free_sip_session(void **data){
    if(data && *data){
        sip_session_t *p_sip = (sip_session_t*)*data;
        free_sip_message(&p_sip->p_msg_sip);
        //p_sm is free but not here
        free(*data);
        *data = NULL;
    }
    return;
}

////////////////////////
////   SIP CONFIG   ////
////////////////////////

map  *cfg_sip;  // <str, config_sip_t>

inline void destroy_config_sip(config_sip_t *sip){
    if(sip->name)
        free(sip->name);
    //sip->name = NULL;
    if(sip->ip)
        free(sip->ip);
    //sip->ip = NULL;
    if(sip->routing_to)
        free(sip->routing_to);
    //sip->routing_to = NULL;
    //sip->port = 0;
    memset(sip, 0, sizeof(config_sip_t));
    return;
}

void free_config_sip(void **sip){
    destroy_config_sip((config_sip_t*)*sip);
    free(*sip);
    *sip = NULL;
    return;
}

int compare_config_sip(const void *sip1, const void *sip2){
    config_sip_t *s1 = (config_sip_t*)sip1;
    config_sip_t *s2 = (config_sip_t*)sip2;
    return (int) strcmp(s1->name, s2->name);
}

inline void display_config_sip(config_sip_t *sip){
    if(sip){
        char buffer[2048] = { 0 };
        sprintf(buffer, "[%s]\n"
                        STR_IP"         : %s\n"
                        STR_PORT"       : %d\n"
                        STR_ROUTING_TO" : %s\n",
                sip->name,
                sip->ip,
                sip->port,
                sip->routing_to);
        DEBUG(LOG_SCREEN, "\n%s", buffer)
    }
    return;
}

//////////////////////

int sip_start_connection(config_sip_t *p_config_sip){
    if(p_config_sip != NULL){
        if(!p_config_sip->sock){
            p_config_sip->sock = new_socket();
        }
        int ret = udp_socket(p_config_sip->sock, p_config_sip->ip, p_config_sip->port);
        return (int) ret;
    }
    return (int) -1;
}

int sip_end_connection(config_sip_t *p_config_sip){
    if(p_config_sip->sock){
        if(p_config_sip){
            int ret = udp_close(p_config_sip->sock);
            return (int) ret;
        }
        free(p_config_sip->sock);
    }
    return (int) -1;
}

int sip_restart_connection(config_sip_t *p_config_sip){
    int ret = sip_end_connection(p_config_sip);
    return (int) (ret == -1 ? -1 : sip_start_connection(p_config_sip));
}

///////////////////////
// RECV
/////

static int sip_recv_processing_request(socket_t *sock, sip_message_t *p_sip, char *interface, char *ip_origin, unsigned int port_origin){
    //SIP MESSAGE (SM)
    //go routing + save session (with call_id_number)
    sip_session_t *p_session = (sip_session_t*)map_get(map_session_sip, p_sip->call_id.number);
    if(p_session == NULL){
        char *k_call_id = NULL;
        p_session = new_sip_session_t();
        sm_data_t *p_sm = new_sm_data_t();
        //sm struct + insert db
        init_sm_data_t(p_sm, sock, 0, I_SIP, ip_origin, port_origin, p_sip, p_sip->from.username, p_sip->to.username, p_sip->message);
        p_sm->id = db_insert_sm(p_sip->call_id.number, 0, interface, ip_origin, port_origin, p_sm->src, p_sm->dst, p_sm->msg);
        //Key p_session
        _strcpy(k_call_id, p_sip->call_id.number);
        //Value p_session
        p_session->p_msg_sip = p_sip;
        p_session->p_sm = p_sm;
        //Set in map
        map_set(map_session_sip, k_call_id, p_session);
        //routing
        if(f_routing(interface, ip_origin, port_origin, p_sm) == -1){
            //send resp error
            ERROR(LOG_SCREEN | LOG_FILE, "Routing return -1 -> destroy SM/Session SMPP and sent error")
            p_sip->content_length = 0;
            p_sip->status_code = 401;
            _strcpy(p_sip->reason_phrase, UNAUTHORIZED_STR);
            sip_send_response(sock, ip_origin, port_origin, p_sip);
            //SMS DESTROY
            //map_erase(map_session_sip, k_call_id);
            free_sm_data(&p_sm);
        }
    }else{
        free_sip_message(&p_sip);
    }
    return (int) -1;
}

static int sip_recv_processing_response(sip_message_t *p_sip){
    //200 OK || 202 ACCEPTED
    sip_session_t *p_session = (sip_session_t*)map_get(map_session_sip, p_sip->call_id.number);
    if(p_session){
        switch(p_session->p_sm->type){
            case I_SIP :
            {   sip_message_t *p_sip_origin = (sip_message_t*)p_session->p_sm->p_msg_origin;
                p_sip_origin->status_code = OK;
                _strcpy(p_sip_origin->reason_phrase, OK_STR);
                sip_send_response(p_session->p_sm->sock, p_session->p_sm->ip_origin, p_session->p_sm->port_origin, p_sip_origin);
                map_erase(map_session_sip, p_sip_origin->call_id.number);
                //free_sip_message(p_sip_origin);
            }   break;
            case I_SMPP :
            {   generic_nack_t *p_smpp = (generic_nack_t*)p_session->p_sm->p_msg_origin;
                smpp_send_response(p_session->p_sm->sock, p_smpp->command_id & GENERIC_NACK, ESME_ROK, &p_smpp->sequence_number);
                map_erase(map_session_smpp, &p_smpp->sequence_number);
                //free(p_smpp);
            }   break;
            case I_SIGTRAN :
                //TODO
                break;
        }
    }
    db_delete_sm_by_id(p_session->p_sm->id);
    free_sm_data(&p_session->p_sm);
    map_erase(map_session_sip, p_sip->call_id.number);
    free_sip_message(&p_sip);
    return (int) -1;
}

static void* sip_recv_processing(void *data){
    void         **all_data    = (void**)data;
    sip_message_t *p_sip_msg   = (sip_message_t*)all_data[0];
    config_sip_t  *p_sip_conf  = (config_sip_t*)all_data[1];
    char          *interface   = (char*)all_data[2];
    char          *ip_origin   = (char*)all_data[3];
    unsigned int  port_origin = (unsigned int)*(all_data+4);
    if(p_sip_msg && ip_origin && port_origin && MSG_IS_MESSAGE(p_sip_msg->method)){
        sip_recv_processing_request(p_sip_conf->sock, p_sip_msg, interface, ip_origin, port_origin);
    }else if(p_sip_msg && (p_sip_msg->status_code == 200 || p_sip_msg->status_code == 202)){
        sip_recv_processing_response(p_sip_msg);
        goto free_origin_param;
    }else if(p_sip_msg && sip_what_is_the_method(p_sip_msg->method) != -1){
        //send 405
        p_sip_msg->status_code = 405;
        _strcpy(p_sip_msg->reason_phrase, METHOD_NOT_ALLOWED_STR);
        sip_send_response(p_sip_conf->sock, ip_origin, port_origin, p_sip_msg);
        //free all param
        goto free_all_param;
    }else{
        //not action
        ERROR(LOG_SCREEN, "SIP status code %d not implemented", p_sip_msg->status_code)
        goto free_all_param;
    }
    return (void*) NULL;
        
free_all_param:
    if(p_sip_msg){
        free_sip_message(&p_sip_msg);
    }
free_origin_param:
    if(ip_origin){
        free(ip_origin);
    }
    if(port_origin){
        free(port_origin);
    }
    return (void*) NULL;
}


int sip_engine(config_sip_t *p_sip_conf){
    int   ret   = -1;
    void **data = NULL;
    void *data_sip    = NULL;
    void *ip_remote   = NULL;
    unsigned int port_remote = 0;

    if((ret = sip_scan_sock(p_sip_conf->sock, (sip_message_t**)&data_sip, &ip_remote, &port_remote)) != -1){
        data = (void**)calloc(6, sizeof(void*));
        data[0] = data_sip;
        data[1] = p_sip_conf;
        data[2] = p_sip_conf->name;
        data[3] = ip_remote;
        data[4] = calloc(1, sizeof(unsigned int));
        *(data+4) = port_remote;
        threadpool_add(p_threadpool, sip_recv_processing, data, 0);
    }
    return (int) ret;
}

void create_default_sip_out_interface(char *str_sip){
    //default out SIP interface
    if(!str_sip){
        str_sip = "sip_out";
    }
    char *name = (char*)calloc(strlen(str_sip) + 1, sizeof(char));
    config_sip_t *p_config_sip = new_config_sip();
    p_config_sip->name = (char*)calloc(strlen(str_sip) + 1, sizeof(char));
    strcpy(p_config_sip->name, str_sip);
    strcpy(name, str_sip);
    map_set(cfg_sip, name, p_config_sip);
}

///////////////////////
// SEND
/////

int send_sms_to_sip(unsigned char *interface_name, sm_data_t *p_sm, unsigned char *ip_remote, unsigned int port_remote){
    config_sip_t  *p_sip_conf = map_get(cfg_sip, interface_name);
    if(p_sip_conf && ip_remote && port_remote > 0 && p_sm){
        char *k_session = NULL;
        sip_session_t *v_session = new_sip_session_t();
        //create SIP MESSAGE message
        sip_message_t *p_sip = new_sip_message_t();
        init_sip_message_t(p_sip, SIP_VERSION, MESSAGE_STR, 0, NULL, NULL, TEXT_PLAIN_STR, strlen(p_sm->msg), p_sm->msg);
        init_sip_ruri_t(&p_sip->ruri, "sip", p_sm->dst, ip_remote, port_remote);
        init_sip_from_t(&p_sip->from, "sip", p_sm->src, p_sip_conf->ip, p_sip_conf->port, NULL);
        init_sip_to_t(&p_sip->to, "sip",   p_sm->dst, ip_remote, port_remote, NULL);
        generate_call_id(&k_session);
        init_sip_call_id_t(&p_sip->call_id, k_session, p_sip_conf->ip);
        init_sip_cseq_t(&p_sip->cseq, (unsigned int)rand()%8000+1, MESSAGE_STR);
        //save session
        v_session->p_msg_sip = p_sip;
        v_session->p_sm = p_sm; 
        map_set(map_session_sip, k_session, v_session); 
        //send msg
        return (int) sip_send_request(p_sip_conf->sock, ip_remote, port_remote, p_sip);
    }
    return (int) -1;
}

