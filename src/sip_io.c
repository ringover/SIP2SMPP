/**
* In this file, all SIP function are regrouped.
*/
#include "sip_io.h"

map *map_str_sip  = NULL;

/**
*  \brief This function create a sip_socket structure
*/
sip_socket* new_sip_socket(unsigned char *interface_name, unsigned char *ip_host, unsigned int port_host){
    sip_socket *p_sip_socket = NULL;
    p_sip_socket = (sip_socket*)calloc(1, sizeof(sip_socket));

    p_sip_socket->interface_name = calloc(strlen((char*)interface_name)+1, sizeof(uint8_t));
    strcpy((char*)p_sip_socket->interface_name, (char*)interface_name);

    p_sip_socket->sock = (socket_t*)calloc(1, sizeof(socket_t));

    p_sip_socket->ip_host = calloc(strlen((char*)ip_host)+1, sizeof(uint8_t));
    strcpy((char*)p_sip_socket->ip_host, (char*)ip_host);

    p_sip_socket->port_host = port_host;

    p_sip_socket->status = SIP_DISCONNECT;

    return (sip_socket*)p_sip_socket;
}

void sip_socket_free(sip_socket **p_sip_socket){
    if(p_sip_socket && *p_sip_socket){
        if((*p_sip_socket)->ip_host){
            free((*p_sip_socket)->ip_host);
        }
        if((*p_sip_socket)->sock){
            //TODO : close + free
        }
        free(*p_sip_socket);
        *p_sip_socket = NULL;
    }
    return;
}

/**
 * Used for MAP and LIST
 */

void free_sip_socket(void **p_p_data){
    //sip_socket_free((sip_socket**)p_p_data);
    return;
}

void* copy_sip_socket(const void *p_data){
    /* Not used*/ 
    return NULL;
}

int compare_sip_socket(const void *p_data1, const void *p_data2){
    int res = 0;
    sip_socket *p_sip_sock1 = p_data1;
    sip_socket *p_sip_sock2 = p_data2;
    if(strcmp((char*)p_sip_sock1->interface_name, (char*)p_sip_sock2->interface_name) != 0){
        res = -1;
    }
/*
    if(p_sip_sock1->status != p_sip_sock2->status){
        res = -1
    }
    if(strcmp((char*)p_sip_sock1->ip_host, (char*) p_sip_sock2->ip_host) != 0){
        res = -1
    }
    if(p_sip_sock1->port != p_sip_sock2->port){
        res = -1
    }
    if(p_sip_sock1-> == p_sip_sock2->){
        res = -1
    }
*/
    return (int) res;
}

/**
*  \brief This function is used for connect the sip socket
*/
int sip_start_connection(sip_socket *p_sip_socket){
    if(p_sip_socket != NULL && p_sip_socket->status == SIP_DISCONNECT){
        int ret = udp_socket(p_sip_socket->sock, p_sip_socket->ip_host, p_sip_socket->port_host);
        p_sip_socket->status = SIP_CONNECT;
        return (int) ret;
    }
    return (int) -1;
}

/**
*  \brief This function is used for disconnect the sip socket
*/
int sip_end_connection(sip_socket *p_sip_socket){
    if(p_sip_socket && p_sip_socket->status == SIP_CONNECT){
        int ret = udp_close(p_sip_socket->sock);
        p_sip_socket->status = SIP_DISCONNECT;
        return (int) ret;
    }
    return (int) -1;
}

/**
*  \brief This function is used for restart the sip socket
*/
int sip_restart_connection(sip_socket *p_sip_socket){
    sip_end_connection(p_sip_socket);
    return (int) sip_start_connection(p_sip_socket);
}

/**
*  \brief This function allow to send a SIP trame
*/
int sip_send_message(sip_socket *p_sip_socket, unsigned char *buffer, unsigned char *ip_remote, unsigned int port_remote){
    if(p_sip_socket && p_sip_socket->sock){
        return do_udp_send(p_sip_socket->sock, buffer, (size_t)strlen((char*)buffer), ip_remote, port_remote);
    }
    return (int) -1;
}

/**
*  \brief This function allow to receive a SIP trame.
*/
int sip_receive_message(sip_socket *p_sip_socket, unsigned char **buffer, unsigned int *buffer_len, unsigned char **ip_remote, unsigned int *port_remote){
    if(p_sip_socket && buffer){
        int ret = 0;
        struct sockaddr_in from = { 0 };
        ret = do_udp_receive(p_sip_socket->sock, buffer, buffer_len, &from);
        *port_remote = ntohs(from.sin_port);
        //ip_remote = (unsigned char*)calloc(20, sizeof(char));
        *ip_remote = inet_ntoa(from.sin_addr);
        return (int) ret;
    }
    return (int) -1;
}

