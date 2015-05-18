
#include "smpp_io.h"

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

map *map_session_smpp;//<uint(sequence_number), smpp_data_t>

///////////////////////
// SMPP Session struct
/////

void init_smpp_session_t(smpp_session_t **p_p_smpp, unsigned int command_id, void *p_msg_smpp, void *p_sm){
    if(p_p_smpp){
        smpp_session_t *p_smpp = NULL;
        if(*p_p_smpp){
            *p_p_smpp = new_smpp_session_t();
        }
        (*p_p_smpp)->p_sm = p_sm;
        (*p_p_smpp)->p_msg_smpp = p_msg_smpp;
    }
    return;
}

void free_smpp_session(void **data){
    if(data && *data){
        smpp_session_t *p_smpp = (smpp_session_t*)*data;
        free(p_smpp->p_msg_smpp);
        //p_sm is free but not here
        free(*data);
        *data = NULL;
    }
    return;
}

///////////////////////
// SMPP Socket struct
//////

map  *cfg_smpp; // <str, config_smpp_t>

inline void destroy_config_client_smpp(config_client_smpp_t *c_smpp){
    if(c_smpp->name)
        free(c_smpp->name);
    //c_smpp->name  = NULL;
    //c_smpp->model = MODE_CLIENT;
    if(c_smpp->ip)
        free(c_smpp->ip);
    //c_smpp->ip = NULL;
    //c_smpp->port = 0;
    if(c_smpp->system_id)
        free(c_smpp->system_id);
    //c_smpp->system_id = NULL;
    if(c_smpp->password)
        free(c_smpp->password);
    //c_smpp->password = NULL;
    //c_smpp->ton = 0;
    //c_smpp->npi = 0;
    if(c_smpp->routing_to)
        free(c_smpp->routing_to);
    //c_smpp->routing_to = NULL;
    memset(c_smpp, 0, sizeof(config_client_smpp_t));
    return;
}

inline void destroy_config_smpp(config_smpp_t *smpp){
    if(smpp->name)
        free(smpp->name);
    //smpp->name  = NULL;
    //smpp->model = MODEL_CLIENT;
    if(smpp->ip)
        free(smpp->ip);
    //smpp->ip = NULL;
    //smpp->port = 0;
    if(smpp->system_id)
        free(smpp->system_id);
    //smpp->system_id = NULL;
    if(smpp->password)
        free(smpp->password);
    //smpp->password = NULL;
    //smpp->npi_src = 0;
    //smpp->ton_src = 0;
    //smpp->npi_dst = 0;
    //smpp->npi_dst = 0;
    if(smpp->system_type)
        free(smpp->system_type);
    if(smpp->service_type)
        free(smpp->service_type);
    //smpp->system_type = NULL;
    //smpp->cillabd_id = 0;
    if(smpp->routing_to)
        free(smpp->routing_to);
    //smpp->routing_to = NULL;
    if(smpp->list_c_smpp)
        map_destroy(&smpp->list_c_smpp);
    memset(smpp, 0, sizeof(config_smpp_t));
    return;
}

void free_config_client_smpp(void **c_smpp){
    destroy_config_client_smpp((config_client_smpp_t*)*c_smpp);
    free(*c_smpp);
    *c_smpp = NULL;
    return;
}

void free_config_smpp(void **smpp){
    destroy_config_smpp((config_smpp_t*)*smpp);
    free(*smpp);
    *smpp = NULL;
    return;
}

int compare_config_client_smpp(const void *c_smpp1, const void *c_smpp2){
    config_client_smpp_t *s1 = (config_client_smpp_t*)c_smpp1;
    config_client_smpp_t *s2 = (config_client_smpp_t*)c_smpp2;
    return (int) strcmp(s1->name, s2->name);
}

int compare_config_smpp(const void *smpp1, const void *smpp2){
    config_smpp_t *s1 = (config_smpp_t*)smpp1;
    config_smpp_t *s2 = (config_smpp_t*)smpp2;
    return (int) strcmp(s1->name, s2->name);
}

