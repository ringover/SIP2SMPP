
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
map *map_iface_smpp;  //<str(name_interface), sip_socket_t>

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

void init_smpp_socket_t(smpp_socket_t *p_smpp_socket, unsigned char *interface_name, unsigned char *ip_host, unsigned int port_host, unsigned char *ip_remote, unsigned int port_remote, unsigned char *user, unsigned char *passwd, int bind, unsigned char *system_type, int ton_src, int npi_src, int ton_dst, int npi_dst){
    if(p_smpp_socket){
        if(interface_name){
            _strcpy(p_smpp_socket->interface, interface_name);
        }

        p_smpp_socket->status = SMPP_DISCONNECT;

        if(bind > 0){    
            p_smpp_socket->bind = bind;
        }

        if(p_smpp_socket->sock == NULL){
            p_smpp_socket->sock = new_socket_t();
        }

        if(ip_host){
            _strcpy(p_smpp_socket->sock->ip, ip_host);
        }

        if(port_host > 0){
            p_smpp_socket->sock->port = port_host;
        }

        if(ip_remote){
            _strcpy(p_smpp_socket->ip_remote, ip_remote);
        }
        if(port_remote > 0){
            p_smpp_socket->port_remote = port_remote;
        }

        //user = 16 chars max
        if(user && strlen(user) <= 16){
            _strcpy(p_smpp_socket->user, user);
        }else{
            ERROR(LOG_SCREEN | LOG_FILE, "user SMPP <= 16");
        }

        //passwd = 9 chars max
        if(passwd && strlen(passwd) <= 9){
            _strcpy(p_smpp_socket->passwd, passwd);
        }else{
            ERROR(LOG_SCREEN | LOG_FILE, "passwd SMPP <= 9");
        }

        //system_type = 13 chars max
        if(system_type && strlen(system_type) <= 13){
            _strcpy(p_smpp_socket->system_type, system_type);
        }else{
            ERROR(LOG_SCREEN | LOG_FILE, "system_type SMPP <= 13");
        }

        p_smpp_socket->ton_src = ton_src;

        p_smpp_socket->npi_src = npi_src;

        p_smpp_socket->ton_dst = ton_dst;

        p_smpp_socket->npi_dst = npi_dst;
    }
    return;
}

inline void free_smpp_socket(void **p_p_data){
    if(p_p_data && *p_p_data){
        smpp_socket_t *p_smpp_sock = (smpp_socket_t*)*p_p_data;

        if(p_smpp_sock->interface){
            free(p_smpp_sock->interface);
        }

        if(p_smpp_sock->ip_remote){
            free(p_smpp_sock->ip_remote);
        }

        if(p_smpp_sock->user){
            free(p_smpp_sock->user);
        }

        if(p_smpp_sock->passwd){
            free(p_smpp_sock->passwd);
        }

        if(p_smpp_sock->system_type){
            free(p_smpp_sock->system_type);
        }

        tcp_close(p_smpp_sock->sock->socket);

        if(p_smpp_sock->sock->ip){
            free(p_smpp_sock->sock->ip);
        }

        free(p_smpp_sock);
        *p_p_data = NULL;
    }
    return;
}

//////////////////////////

/**
*  \brief This function is a constructor to the smpp structure, it allow to initialize a SMPP connection
*
*  \param  p_smpp_socket     It is a structure used to connect a SMPP session
*
*/
int smpp_start_connection(smpp_socket_t *p_smpp_socket){
    int ret = 0;
    if(p_smpp_socket != NULL && p_smpp_socket->bind > 0 && p_smpp_socket->status == SMPP_DISCONNECT ){
        unsigned int sequence_number = new_uint32();
        if(smpp_send_bind_client(p_smpp_socket->sock, p_smpp_socket->bind, p_smpp_socket->ip_remote, p_smpp_socket->port_remote,
                                 p_smpp_socket->user, p_smpp_socket->passwd, p_smpp_socket->system_type, p_smpp_socket->ton_src, p_smpp_socket->npi_src, sequence_number) != -1){
            //create session
            smpp_session_t *p_session = new_smpp_session_t();
            map_set(map_session_smpp, sequence_number, p_session);
            p_smpp_socket->status = SMPP_CONNECT;
            INFO(LOG_SCREEN | LOG_FILE, "Wait SMPP connection of %s:%d", p_smpp_socket->ip_remote, p_smpp_socket->port_remote)
        }else{
            free(sequence_number);
        }
        return (int) 0;
    }
    return (int) -1;
}

/**
*  \brief This function is used for disconnect the smpp socket
*
*  \param p_smpp_socket   This parameter is the SMPP socket structure
*
*/
int smpp_end_connection(smpp_socket_t *p_smpp_socket){
    if(p_smpp_socket && p_smpp_socket->status == SMPP_CONNECT){
        //TODO: smpp_send_unbind(p_smpp_socket->sock,);
        tcp_close(p_smpp_socket->sock);
        p_smpp_socket->status = SMPP_DISCONNECT;
        return (int) 0;
    }
    INFO(LOG_SCREEN | LOG_FILE,"smpp already disconnect")
    return (int) -1;
}

