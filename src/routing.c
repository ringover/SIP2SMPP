/////////////////////////////
//  SCRIPT ROUTING SAMPLE  //
/////////////////////////////
#include "routing.h"

/////////////////
// Foreward sample
/////

inline int route_parser(char *routing_to, char *to, unsigned int *port){
    if(routing_to && to && port){
        if(strncmp(routing_to, "sip:", 4) == 0){
            char *tmp = NULL;
            routing_to += 4;
            if((tmp = strchr(routing_to, ':')) != NULL){
                *port = atoi(tmp+1);
                strncpy(to, routing_to, tmp-routing_to);
            }else{
                strcpy(to, routing_to);
                *port = 5060;
            }
            return (int) I_SIP;
        }else if(strncmp(routing_to, "smpp:", 5) == 0){
            routing_to += 5;
            strcpy(to, routing_to);
            *port = 0;
            return (int) I_SMPP;
        }
    }
    return (int) -1;
}

int routing_to(char *name, char *to, unsigned int *port){
    if(cfg_smpp){
        iterator_map *p_it = cfg_smpp->begin;
        while(p_it){
            char *key_name = (char*)p_it->key;
            if(key_name && strcmp(key_name, name) == 0){
                config_smpp_t *p_config_smpp = (config_smpp_t*)p_it->value;
                if(p_config_smpp->routing_to){
                    return (int) route_parser(p_config_smpp->routing_to, to, port);
                }
                return (int) -1;
            }
            p_it = p_it->next;
        }
    }
    if(cfg_sip){
        iterator_map *p_it = cfg_sip->begin;
        while(p_it){
            char *key_name = (char*)p_it->key;
            if(key_name && strcmp(key_name, name) == 0){
                config_sip_t *p_config_sip = (config_sip_t*)p_it->value;
                if(p_config_sip->routing_to){
                    return (int) route_parser(p_config_sip->routing_to, to, port);
                }
                return (int) -1;
            }
            p_it = p_it->next;
        }
    }
    return (int) -1;
}

int start_routing(){
    //default SIP
    char *name = (char*)calloc(strlen("sip_out") + 1, sizeof(char));
    config_sip_t *p_config_sip = new_config_sip();
    p_config_sip->name = (char*)calloc(strlen("sip_out") + 1, sizeof(char));
    strcpy(p_config_sip->name, "sip_out");
    strcpy(name, "sip_out");
    map_set(cfg_sip, name, p_config_sip);
    return (int) 0;
}

int routing(const unsigned char *interface_name, const unsigned char *origin_ip, const unsigned int *origin_port, sm_data_t *p_sm){
    char to[16] = { 0 };
    unsigned int port = 0;

    switch(routing_to(interface_name, to, &port)){
        case I_SMPP : 
            return (int) send_sms_to_client_smpp(to, p_sm);
        case I_SIP :
            return (int) send_sms_to_sip("sip_out", p_sm, to, port);
        case I_SIGTRAN :
        default:
            return (int) -1;
    }

    ERROR(LOG_SCREEN | LOG_FILE, "Routing failed");
    return (int) -1;
}

int close_routing(){
    return (int) 0;
}

/// END SCRIPT ROUTING SAMPLE