inline void display_config_client_smpp(config_client_smpp_t *c_smpp){
    if(c_smpp){
        char buffer[2048] = { 0 };
        sprintf(buffer, "[%s]\n"
                        STR_IP"         : %s\n"
                        STR_PORT"       : %d\n"
                        STR_SYSTEM_ID"  : %s\n"
                        STR_PASSWORD"   : %s\n"
                        STR_ROUTING_TO" : %s\n",
                c_smpp->name,
                c_smpp->ip,
                c_smpp->port,
                c_smpp->system_id,
                c_smpp->password,
                c_smpp->routing_to);
        DEBUG(LOG_SCREEN, "\n%s", buffer)
    }
    return;
}

inline void display_config_smpp(config_smpp_t *smpp){
    if(smpp){
        char buffer[2048] = { 0 };
        sprintf(buffer, "[%s]\n"
                        STR_MODEL"       : %s\n"
                        STR_IP"          : %s\n"
                        STR_PORT"        : %d\n"
                        STR_SYSTEM_ID"   : %s\n"
                        STR_PASSWORD"    : %s\n"
                        STR_NPI" : %s\n"
                        STR_TON"         : %s\n"
                        STR_SYSTEM_TYPE" : %s\n"
                        STR_BIND"        : %s\n"
                        STR_ROUTING_TO"  : %s\n",
                smpp->name,
                str_model[smpp->model],
                smpp->ip,
                smpp->port,
                smpp->system_id,
                smpp->password,
                npi_to_str(smpp->npi),
                ton_to_str(smpp->ton),
                smpp->system_type,
                bind_to_str(smpp->command_id),
                smpp->routing_to);
        DEBUG(LOG_SCREEN, "\n%s", buffer)
        if(smpp->list_c_smpp){
            //list clients
            iterator_map *p_it = smpp->list_c_smpp->begin;
            while(p_it){
                display_config_client_smpp((config_client_smpp_t*)p_it->value);
                p_it = p_it->next;
            }
        }
    }
    return;
}

//////////////////////////

int smpp_start_connection(config_smpp_t *p_config_smpp){
    int ret = 0;
    if(p_config_smpp != NULL && p_config_smpp->command_id > 0 && p_config_smpp->status == SMPP_DISCONNECT ){
        unsigned int sequence_number = new_uint32();
        if(!p_config_smpp->sock){
            p_config_smpp->sock = new_socket();
        }
        if(smpp_send_bind_client(p_config_smpp->sock, p_config_smpp->command_id, p_config_smpp->ip, p_config_smpp->port, p_config_smpp->system_id, p_config_smpp->password, p_config_smpp->system_type, p_config_smpp->ton, p_config_smpp->npi, sequence_number) != -1){
            //create session
            smpp_session_t *p_session = new_smpp_session_t();
            map_set(map_session_smpp, sequence_number, p_session);
            p_config_smpp->status = SMPP_CONNECT;
            INFO(LOG_SCREEN | LOG_FILE, "Wait SMPP connection of %s:%d", p_config_smpp->ip, p_config_smpp->port)
        }else{
            free(sequence_number);
        }
        return (int) 0;
    }
    return (int) -1;
}

int smpp_end_connection(config_smpp_t *p_config_smpp){
    if(p_config_smpp && p_config_smpp->status == SMPP_CONNECT && p_config_smpp->sock){
        //TODO: smpp_send_unbind(p_config_smpp->sock,);
        tcp_close(p_config_smpp->sock);
        p_config_smpp->status = SMPP_DISCONNECT;
        free(p_config_smpp->sock);
        return (int) 0;
    }
    INFO(LOG_SCREEN | LOG_FILE,"smpp already disconnect")
    return (int) -1;
}

int smpp_restart_connection(config_smpp_t *p_config_smpp){
    int ret = smpp_end_connection(p_config_smpp->sock);
    return (int) (ret == -1 ? -1 : smpp_start_connection(p_config_smpp->sock));
}

////////////////////////////
// SMPP PROCESSING
/////


#define copy_sm(type, data, src, dst, msg) \
    { \
        type *smt = (type*)data; \
        size_t size = strlen((char*)smt->source_addr); \
        _strncpy(src, (char*)smt->source_addr, size); \
        size = strlen((char*)smt->destination_addr); \
        _strncpy(dst, (char*)smt->destination_addr, size); \
        if(smt->short_message && (size = strlen((char*)smt->short_message)) > 0){ \
            _strncpy(msg, (char*)smt->short_message, size); \
        }else if(smt->tlv->tag == TLVID_message_payload && smt->tlv->length > 0){ \
            _strncpy(msg, smt->tlv->value.octet, smt->tlv->length); \
        }else{ \
            INFO(LOG_SCREEN | LOG_FILE, "Short message empty") \
            free(src); \
            free(dst); \
        } \
    } 