/**
 * Get message on SIP Message
 */
int get_message(unsigned char *buffer_in, unsigned char **buffer_out){
    char *p_str = buffer_in;
    char *s_str = NULL;
    while((s_str = strstr(p_str, "\r\n")) != NULL){
        p_str = (s_str + strlen((char*)"\r\n"));
    }
    if(p_str){
        int len = strlen((char*)p_str);
        *buffer_out = (unsigned char*)calloc(len, sizeof(char));
        strncpy((char*)*buffer_out, (char*)p_str, len);
        return (int) len;
    }
    return (int) -1;
} 


/**
 * Send/Receive SMS
 */
int sip_receive_sms(sip_socket *p_sip_socket, unsigned char **from_msisdn, unsigned char **to_msisdn, unsigned char **msg){
    if(p_sip_socket){
        unsigned char *buffer     = NULL;
        unsigned int  buffer_len  = 0;
        unsigned int  port_remote = 0;
        unsigned char *ip_remote  = NULL;
        osip_message_t *message   = NULL;
        osip_call_id_t *call_id   = NULL;
        //osip_t *osip = NULL;
        int ret = 0;
        //osip_init(&osip);

        if((ret = sip_receive_message(p_sip_socket, &buffer, &buffer_len, &ip_remote, &port_remote)) <= 0){
            ERROR(LOG_SCREEN, "SIP receive failed");
            return (int) -1;
        }
        
        if(osip_message_init(&message) != 0){
            ERROR(LOG_FILE | LOG_SCREEN, "Cannot init parse osip")
            return (int) -1;
        }
        
        if(osip_message_parse(message, buffer, strlen(buffer)) != 0){
            osip_message_free(message);
            ERROR(LOG_FILE | LOG_SCREEN, "Cannot parse message sip")
            return (int) -1;
        }
        
        if(MSG_IS_MESSAGE(message)){
            if(message->from && message->from->url){
                from_msisdn = (unsigned char*)calloc(strlen(message->from->url->username), sizeof(char));
                strcpy((char*)from_msisdn, (char*)message->from->url->username);
            }
            if(message->from && message->from->url){
                to_msisdn = (unsigned char*)calloc(strlen(message->to->url->username), sizeof(char));
                strcpy((char*)to_msisdn, (char*)message->to->url->username);
            }
            if(get_message(buffer, msg) <= 0){
                WARNING(LOG_SCREEN, "Message empty");
            }
            //TODO: send 202 accepted
            char buffer[2048] = "";
            if(create_trame_sip_202_accepted(&buffer, p_sip_socket->ip_host, p_sip_socket->port_host, ip_remote, port_remote, from_msisdn, to_msisdn) == 0){
                sip_send_message(p_sip_socket, buffer, ip_remote, port_remote);
            }
        }else if(MSG_TEST_CODE(message,202) || MSG_TEST_CODE(message,200)){
            //TODO : clean DB ?  
            //call_id = message->call_id;
            WARNING(LOG_SCREEN, "TODO sip method %s", message->sip_method);
        }else{
            WARNING(LOG_SCREEN, "%s not implemented", message->sip_method);
        }
        
        if(buffer){
            free(buffer);
        }
        
        osip_message_free(message);

        return (int) 0;
    }
    return (int) -1;
}

int send_sms_to_sip(unsigned char *interface_name, unsigned char *from_msisdn, unsigned char *to_msisdn, unsigned char *message, unsigned char *ip_remote, unsigned int port_remote){
    char *buffer = NULL;
    sip_socket *p_sock = map_get(map_str_sip, interface_name);
    if(create_trame_sip_message(buffer, p_sock->ip_host, p_sock->port_host, ip_remote, port_remote, from_msisdn, to_msisdn, message) == 0){
        return sip_send_message(p_sock->sock, buffer, ip_remote, port_remote);
    }
    if(buffer){
        free(buffer);
    }
    ERROR(LOG_SCREEN | LOG_FILE, "Create trame sip message is failed")
    return (int) -1;
}

int send_sms_to_sip_interface(unsigned char *interface_name_src, unsigned char *from_msisdn, unsigned char *to_msisdn, unsigned char *message, unsigned char *interface_name_dst){
    return (int) 0;
}

/**
 * TODO: Voir pour la gestion des messages SIP (200, 202, 500, ...)

int sip_send_200(sip_socket *p_sip_socket, unsigned char **from_msisdn, unsigned char **to_msisdn, unsigned char **message, unsigned char *ip_remote, unsigned char *port_remote){
    return (int) 0;
}

 */
