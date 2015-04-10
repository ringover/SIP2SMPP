
#include "struct_display.h"
#include "config.h"

////////////////////
// DISPLAY CONFIG //
////////////////////

inline void display_config_main(config_main_t *main){
    if(main){
        char buffer[2048] = { 0 };
        sprintf(buffer, "[main]\n"
                        STR_LOG_LEVEL"      : %s\n"
                        STR_LAUNCH_MSG"     : %s\n"
                        STR_FORK"           : %d\n"
                        STR_ROUTING_MODULE" : %s\n",
                str_loglevel[(int)main->log_level],
                main->launch_msg,
                main->fork,
                main->routing_module ? main->routing_module : "null");
        DEBUG(LOG_SCREEN, "\n%s", buffer)
    }
    return;
}

inline void display_config_sqlite(config_sqlite_t *sqlite){
    if(sqlite){
        char buffer[2048] = { 0 };
        sprintf(buffer, "[sqlite]\n"
                        STR_PATH"         : %s\n"
                        STR_ENCODING"     : %s\n"
                        STR_SYNCHRONOUS"  : %s\n"
                        STR_HEAP_LIMIT"   : %d\n"
                        STR_FOREIGN_KEYS" : %s\n",
                sqlite->path,
                sqlite->encoding,
                sqlite->synchronous,
                sqlite->heap_limit,
                sqlite->foreign_keys);
        DEBUG(LOG_SCREEN, "\n%s", buffer)
    }
    return;
}
/*
inline void display_config_sigtran(config_sigtran_t *sigtran){
    if(sigtran){
        //TODO
    }
    return;
}
*/
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

void display_config_file(enum_config_load_t flags, const char *name){
    if(flags & CONFIG_MAIN){
        display_config_main(cfg_main);
    }
    if(flags & CONFIG_SQLITE){
        display_config_sqlite(cfg_sqlite);
    }
    if((flags & CONFIG_SMPP) && name){
        config_smpp_t *value = (config_smpp_t*)map_get(cfg_smpp, name);
        display_config_smpp(value);
    }else if(flags & CONFIG_SMPP){
        iterator_map *p_it = cfg_smpp->begin;
        while(p_it){
            display_config_smpp((config_smpp_t*)p_it->value);
            p_it = p_it->next;
        }
    }
    if((flags & CONFIG_SIP) && name){
        config_sip_t *value = (config_sip_t*)map_get(cfg_sip, name);
        display_config_sip(value);
    }else if(flags & CONFIG_SIP){
        iterator_map *p_it = cfg_sip->begin;
        while(p_it){
            display_config_sip((config_sip_t*)p_it->value);
            p_it = p_it->next;
        }
    }
//    if((flags & CONFIG_SIGTRAN) && name){
//        config_client_sigtran_t *value = (config_client_sigtran_t*)map_get(cfg_sigtran, name);
//        display_config_sigtran(value);
//    }else if(flags & CONFIG_SIGTRAN){
//        iterator_map *p_it = cfg_sigtran->begin;
//        while(p_it){
//            display_config_sigtran((config_client_sigtran_t*)p_it->value);
//            p_it = p_it->next;
//        }
//    }
    return;
}