//int smpp_processing_request_sm(unsigned *interface, unsigned char *ip_repote, unsigned int port_remote, const void *data){
int smpp_recv_processing_request_sm(socket_t *sock, char *interface, char *ip_remote, unsigned int port_remote, void *data){
    //sent SM to Ronting function
    int ret = -1;
    if(sock && interface && ip_remote && data){
        sm_data_t *p_sm = new_sm_data_t();
        generic_nack_t *req = (generic_nack_t*)data;
        switch(req->command_id){
            case DELIVER_SM : //client
                copy_sm(deliver_sm_t, data, p_sm->src, p_sm->dst, p_sm->msg);
                break;
            case SUBMIT_SM : //server
                copy_sm(submit_sm_t, data, p_sm->src, p_sm->dst, p_sm->msg);
                break;
            case SUBMIT_MULTI :
                smpp_send_response(sock, req->command_id & GENERIC_NACK, ESME_RINVCMDID, req->sequence_number);
                INFO(LOG_SCREEN, "SUBMIT_MULTI not allowed")
                break;
            default :
                smpp_send_response(sock, req->command_id & GENERIC_NACK, ESME_RINVCMDID, req->sequence_number);
                INFO(LOG_SCREEN, "Data parameter is not a SMS")
                break;
        }
        if(interface && ip_remote && port_remote && p_sm->src && p_sm->dst && p_sm->msg){
            //création de sm_data_t + sauvegarde de la session SMPP
            init_sm_data_t(p_sm, sock, 0, I_SMPP, ip_remote, port_remote, data, NULL, NULL, NULL);
            p_sm->id = db_insert_sm("", req->sequence_number, interface, ip_remote, port_remote, p_sm->src, p_sm->dst, p_sm->msg);
            //save SMPP session
            unsigned int *k_smpp_data = new_uint32();//Key
            smpp_session_t *p_smpp = new_smpp_session_t();//Value
            init_smpp_session_t(&p_smpp, req->command_id, data, p_sm);
            *k_smpp_data = req->sequence_number;
            map_set(map_session_smpp, k_smpp_data, p_smpp);
            //routing
            if(f_routing(interface, ip_remote, port_remote, p_sm) == -1){
                //send resp error
                ERROR(LOG_SCREEN | LOG_FILE, "Routing return -1 -> destroy SM/Session SMPP and sent error")
                smpp_send_response(sock, req->command_id + GENERIC_NACK, ESME_RINVCMDID, &req->sequence_number);
                //SMS DESTROY
                map_erase(map_session_smpp, k_smpp_data);
                free_sm_data(&p_sm);
            }
        }else{
            free_sm_data((void**)&p_sm);
        }
    }
    return (int) ret;
}

