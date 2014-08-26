/**
*  \file ConnectionSMPP.cpp
*  
*  \brief This file contain all function to the Connection_SMPP object.
*  
*/
#include "smpp_io.h"

static uint8_t type_bind_smpp_str[4][25] = {
        "SMPP_BIND_NONE",
        "SMPP_BIND_RECEIVER",
        "SMPP_BIND_TRANSMITTER",
        "SMPP_BIND_TRANSCEIVER"
};

uint8_t* type_bind_smpp_enum_to_str(type_bind_smpp_enum num){
    return (uint8_t*) type_bind_smpp_str[num];
}

/**
*  \brief This function return the enum number STR SMPP BIND
*
*  \param str_in   This parameter is the SMPP BIND mode
*/
type_bind_smpp_enum type_bind_smpp_str_to_enum(uint8_t *str_in){
        type_bind_smpp_enum i = SMPP_BIND_RECEIVER;
        while(i<=SMPP_BIND_TRANSCEIVER && strcmp((char*)type_bind_smpp_str[i],(char*)str_in) != 0) i++;
        return (i>SMPP_BIND_TRANSCEIVER ? SMPP_BIND_NONE : i);
}

smpp_socket* new_smpp_socket(uint8_t *ip_addr, uint16_t port, uint8_t *user, uint8_t *passwd, type_bind_smpp_enum type_bind, uint8_t *system_type, int8_t ton_src, int8_t npi_src, int8_t ton_dst, int8_t npi_dst){
    smpp_socket *p_smpp_socket = (smpp_socket*)calloc(1,sizeof(smpp_socket));

    p_smpp_socket->status = SMPP_DISCONNECT;
    
    p_smpp_socket->type_bind = type_bind;

    p_smpp_socket->ip_addr = (uint8_t*)calloc(strlen((char*)ip_addr)+1,sizeof(uint8_t));
    strcpy((char*)p_smpp_socket->ip_addr,(char*)ip_addr);

    p_smpp_socket->port = port;

    p_smpp_socket->sock = 0;

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

void free_smpp_socket(smpp_socket **p_smpp_socket){
    if(p_smpp_socket && *p_smpp_socket){
        if((*p_smpp_socket)->ip_addr){
            free((*p_smpp_socket)->ip_addr);
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
        free(*p_smpp_socket);
        *p_smpp_socket = NULL;
    }
    return;
}

/**
*  \brief This function is a constructor to the smpp structure, it allow to initialize a SMPP connection
*
*  \param  p_smpp_socket     It is a structure used to connect a SMPP session
*
*/
error_smpp smpp_start_connection(smpp_socket *p_smpp_socket){
    if(p_smpp_socket == NULL){
        ERROR(LOG_SCREEN | LOG_FILE,"SMPP socket is null")
        return (error_smpp) SMPP_ERROR_GENERAL;
    }
    if(p_smpp_socket->type_bind <= SMPP_BIND_NONE || p_smpp_socket->type_bind > SMPP_BIND_TRANSCEIVER){
        ERROR(LOG_SCREEN | LOG_FILE,"smpp bind")
        return (error_smpp) SMPP_ERROR_BIND;
    }
    if(p_smpp_socket->status == SMPP_DISCONNECT){
        if(do_tcp_connect(&(p_smpp_socket->sock),(uint8_t*)p_smpp_socket->ip_addr,(int16_t)p_smpp_socket->port) == -1){
            ERROR(LOG_SCREEN | LOG_FILE,"smpp connection")
            return (error_smpp) SMPP_ERROR_CONNECT;
        }
        switch(p_smpp_socket->type_bind){
        case SMPP_BIND_RECEIVER :
            if(do_smpp_connect_receiver(p_smpp_socket->sock, p_smpp_socket->user, p_smpp_socket->passwd, p_smpp_socket->system_type,p_smpp_socket->ton_src,p_smpp_socket->npi_src)){
                ERROR(LOG_SCREEN | LOG_FILE,"\033[0;31;40mError : smpp receiver connect.\033[0m")
            }
            break;
        case SMPP_BIND_TRANSMITTER :
            if(do_smpp_connect_transmitter(p_smpp_socket->sock, p_smpp_socket->user, p_smpp_socket->passwd, p_smpp_socket->system_type,p_smpp_socket->ton_src,p_smpp_socket->npi_src)){
                ERROR(LOG_SCREEN | LOG_FILE,"\033[0;31;40mError : smpp transmitter connect.\033[0m")
            }
            break;
        case SMPP_BIND_TRANSCEIVER :
            if(do_smpp_connect_transceiver(p_smpp_socket->sock, p_smpp_socket->user, p_smpp_socket->passwd, p_smpp_socket->system_type,p_smpp_socket->ton_src,p_smpp_socket->npi_src)){
                ERROR(LOG_SCREEN | LOG_FILE,"\033[0;31;40mError : smpp transceiver connect.\033[0m")
            }
            break;
        }
        p_smpp_socket->status = SMPP_CONNECT;
        INFO(LOG_SCREEN,"\033[0;33;40mSMPP loaded to %s mode\033[0m",type_bind_smpp_str[p_smpp_socket->type_bind])
        return (error_smpp) SMPP_ERROR_NO;
    }
    ERROR(LOG_SCREEN | LOG_FILE,"smpp ready connect")
    return (error_smpp) SMPP_ALREADY_CONNECT;
}

/**
*  \brief This function is used for disconnect the smpp socket
*
*  \param p_smpp_socket   This parameter is the SMPP socket structure
*
*/
error_smpp smpp_end_connection(smpp_socket *p_smpp_socket){
    if(p_smpp_socket && p_smpp_socket->status == SMPP_CONNECT){
        do_udp_close(p_smpp_socket->sock);
        p_smpp_socket->status = SMPP_DISCONNECT;
        return (error_smpp) SMPP_ERROR_NO;
    }
    INFO(LOG_SCREEN | LOG_FILE,"smpp already disconnect")
    return (error_smpp) SMPP_IS_NOT_CONNECT;
}

/**
*  \brief This function is used for restart the smpp socket
*
*  \param p_smpp_socket   This parameter is the SMPP socket structure
*
*/
error_smpp smpp_restart_connection(smpp_socket *p_smpp_socket){
    smpp_end_connection(p_smpp_socket);
    return (error_smpp) smpp_start_connection(p_smpp_socket);
}

error_smpp smpp_send_message(smpp_socket *p_smpp_socket, uint8_t *from_msisdn, uint8_t *to_msisdn, uint8_t *msg){
//    do_smpp_send_message(p_smpp_socket->sock,from_msisdn,to_msisdn,msg,p_smpp_socket->ton_src,p_smpp_socket->npi_src,p_smpp_socket->ton_dst,p_smpp_socket->npi_dst);

    if(p_smpp_socket &&
       (p_smpp_socket->type_bind == SMPP_BIND_TRANSMITTER || p_smpp_socket->type_bind == SMPP_BIND_TRANSCEIVER) &&
       from_msisdn && strlen((char*)from_msisdn) > 0 &&
       to_msisdn && strlen((char*)to_msisdn) > 0 &&
       msg && strlen((char*)msg) > 0 &&
       do_smpp_send_message(p_smpp_socket->sock,from_msisdn,to_msisdn,msg,p_smpp_socket->ton_src,p_smpp_socket->npi_src,p_smpp_socket->ton_dst,p_smpp_socket->npi_dst) == 0
       ){
            return (error_smpp) SMPP_ERROR_NO;
    }
    ERROR(LOG_SCREEN | LOG_FILE, "smpp message don't send")
    return (error_smpp) SMPP_ERROR_SEND;
}

error_smpp smpp_receive_message(smpp_socket *p_smpp_socket, uint8_t **from_msisdn, uint8_t **to_msisdn, uint8_t **msg){
    if(p_smpp_socket &&
       (p_smpp_socket->type_bind == SMPP_BIND_RECEIVER || p_smpp_socket->type_bind == SMPP_BIND_TRANSCEIVER) &&
       from_msisdn && *from_msisdn == NULL &&
       to_msisdn && *to_msisdn == NULL &&
       msg && *msg == NULL){
            if(do_smpp_receive_message(p_smpp_socket->sock,from_msisdn,to_msisdn,msg) == 0){
                return (error_smpp) SMPP_ERROR_NO;
            }
            return (error_smpp) SMPP_NO_MESSAGE;
    }
    ERROR(LOG_SCREEN | LOG_FILE, "smpp message don't receive")
    return (error_smpp) SMPP_ERROR_RECEIVE;
}

/**
*  \brief This function allow to display a SMS
*
*  \param from_msisdn  This parameter is the caller 
*  \param to_msisdn    This parameter is the callee
*  \param msg          This parameter is the message
*/
void display_smpp_message(uint8_t *from_msisdn, uint8_t *to_msisdn, uint8_t *msg){
    INFO(LOG_SCREEN, "------------------------------------------------------"
                      "  _______  "
                      " |\\_____/| source addr: %s"
                      " |_______| dest addr: %s"
                      " Short Message :\n%s"
                      "------------------------------------------------------"
                      , (char*)from_msisdn, (char*)to_msisdn, (char*)msg)
    return;
}

