
#include "smpp_engine.h"

smpp_socket* new_smpp_socket(unsigned char *interface_name, unsigned char *ip_remote, unsigned int port_remote, unsigned char *user, unsigned char *passwd, int bind, unsigned char *system_type, int ton_src, int npi_src, int ton_dst, int npi_dst){
    smpp_socket *p_smpp_socket = (smpp_socket*)calloc(1,sizeof(smpp_socket));

/*    if(pthread_mutex_init(&p_smpp_socket->mutex, NULL) != 0){
        ERROR(LOG_SCREEN, "Error in mutex");
        return (int)-1;
    }
*/
    p_smpp_socket->interface_name = (uint8_t*)calloc(strlen((char*)interface_name)+1,sizeof(uint8_t));
    strcpy((char*)p_smpp_socket->interface_name,(char*)interface_name);

    p_smpp_socket->status = SMPP_DISCONNECT;
    
    p_smpp_socket->bind = bind;

    p_smpp_socket->ip_remote = (uint8_t*)calloc(strlen((char*)ip_remote)+1,sizeof(uint8_t));
    strcpy((char*)p_smpp_socket->ip_remote,(char*)ip_remote);

    p_smpp_socket->port_remote = port_remote;

    p_smpp_socket->sock = (socket_t*)calloc(1,sizeof(socket_t));

    //user = 16 chars max
    p_smpp_socket->user = (uint8_t*)calloc(strlen((char*)user)+1,sizeof(uint8_t));
    strcpy((char*)p_smpp_socket->user,(char*)user);

    //passwd = 9 chars max
    p_smpp_socket->passwd = (uint8_t*)calloc(strlen((char*)passwd)+1,sizeof(uint8_t));
    strcpy((char*)p_smpp_socket->passwd,(char*)passwd);
    
    //system_type = 13 chars max
    p_smpp_socket->system_type = (uint8_t*)calloc(strlen((char*)system_type)+1,sizeof(uint8_t));
    strcpy((char*)p_smpp_socket->system_type,(char*)system_type);

    p_smpp_socket->ton_src = ton_src;

    p_smpp_socket->npi_src = npi_src;

    p_smpp_socket->ton_dst = ton_dst;

    p_smpp_socket->npi_dst = npi_dst;

    return (smpp_socket*) p_smpp_socket;
}

void smpp_socket_free(smpp_socket **p_smpp_socket){
    if(p_smpp_socket && *p_smpp_socket){
        if((*p_smpp_socket)->ip_remote){
            free((*p_smpp_socket)->ip_remote);
        }
        if((*p_smpp_socket)->user){
            free((*p_smpp_socket)->user);
        }
        if((*p_smpp_socket)->passwd){
            free((*p_smpp_socket)->passwd);
        }
        if((*p_smpp_socket)->system_type){
            free((*p_smpp_socket)->system_type);
        }
//        socket_t_free(&(*p_smpp_socket)->sock);
        if((*p_smpp_socket)->sock){
            free((*p_smpp_socket)->sock);
        }
//	pthread_mutex_destroy(&(*p_smpp_socket)->mutex);
        free(*p_smpp_socket);
        *p_smpp_socket = NULL;
    }
    return;
}

/**
 * Used for MAP ans LIST
 */
void free_smpp_socket(void **p_p_data){
    //smpp_socket_free((smpp_socket**)p_p_data);
    return;
}

void* copy_smpp_socket(const void *p_data){
    /*Not used*/
    return NULL;
}

int compare_smpp_socket(const void *p_data1, const void *p_data2){
    int res = 0;
    smpp_socket *p_smpp_sock1 = (smpp_socket*)p_data1;
    smpp_socket *p_smpp_sock2 = (smpp_socket*)p_data2;
    if(strcmp((char*)p_smpp_sock1->interface_name, (char*)p_smpp_sock2->interface_name) != 0){
        return -1;
    }
    return (int) res;
}

