/////////////////////////////
//  SCRIPT ROUTING SAMPLE  //
/////////////////////////////
#include "routing.h"

/**
 * Global Variables
 */

typedef int (*p_send_sms_to_smpp)(unsigned char* interface_name, sm_data_t *p_sm);
typedef int (*p_send_sms_to_sip)(unsigned char *interface_name, sm_data_t *p_sm, unsigned char *ip_remote, unsigned int port_remote);

static p_send_sms_to_smpp r_send_sms_to_smpp;
static p_send_sms_to_sip r_send_sms_to_sip;
static map* r_cfg_sip;
static map* r_cfg_smpp;
//map* cfg_sigtran; //TODO


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
    if(r_cfg_smpp){
        iterator_map *p_it = r_cfg_smpp->begin;
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
    if(r_cfg_sip){
        iterator_map *p_it = r_cfg_sip->begin;
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

int default_start_routing(void **functions, void **cfgs){
    if(!functions){
        ERROR(LOG_SCREEN, "functions parameter is empty");
        return (int) -1;
    }
    if(!cfgs){
        ERROR(LOG_SCREEN, "cfgs parameter is empty");
        return (int) -1;
    }
    r_send_sms_to_smpp = (p_send_sms_to_smpp)functions[0]; //send_sms_to_client_smpp
    r_send_sms_to_sip = (p_send_sms_to_sip)functions[1]; //send_sms_to_sip
    r_cfg_smpp = (map*)cfgs[0]; //cfg_smpp
    r_cfg_sip = (map*)cfgs[1]; //cfg_sip

		if(map_get(r_cfg_sip, "sip_out") == NULL){
        ERROR(LOG_SCREEN, "sip_out interface doesn't exist");
				return (int) -1;
		}

    return (int) 0;
}

int default_routing(const unsigned char *interface_name, const unsigned char *origin_ip, const unsigned int *origin_port, sm_data_t *p_sm){
    char to[16] = { 0 };
    unsigned int port = 0;

    switch(routing_to(interface_name, to, &port)){
        case I_SMPP : 
            return (int) r_send_sms_to_smpp(to, p_sm);
        case I_SIP :
            return (int) r_send_sms_to_sip("sip_out", p_sm, to, port);
        case I_SIGTRAN : //TODO
        default:
            return (int) -1;
    }

    ERROR(LOG_SCREEN | LOG_FILE, "Routing failed");
    return (int) -1;
}

int default_close_routing(){
    return (int) 0;
}

/// END SCRIPT ROUTING SAMPLE