int smpp_recv_processing_request(socket_t *sock, const void *req){
    //Action sent by server | The client receive those actions
    int ret = -1;
    if(req){
        switch(((generic_nack_t*)req)->command_id){
            case UNBIND :
            {   unbind_t *unbind = (unbind_t*)req;
		ret = smpp_send_unbind_resp(sock, unbind->sequence_number, ESME_ROK);
                //TODO : clean socket, ...
            }   break;
            case ENQUIRE_LINK :
            {   enquire_link_t *enq_l = (enquire_link_t*)req;
		ret = smpp_send_unquire_link_resp(sock, enq_l->sequence_number, ESME_ROK);
            }   break;
            case QUERY_SM :
            {   //TODO
                query_sm_t *query = (query_sm_t*)req;
                ret = smpp_send_response(sock, QUERY_SM_RESP, &query->sequence_number, ESME_RINVCMDID);
                INFO(LOG_SCREEN, "QUERY_SM not allowed")
            }   break;
            case REPLACE_SM :
            {   //TODO
                replace_sm_t *repl = (replace_sm_t*)req;
		ret = smpp_send_replace_sm_resp(sock, repl->sequence_number, ESME_RINVCMDID);
                INFO(LOG_SCREEN, "REPLACE_SM not allowed")
            }   break;
            case CANCEL_SM :
            {   //TODO
                cancel_sm_t *cancel = (cancel_sm_t*)req;
		ret = smpp_send_cancel_sm_resp(sock, cancel->sequence_number, ESME_RINVCMDID);
                INFO(LOG_SCREEN, "CANCEL_SM not allowed")
            }   break;
            case ALERT_NOTIFICATION :
            {   //TODO
                alert_notification_t *alert = (alert_notification_t*)req;
                INFO(LOG_SCREEN, "ALERT_NOTIFICATION not allowed")
            }   break;
            case BIND_TRANSMITTER :
            {   bind_transmitter_t *p_bind = (bind_transmitter_t*)req;
                smpp_send_bind_receiver_resp(sock, p_bind->system_id, p_bind->sequence_number, ESME_ROK, false);
            }   break;
            case BIND_RECEIVER :
            {   bind_receiver_t *p_bind = (bind_receiver_t*)req;
                smpp_send_bind_transmitter_resp(sock, p_bind->system_id, p_bind->sequence_number, ESME_ROK, false);
            }   break;
            case BIND_TRANSCEIVER :
            {   bind_transceiver_t *p_bind = (bind_transceiver_t*)req;
                smpp_send_bind_transceiver_resp(sock, p_bind->system_id, p_bind->sequence_number, ESME_ROK, false);
            }   break;
            default:
            {   ret = smpp_send_response(sock, DATA_SM_RESP, &((generic_nack_t*)req)->sequence_number, ESME_RINVCMDID);
                ERROR(LOG_SCREEN | LOG_FILE ,"Request not allowed [%d:%d]", ((generic_nack_t*)req)->command_id, ((generic_nack_t*)req)->command_status)
            }   break;
        }
    }
    free(req);
    return (int) ret;
}

#define smpp_response_sm(data, p_session) \
    /*Get original message*/ \
    switch(p_session->p_sm->type){ \
        case I_SIP : \
        {   sip_message_t *p_sip = (sip_message_t*)p_session->p_sm->p_msg_origin; \
            if(data->command_status == ESME_ROK){ \
                p_sip->status_code = 202; \
                _strcpy(p_sip->reason_phrase, ACCEPTED_STR); \
            }else{ \
                p_sip->status_code = 406; \
                _strcpy(p_sip->reason_phrase, NOT_ACCEPTABLE_STR); \
            } \
            p_sip->content_length = 0; \
            /*p_sip->cseq.number++;*/ \
            if(sip_send_response(p_session->p_sm->sock, p_session->p_sm->ip_origin, p_session->p_sm->port_origin, p_sip) != -1){ \
                /*Clean DB*/ \
                db_delete_sm_by_id(p_session->p_sm->id); \
                /*Clean Memory*/ \
                free_sm_data(&p_session->p_sm); \
                map_erase(map_session_sip, p_sip->call_id.number);/*clean origin session*/ \
                map_erase(map_session_smpp, &((generic_nack_t*)p_session->p_msg_smpp)->sequence_number);/*clean forward session*/ \
            } \
        }   break; \
        case I_SMPP : \
        {   generic_nack_t *p_smpp = (smpp_session_t*)p_session->p_sm->p_msg_origin; \
            if(smpp_send_response(p_session->p_sm->sock, p_smpp->command_id & GENERIC_NACK, data->command_status, &p_smpp->sequence_number) != -1){ \
                /*Clean DB*/ \
                db_delete_sm_by_id(p_session->p_sm->id); \
                /*Clean Memory*/ \
                free_sm_data(p_session->p_sm); \
                map_erase(map_session_smpp, p_smpp->sequence_number);/*clean origin session*/ \
                map_erase(map_session_smpp, &((generic_nack_t*)p_session->p_msg_smpp)->sequence_number);/*clean forward session*/ \
            } \
        }   break; \
        case I_SIGTRAN : /*not implemented*/ \
            break; \
    }