/**
*  \brief This function is a constructor to the smpp structure, it allow to initialize a SMPP connection
*
*  \param  p_smpp_socket     It is a structure used to connect a SMPP session
*
*/
int smpp_start_connection(smpp_socket *p_smpp_socket){
    int ret = 0;
    if(p_smpp_socket != NULL && (p_smpp_socket->bind == BIND_TRANSCEIVER || p_smpp_socket->bind == BIND_TRANSMITTER || p_smpp_socket->bind == BIND_RECEIVER) &&
       p_smpp_socket->status == SMPP_DISCONNECT ){
        if(tcp_socket_client(p_smpp_socket->sock, p_smpp_socket->ip_remote, p_smpp_socket->port_remote) == -1){
            return (int) -1;
        }
        switch(p_smpp_socket->bind){
          case BIND_RECEIVER :
              if((ret = do_smpp_bind_receiver(p_smpp_socket->sock, p_smpp_socket->user, p_smpp_socket->passwd, p_smpp_socket->system_type,p_smpp_socket->ton_src,p_smpp_socket->npi_src)) == -1){
                  ERROR(LOG_SCREEN | LOG_FILE, RED"Error : smpp receiver connect."END_COLOR)
              }
              break;
          case BIND_TRANSMITTER :
              if((ret = do_smpp_bind_transmitter(p_smpp_socket->sock, p_smpp_socket->user, p_smpp_socket->passwd, p_smpp_socket->system_type,p_smpp_socket->ton_src,p_smpp_socket->npi_src)) == -1){
                  ERROR(LOG_SCREEN | LOG_FILE, RED"Error : smpp transmitter connect."END_COLOR)
              }
              break;
          case BIND_TRANSCEIVER :
              if((ret = do_smpp_bind_transceiver(p_smpp_socket->sock, p_smpp_socket->user, p_smpp_socket->passwd, p_smpp_socket->system_type,p_smpp_socket->ton_src,p_smpp_socket->npi_src)) == -1){
                  ERROR(LOG_SCREEN | LOG_FILE, RED"Error : smpp transceiver connect."END_COLOR)
              }
              break;
          default:
              ret = -1;
              break;
        }
        if(ret == -1){
            tcp_close(p_smpp_socket->sock);
            return (int) -1;
        }
        p_smpp_socket->status = SMPP_CONNECT;
        ERROR(LOG_SCREEN | LOG_FILE, GREEN"The connection to %s:%d SMPP Socket is ready"END_COLOR, p_smpp_socket->ip_remote, p_smpp_socket->port_remote)
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
int smpp_end_connection(smpp_socket *p_smpp_socket){
    if(p_smpp_socket && p_smpp_socket->status == SMPP_CONNECT){
        do_smpp_unbind(p_smpp_socket->sock);
        udp_close(p_smpp_socket->sock);
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
int smpp_restart_connection(smpp_socket *p_smpp_socket){
    smpp_end_connection(p_smpp_socket->sock);
    return (int) smpp_start_connection(p_smpp_socket->sock);
}

void display_sms(char *from_msisdn, char *to_msisdn, char *msg){
    if(from_msisdn && to_msisdn && msg){
        INFO(LOG_SCREEN, "------------------------------------------------------\n"
                         "  _______  \n"
                         " |\\_____/| from msisdn: %s\n"
                         " |_______| to msisdn: %s\n"
                         " Short Message :\n%s\n"
                         "------------------------------------------------------"
                         , (char*)from_msisdn, (char*)to_msisdn, (char*)msg)
    }
    return;
}

/**
 * SMPP PROCESSING
 */
static int _strcpy(char **str_out, char str_in);
static int _strcpy(char **str_out, char str_in){
    if(str_out && *str_out == NULL && strlen(str_in) > 0){
        *str_out = (char*)calloc(strlen(str_in)+1, sizeof(char));
        strcpy(*str_out, str_in);
        return (int) 0;
    }
	  return (int) -1;
}

int smpp_processing_sm(void *data, char **from, char **to, char **msg, unsigned int *sequence_number){
    int ret = -1;
    if(data && from && to && msg && sequence_num){
        data_sm_t *req = (data_sm_t*)data;
        switch(req->command_id){
            case DELIVER_SM : //client
            {   //copy SMS
                deliver_sm_t *deliver = (deliver_sm_t*)data;
                if(_strcpy(from, deliver->source_addr) == 0){
                    ERROR(LOG_SCREEN | LOG_FILE ,"Error invalid source_addr")
                }
                if(_strcpy(to, deliver->destination_addr) == 0){
                    ERROR(LOG_SCREEN | LOG_FILE ,"Error invalid destination_addr")
                }
                if(_strcpy(msg, deliver->short_message) == 0){//TODO tlv :    || _strcpy(msg, deliver->tlv.value.octet) == 0){
                    ERROR(LOG_SCREEN | LOG_FILE ,"Error invalid short_message")
                }
            }
                break;
            case SUBMIT_SM : //server
            {   //copy SMS
                submit_sm_t *submit = (submit_sm_t*)data;
                if(_strcpy(from, submit->source_addr) == 0){
                    ERROR(LOG_SCREEN | LOG_FILE ,"Error invalid source_addr")
                }
                if(_strcpy(to, submit->destination_addr) == 0){
                    ERROR(LOG_SCREEN | LOG_FILE ,"Error invalid destination_addr")
                }
                if(_strcpy(msg, submit->short_message) == 0){//TODO tlv :    || _strcpy(msg, submit->tlv.value.octet) == 0){
                    ERROR(LOG_SCREEN | LOG_FILE ,"Error invalid short_message")
                }
            }
                break;
            case SUBMIT_MULTI :
                WARNING(LOG_SCREEN, "SUBMIT_MULTI not allowed");
                break;
            default :
                WARNING(LOG_SCREEN, "Data parameter is not a SMS");
                break;
        }
        *sequence_number = req->sequence_number;
    }
    return (int) ret;
}

int smpp_processing_request(socket_t *sock, const void *data){
    int ret = -1;
    if(data){
        data_sm_t *req = (data_sm_t*)data;
        switch(req->command_id){
            case UNBIND :
                //Response
                ret = smpp_send_response(sock, UNBIND_RESP, req->sequence_number, 0);
                //processing
                //TODO
                break;
            case ENQUIRE_LINK :
                ret = smpp_send_response(sock, ENQUIRE_LINK_RESP, req->sequence_number, 0);
                break;
            case QUERY_SM :
                //TODO
                ret = smpp_send_response(sock, QUERY_SM_RESP, req->sequence_number, ESME_RINVCMDID);
                WARNING(LOG_SCREEN, "QUERY_SM not allowed")
                break;
            case REPLACE_SM :
                //TODO
                ret = smpp_send_response(sock, REPLACE_SM_RESP, req->sequence_number, ESME_RINVCMDID);
                WARNING(LOG_SCREEN, "REPLACE_SM not allowed")
                break;
            case CANCEL_SM :
                //TODO
                ret = smpp_send_response(sock, CANCEL_SM_RESP, req->sequence_number, ESME_RINVCMDID);
                WARNING(LOG_SCREEN, "CANCEL_SM not allowed")
                break;
            default:
                ret = smpp_send_response(sock, DATA_SM_RESP, req->sequence_number, ESME_RINVCMDID);
                ERROR(LOG_SCREEN | LOG_FILE ,"Request not allowed [%d:%d]", req.command_id, req.command_status)
                break;
        }
    }
    return (int) ret;
}

int smpp_processing_response(void *data){
    int ret = -1;
    if(data){
        data_sm_resp_t *resp = (data_sm_resp_t*)data;
        switch(resp->command_id){
            case DELIVER_SM_RESP :
                deliver_sm_resp_t *deliver = (deliver_sm_resp_t*)data;
                
                break;
            case SUBMIT_SM_RESP :
                submit_sm_resp_t *submit = (submit_sm_resp_t*)data;
                
                break;
            case UNBIND_RESP :
                //TODO
                WARNING(LOG_SCREEN, "UNBIND_RESP not allowed")
                break;
            case ENQUIRE_LINK_RESP :
                //TODO
                WARNING(LOG_SCREEN, "ENQUIRE_LINK_RESP not allowed")
                break;

            case QUERY_SM_RESP :
                //TODO
                WARNING(LOG_SCREEN, "QUERY_SM_RESP not allowed")
                break;
            case REPLACE_SM_RESP :
                //TODO
                WARNING(LOG_SCREEN, "REPLACE_SM_RESP not allowed")
                break;
            case CANCEL_SM_RESP :
                //TODO
                WARNING(LOG_SCREEN, "CANCEL_SM_RESP not allowed")
                break;
            default :  
                WARNING(LOG_SCREEN, "Response not allowed[%d]", resp->command_id);
                break;
        }
    }
    return (int) ret;
}

/**
 * SMPP Engine
 * in main file
 */
int smpp_engine(){
    int ret = -1;
    //smpp_scan_sock();
    //...
    return (int) ret;
}

/**
 * Routing function
 */
int send_sms_to_smpp(unsigned char* interface_name, unsigned char *from_msisdn, unsigned char *to_msisdn, unsigned char *msg){
    int res = 0;
    smpp_socket *p_sock = map_get(map_str_smpp, interface_name);
    if(p_sock->bind == BIND_TRANSMITTER || p_sock->bind == BIND_TRANSCEIVER){
//	pthread_mutex_lock(&p_sock->mutex);
        res =  do_smpp_send_sms(p_sock->sock,from_msisdn,to_msisdn,msg,p_sock->ton_src,p_sock->npi_src,p_sock->ton_dst,p_sock->npi_dst);
//	pthread_mutex_unlock(&p_sock->mutex);
    }else{
        ERROR(LOG_SCREEN | LOG_FILE, "SMPP Message don't send")
    }
    return (int) -1;
}

int send_sms_to_smpp_interface(unsigned char* interface_name_src, unsigned char *from_msisdn, unsigned char *to_msisdn, unsigned char *msg, unsigned char* interface_name_dst){
    return (int) -1;
}



