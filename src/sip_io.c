/**
* In this file, all SIP function are regrouped.
*/
#include "sip_io.h"

static uint8_t type_bind_sip_str[4][25] = {
  "SIP_BIND_NONE",
  "SIP_BIND_RECEIVER",
  "SIP_BIND_TRANSMITTER",
  "SIP_BIND_TRANSCEIVER"
};

/**
*  \brief This function return the enum number STR SIP BIND
*
*  \param str_in   This parameter is the SIP BIND mode
*/
type_bind_sip_enum type_bind_sip_str_to_enum(uint8_t *str_in){
	type_bind_sip_enum i = SIP_BIND_RECEIVER;
	while(i<=SIP_BIND_TRANSCEIVER && strcmp((char*)type_bind_sip_str[i],(char*)str_in) != 0) i++;
	return (i>SIP_BIND_TRANSCEIVER ? SIP_BIND_NONE : i);
}

/**
*  \brief This function create a sip_socket structure
*
*  \param ip_addr   This parameter is the IP of the local SIP interface
*  \param port      This parameter is the Port of the local SIP interface
*  \param type_bin  This parameter is mode of connection : SIP_BIND_TRANSMITTER | SIP_BIND_RECEIVER | SIP_BIND_TRANSCEIVER
*
*/
sip_socket* new_sip_socket(uint8_t *ip_addr, uint16_t port, type_bind_sip_enum type_bind){
    sip_socket *p_sip_socket = NULL;
    p_sip_socket = (sip_socket*)calloc(1,sizeof(sip_socket));

    p_sip_socket->ip_addr = calloc(strlen((char*)ip_addr)+1,sizeof(uint8_t));
    strcpy((char*)p_sip_socket->ip_addr, (char*)ip_addr);

    p_sip_socket->port = port;

    p_sip_socket->type_bind = type_bind;

    p_sip_socket->status = SIP_DISCONNECT;
    p_sip_socket->sock   = 0;	

    return (sip_socket*)p_sip_socket;
}

void free_sip_socket(sip_socket **p_sip_socket){
    if(p_sip_socket && *p_sip_socket){
        if((*p_sip_socket)->ip_addr){
            free((*p_sip_socket)->ip_addr);
        }
        free(*p_sip_socket);
        *p_sip_socket = NULL;
    }
    return;
}

/**
*  \brief This function is used for connect the sip socket
*
*  \param p_sip_socket   This parameter is the SIP socket structure
*
*/
error_sip sip_start_connection(sip_socket *p_sip_socket){
    if(p_sip_socket == NULL){
        ERROR(LOG_SCREEN | LOG_FILE,"SIP socket is null");
        return (error_sip) SIP_ERROR_GENERAL; 
    }
    if(p_sip_socket->type_bind <= SIP_BIND_NONE || p_sip_socket->type_bind > SIP_BIND_TRANSCEIVER){
        ERROR(LOG_SCREEN | LOG_FILE,"sip bind");
        return (error_sip) SIP_ERROR_BIND;
    }
    if(p_sip_socket->status == SIP_DISCONNECT){
	if(do_udp_connect(&(p_sip_socket->sock),(uint8_t*)p_sip_socket->ip_addr,(int16_t)p_sip_socket->port) == -1){
            ERROR(LOG_SCREEN | LOG_FILE,"sip connection");
            return (error_sip) SIP_ERROR_CONNECT;
        }
        p_sip_socket->status = SIP_CONNECT;
        INFO(LOG_SCREEN,"\033[0;33;40mSIP loaded to %s mode\033[0m",type_bind_sip_str[p_sip_socket->type_bind]);
        return (error_sip) SIP_ERROR_NO;
    }
    ERROR(LOG_SCREEN | LOG_FILE,"sip ready connect");
    return (error_sip) SIP_ALREADY_CONNECT;
}

/**
*  \brief This function is used for disconnect the sip socket
*
*  \param p_sip_socket   This parameter is the SIP socket structure
*
*/
error_sip sip_end_connection(sip_socket *p_sip_socket){
    if(p_sip_socket && p_sip_socket->status == SIP_CONNECT){
        do_udp_close(p_sip_socket->sock);
        p_sip_socket->status = SIP_DISCONNECT;
	return (error_sip) SIP_ERROR_NO;
    }
    INFO(LOG_SCREEN | LOG_FILE,"sip already disconnect");
    return (error_sip) SIP_IS_NOT_CONNECT;
}

/**
*  \brief This function is used for restart the sip socket
*
*  \param p_sip_socket   This parameter is the SIP socket structure
*
*/
error_sip sip_restart_connection(sip_socket *p_sip_socket){
    sip_end_connection(p_sip_socket);
    return (error_sip) sip_start_connection(p_sip_socket);
}

/**
*  \brief This function allow to send a SIP trame
*
*  \param p_sip_socket   This parameter is the SIP socket structure
*  \param str_in         This parameter is the trame sip message
*  \param ip_addr        This parameter is the IP of the SIP gateway
*  \param port           This parameter is the Port of the SIP gateway
*
*/
error_sip sip_send_message(sip_socket *p_sip_socket, uint8_t *buffer, uint8_t *ip_addr, uint8_t *port){
    if(p_sip_socket &&
       (p_sip_socket->type_bind == SIP_BIND_TRANSMITTER || p_sip_socket->type_bind == SIP_BIND_TRANSCEIVER) && 
       buffer && strlen((char*)buffer) > 0 && 
       ip_addr && strlen((char*)ip_addr) > 0 &&
       atoi(port) >= 0 && atoi(port) < UINT16_MAX ){
            if(udp_send(p_sip_socket->sock, (uint8_t*)buffer, strlen((char*)buffer), ip_addr, (uint16_t)atoi(port)) == 0){
                INFO(LOG_SCREEN, "sip message send");
                return (error_sip) SIP_ERROR_NO;
            }
            ERROR(LOG_SCREEN | LOG_FILE, "sip message don't send");
    }else{
        ERROR(LOG_SCREEN | LOG_FILE, "error in config sip message");
    }
    return (error_sip) SIP_ERROR_SEND;
}

/**
*  \brief This function allow to receive a SIP trame, only if the mode is RECEIVE or TRANSCEIVER.
*/
error_sip sip_receive_message(sip_socket *p_sip_socket, uint8_t **str_out){
    if(p_sip_socket &&
       (p_sip_socket->type_bind == SIP_BIND_RECEIVER || p_sip_socket->type_bind == SIP_BIND_TRANSCEIVER) &&
       str_out && *str_out == NULL
      ){
        *str_out = (uint8_t*)calloc(2049, sizeof(uint8_t));
        if(udp_receive(p_sip_socket->sock, str_out, sizeof(uint8_t)*2048) == 0){
            return (error_sip) SIP_ERROR_NO;
        }
        free(*str_out);
        return (error_sip) SIP_NO_MESSAGE;
    }
    ERROR(LOG_SCREEN | LOG_FILE, "sip message don't receive");
    return (error_sip) SIP_ERROR_RECEIVE;
}