int smpp_recv_processing_response(void *res){
    int ret = -1;
    if(res){
        //
        smpp_session_t *p_session = (smpp_session_t*)map_get(map_session_smpp, &((generic_nack_t*)res)->sequence_number);
        if(p_session){
            //I have a response to my request
            switch(((generic_nack_t*)res)->command_id){
                case DELIVER_SM_RESP :
                {   deliver_sm_resp_t *deliver = (deliver_sm_resp_t*)res;
                    smpp_response_sm(deliver,p_session);
                }   break;
                case SUBMIT_SM_RESP :
                {   submit_sm_resp_t *submit = (submit_sm_resp_t*)res;
                    smpp_response_sm(submit,p_session);
                }   break;
                case UNBIND_RESP :
                    //TODO
                    INFO(LOG_SCREEN, "UNBIND_RESP not allowed")
                    break;
                case ENQUIRE_LINK_RESP :
                    //TODO
                    INFO(LOG_SCREEN, "ENQUIRE_LINK_RESP not allowed")
                    break;
                case QUERY_SM_RESP :
                    //TODO
                    INFO(LOG_SCREEN, "QUERY_SM_RESP not allowed")
                    break;
                case REPLACE_SM_RESP :
                    //TODO
                    INFO(LOG_SCREEN, "REPLACE_SM_RESP not allowed")
                    break;
                case CANCEL_SM_RESP :
                    //TODO
                    INFO(LOG_SCREEN, "CANCEL_SM_RESP not allowed")
                    break;
                case BIND_TRANSMITTER_RESP :
                case BIND_RECEIVER_RESP :
                case BIND_TRANSCEIVER_RESP :
                    map_erase(map_session_smpp, &((generic_nack_t*)res)->sequence_number);
                    break;
                default : 
                    INFO(LOG_SCREEN, "Response not allowed[%d]", ((generic_nack_t*)res)->command_id)
                    break;
            }
        }
    }
    return (int) ret;
}

static void* smpp_recv_processing(void *data){
    void **all_data = (void**)data;
    config_smpp_t *p_config_smpp = (config_smpp_t*)all_data[1];
    if(DELIVER_SM == ((generic_nack_t*)all_data[0])->command_id  || SUBMIT_SM == ((generic_nack_t*)all_data[0])->command_id){//REQUEST : SUBMIT or DELIVER
        return (void* )smpp_recv_processing_request_sm(p_config_smpp->sock, p_config_smpp->name, p_config_smpp->ip, p_config_smpp->port, all_data[0]);
    }else if(((generic_nack_t*)all_data[0])->command_id & GENERIC_NACK){//RESPONSE
        return (void*) smpp_recv_processing_response(all_data[0]);
    }else{//REQUEST
        return (void*) smpp_recv_processing_request(p_config_smpp->sock, all_data[0]);
    }
    free(all_data);
    return (void*) -1;
}

int smpp_engine(config_smpp_t *p_config_smpp){
    int   ret  = -1;
    void **data = NULL;
    void *data_smpp = NULL;

    if((ret = smpp_scan_sock(p_config_smpp->sock, &data_smpp)) != -1){
        data = (void**)calloc(3, sizeof(void*));
        data[0] = data_smpp;
        data[1] = p_config_smpp;
        threadpool_add(p_threadpool, smpp_recv_processing, data, 0);
    }
    return (int) ret;
}


/**
 * Routing function
 */

int send_sms_to_smpp(unsigned char* interface_name, sm_data_t *p_sm){
    int ret = -1;
    config_smpp_t  *p_config_smpp = map_get(cfg_smpp, interface_name);
    //create session + send submit_sm_t
    if(p_config_smpp && (p_config_smpp->command_id == BIND_TRANSMITTER || p_config_smpp->command_id == BIND_TRANSCEIVER) && p_sm){
        unsigned int *k_sequence_number = new_uint32();
        smpp_session_t *v_session = new_smpp_session_t();
        generic_nack_t *gen = (generic_nack_t*)calloc(1, sizeof(generic_nack_t));
        //......
        gen->sequence_number = get_sequence_number();
        *k_sequence_number = gen->sequence_number;
        v_session->command_id = SUBMIT_SM;
        v_session->p_msg_smpp = gen;
        v_session->p_sm = p_sm;
        map_set(map_session_smpp, k_sequence_number, v_session);
        ret = smpp_send_submit_sm(p_config_smpp->sock, p_sm->src, p_sm->dst, p_sm->msg, &(gen->sequence_number), p_config_smpp->ton, p_config_smpp->npi, p_config_smpp->ton, p_config_smpp->npi);
    }
    return (int) ret;
}


