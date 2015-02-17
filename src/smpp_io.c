/**
*  \file ConnectionSMPP.cpp
*  
*  \brief This file contain all function to the Connection_SMPP object.
*  
*/
#include "smpp_io.h"
smpp_socket* new_smpp_socket(unsigned char *interface_name, unsigned char *ip_remote, unsigned int port_remote, unsigned char *user, unsigned char *passwd, int bind, unsigned char *system_type, int ton_src, int npi_src, int ton_dst, int npi_dst){
    smpp_socket *p_smpp_socket = (smpp_socket*)calloc(1,sizeof(smpp_socket));

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

int smpp_receive_sms(smpp_socket *p_smpp_socket, unsigned char **from_msisdn, unsigned char **to_msisdn, unsigned char **msg){
    if(p_smpp_socket && (p_smpp_socket->bind == BIND_RECEIVER || p_smpp_socket->bind == BIND_TRANSCEIVER)){
        return (int) do_smpp_receive_sms(p_smpp_socket->sock,from_msisdn,to_msisdn,msg);
    }
    ERROR(LOG_SCREEN | LOG_FILE, "SMPP Message don't receive")
    return (int) -1;
}

/**
*  \brief This function allow to display a SMS
*
*  \param from_msisdn  This parameter is the caller 
*  \param to_msisdn    This parameter is the callee
*  \param msg          This parameter is the message
*/
void display_sms(unsigned char *from_msisdn, unsigned char *to_msisdn, unsigned char *msg){
    if(from_msisdn && to_msisdn && msg){
        INFO(LOG_SCREEN, "------------------------------------------------------"
                         "  _______  "
                         " |\\_____/| from msisdn: %s"
                         " |_______| to msisdn: %s"
                         " Short Message :\n%s"
                         "------------------------------------------------------"
                         , (char*)from_msisdn, (char*)to_msisdn, (char*)msg)
    }
    return;
}


int send_sms_to_smpp(unsigned char* interface_name, unsigned char *from_msisdn, unsigned char *to_msisdn, unsigned char *msg){
    smpp_socket *p_sock = map_get(map_str_smpp, interface_name);
    if(p_sock->bind == BIND_TRANSMITTER || p_sock->bind == BIND_TRANSCEIVER){
        return (int) do_smpp_send_sms(p_sock->sock,from_msisdn,to_msisdn,msg,p_sock->ton_src,p_sock->npi_src,p_sock->ton_dst,p_sock->npi_dst);
    }
    ERROR(LOG_SCREEN | LOG_FILE, "SMPP Message don't send") 
    return (int) -1;
}

int send_sms_to_smpp_interface(unsigned char* interface_name_src, unsigned char *from_msisdn, unsigned char *to_msisdn, unsigned char *msg, unsigned char* interface_name_dst){
    return (int) -1;
}