/**
*  \brief This function is used for restart the smpp socket
*
*  \param p_smpp_socket   This parameter is the SMPP socket structure
*
*/
int smpp_restart_connection(smpp_socket_t *p_smpp_socket){
    smpp_end_connection(p_smpp_socket->sock);
    return (int) smpp_start_connection(p_smpp_socket->sock);
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
            init_sm_data_t(p_sm, 0, sock, I_SMPP, ip_remote, port_remote, data, NULL, NULL, NULL);
            p_sm->id = db_insert_sm("", req->sequence_number, interface, ip_remote, port_remote, p_sm->src, p_sm->dst, p_sm->msg);
            //save SMPP session
            unsigned int *k_smpp_data = new_uint32();//Key
            smpp_session_t *p_smpp = new_smpp_session_t();//Value
            init_smpp_session_t(&p_smpp, req->command_id, data, p_sm);
            *k_smpp_data = req->sequence_number;
            map_set(map_session_smpp, k_smpp_data, p_smpp);
            //routing
            if(routing(interface, ip_remote, port_remote, p_sm) == -1){
                //send resp error
                ERROR(LOG_SCREEN | LOG_FILE, "Routing return -1 -> destroy SM/Session SMPP and sent error")
                smpp_send_response(sock, req->command_id & GENERIC_NACK, ESME_RINVCMDID, req->sequence_number);
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
                ret = smpp_send_response(sock, UNBIND_RESP, unbind->sequence_number, ESME_ROK);
                //TODO : clean socket, ...
            }   break;
            case ENQUIRE_LINK :
            {   enquire_link_t *enq_l = (enquire_link_t*)req;
                ret = smpp_send_response(sock, ENQUIRE_LINK_RESP, enq_l->sequence_number, ESME_ROK);
            }   break;
            case QUERY_SM :
            {   //TODO
                query_sm_t *query = (query_sm_t*)req;
                ret = smpp_send_response(sock, QUERY_SM_RESP, query->sequence_number, ESME_RINVCMDID);
                INFO(LOG_SCREEN, "QUERY_SM not allowed")
            }   break;
            case REPLACE_SM :
            {   //TODO
                replace_sm_t *repl = (replace_sm_t*)req;
                ret = smpp_send_response(sock, REPLACE_SM_RESP, repl->sequence_number, ESME_RINVCMDID);
                INFO(LOG_SCREEN, "REPLACE_SM not allowed")
            }   break;
            case CANCEL_SM :
            {   //TODO
                cancel_sm_t *cancel = (cancel_sm_t*)req;
                ret = smpp_send_response(sock, CANCEL_SM_RESP, cancel->sequence_number, ESME_RINVCMDID);
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
            {   ret = smpp_send_response(sock, DATA_SM_RESP, ((generic_nack_t*)req)->sequence_number, ESME_RINVCMDID);
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
                p_sip->status_code = 200; \
                _strcpy(p_sip->reason_phrase, OK_STR); \
            }else{ \
                p_sip->status_code = 406; \
                _strcpy(p_sip->reason_phrase, NOT_ACCEPTABLE_STR); \
            } \
            p_sip->content_length = 0; \
            p_sip->cseq.number++; \
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
    smpp_socket_t *smpp_sock = (smpp_socket_t*)all_data[1];
    if(DELIVER_SM == ((generic_nack_t*)all_data[0])->command_id  || SUBMIT_SM == ((generic_nack_t*)all_data[0])->command_id){//REQUEST : SUBMIT or DELIVER
        return smpp_recv_processing_request_sm(smpp_sock->sock, smpp_sock->interface, smpp_sock->ip_remote, smpp_sock->port_remote, all_data[0]);
    }else if(((generic_nack_t*)all_data[0])->command_id & GENERIC_NACK){//RESPONSE
        return smpp_recv_processing_response(all_data[0]);
    }else{//REQUEST
        return smpp_recv_processing_request(smpp_sock->sock, all_data[0]);
    }
    free(all_data);
    return -1;
}

int smpp_engine(smpp_socket_t *p_smpp_sock){
    int   ret  = -1;
    void **data = NULL;
    void *data_smpp = NULL;

    if((ret = smpp_scan_sock(p_smpp_sock->sock, &data_smpp)) != -1){
        data = (void**)calloc(3, sizeof(void*));
        data[0] = data_smpp;
        data[1] = p_smpp_sock;
        threadpool_add(p_threadpool, smpp_recv_processing, data, 0);
    }
    return (int) ret;
}


/**
 * Routing function
 */

int send_sms_to_client_smpp(unsigned char* interface_name, sm_data_t *p_sm){
    int ret = -1;
    smpp_socket_t  *p_sock = map_get(map_iface_smpp, interface_name);
    //create session + send submit_sm_t
    if(p_sock && (p_sock->bind == BIND_TRANSMITTER || p_sock->bind == BIND_TRANSCEIVER) && p_sm){
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
        ret = smpp_send_submit_sm(p_sock->sock, p_sm->src, p_sm->dst, p_sm->msg, &(gen->sequence_number), p_sock->ton_src, p_sock->npi_src, p_sock->ton_dst, p_sock->npi_dst);
    }
    return (int) ret;
}

int send_sms_to_server_smpp(unsigned char* interface_name, sm_data_t *p_sm, char *ip_client, unsigned int port_client){
    return (int) -1;
}


