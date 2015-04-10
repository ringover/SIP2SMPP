#include "struct.h"
#include "config.h"

/////////////////
// STR TO ENUM //
/////////////////

inline int str_to_sock_model(const char *str){
    if(str){
        char *tmp = NULL;
        strupper(&tmp, str);
        if(strcmp(tmp, STR_MODEL_CLIENT) == 0){
            return (int) MODEL_CLIENT;
        }
        if(strcmp(tmp, STR_MODEL_SERVER) == 0){
            return (int) MODEL_SERVER;
        }
    }
    return (int) -1;
}

char *str_model[] = {
    STR_MODEL_CLIENT,
    STR_MODEL_SERVER
};

inline const char* bind_to_str(int i){
    if(i == BIND_TRANSCEIVER){
        return (const char*) STR_BIND_TRANSCEIVER;
    }
    if(i == BIND_RECEIVER){
        return (const char*) STR_BIND_RECEIVER;
    }
    if(i == BIND_TRANSMITTER){
        return (const char*) STR_BIND_TRANSMITTER;
    }
    return (const char*) NULL;
}

inline int str_to_bind(const char *str){
    if(str){
        char *tmp = NULL;
        strupper(&tmp, str);
        if(strcmp(tmp, STR_BIND_TRANSCEIVER) == 0){
            return (int) BIND_TRANSCEIVER;
        }
        if(strcmp(tmp, STR_BIND_RECEIVER) == 0){
            return (int) BIND_RECEIVER;
        }
        if(strcmp(tmp, STR_BIND_TRANSMITTER) == 0){
            return (int) BIND_TRANSMITTER;
        }
    }
    return (int) -1;
}

inline const char* npi_to_str(int i){
    if(i == NPI_Unknown){
        return (const char*) STR_NPI_Unknown;
    }
    if(i == NPI_ISDN_E163_E164){
        return (const char*) STR_NPI_ISDN_E163_E164;
    }
    if(i == NPI_Data_X121){
        return (const char*) STR_NPI_Data_X121;
    }
    if(i == NPI_Telex_F69){
        return (const char*) STR_NPI_Telex_F69;
    }
    if(i == NPI_Land_Mobile_E212){
        return (const char*) STR_NPI_Land_Mobile_E212;
    }
    if(i == NPI_National){
        return (const char*) STR_NPI_National;
    }
    if(i == NPI_Private){
        return (const char*) STR_NPI_Private;
    }
    if(i == NPI_ERMES){
        return (const char*) STR_NPI_ERMES;
    }
    if(i == NPI_Internet_IP){
        return (const char*) STR_NPI_Internet_IP;
    }
    if(i == NPI_WAP_Client_Id){
        return (const char*) STR_NPI_WAP_Client_Id;
    }
    return (const char*) NULL;
}

inline int str_to_npi(const char *str){
    if(str){
        char *tmp = NULL;
        strupper(&tmp, str);
        if(strcmp(tmp, STR_NPI_Unknown) == 0){
            return (int) NPI_Unknown;
        }
        if(strcmp(tmp, STR_NPI_ISDN_E163_E164) == 0){
            return (int) NPI_ISDN_E163_E164;
        }
        if(strcmp(tmp, STR_NPI_Data_X121) == 0){
            return (int) NPI_Data_X121;
        }
        if(strcmp(tmp, STR_NPI_Telex_F69) == 0){
            return (int) NPI_Telex_F69;
        }
        if(strcmp(tmp, STR_NPI_Land_Mobile_E212) == 0){
            return (int) NPI_Land_Mobile_E212;
        }
        if(strcmp(tmp, STR_NPI_National) == 0){
            return (int) NPI_National;
        }
        if(strcmp(tmp, STR_NPI_Private) == 0){
            return (int) NPI_Private;
        }
        if(strcmp(tmp, STR_NPI_ERMES) == 0){
            return (int) NPI_ERMES;
        }
        if(strcmp(tmp, STR_NPI_Internet_IP) == 0){
            return (int) NPI_Internet_IP;
        }
        if(strcmp(tmp, STR_NPI_WAP_Client_Id) == 0){
            return (int) NPI_WAP_Client_Id;
        }
    }
    return (int) -1;
}

inline const char* ton_to_str(int i){
    if(i == TON_Unknown){
        return (const char*) STR_TON_Unknown;
    }
    if(i == TON_International){
        return (const char*) STR_TON_International;
    }
    if(i == TON_National){
        return (const char*) STR_TON_National;
    }
    if(i == TON_Network_Specific){
        return (const char*) STR_TON_Network_Specific;
    }
    if(i == TON_Subscriber_Number){
        return (const char*) STR_TON_Subscriber_Number;
    }
    if(i == TON_Alphanumeric){
        return (const char*) STR_TON_Alphanumeric;
    }
    if(i == TON_Abbreviated){
        return (const char*) STR_TON_Abbreviated;
    }
    return (const char*) NULL;
}

inline int str_to_ton(const char *str){
    if(str){
        char *tmp = NULL;
        strupper(&tmp, str);
        if(strcmp(tmp, STR_TON_Unknown) == 0){
            return (int) TON_Unknown;
        }
        if(strcmp(tmp, STR_TON_International) == 0){
            return (int) TON_International;
        }
        if(strcmp(tmp, STR_TON_National) == 0){
            return (int) TON_National;
        }
        if(strcmp(tmp, STR_TON_Network_Specific) == 0){
            return (int) TON_Network_Specific;
        }
        if(strcmp(tmp, STR_TON_Subscriber_Number) == 0){
            return (int) TON_Subscriber_Number;
        }
        if(strcmp(tmp, STR_TON_Alphanumeric) == 0){
            return (int) TON_Alphanumeric;
        }
        if(strcmp(tmp, STR_TON_Abbreviated) == 0){
            return (int) TON_Abbreviated;
        }
    }
    return (int) -1;
}

/////////////////
//   Destroy   //
/////////////////

inline void destroy_config_main(config_main_t *main){
    //main->log_level = 0;
    //main->loglevel = NULL;
    //main->fork = false;
    if(main->launch_msg)
        free(main->launch_msg);
    //main->launch_msg = NULL;
    if(main->routing_module)
        free(main->routing_module);
    //main->routing_module = NULL;
    memset(main, 0, sizeof(config_main_t));
    return;
}

inline void destroy_config_sip(config_sip_t *sip){
    if(sip->name)
        free(sip->name);
    //sip->name = NULL;
    if(sip->ip)
        free(sip->ip);
    //sip->ip = NULL;
    if(sip->routing_to)
        free(sip->routing_to);
    //sip->routing_to = NULL;
    //sip->port = 0;
    memset(sip, 0, sizeof(config_sip_t));
    return;
}

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
/*
inline void destroy_config_sigtran(config_sigtran_t *sigtran){
    return;
}
*/
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

inline void destroy_config_sqlite(config_sqlite_t *sqlite){
    if(sqlite->path)
        free(sqlite->path);
    //sqlite->path = NULL;
    if(sqlite->encoding)
        free(sqlite->encoding);
    //sqlite->encoding = NULL;
    if(sqlite->synchronous)
        free(sqlite->synchronous);
    //sqlite->synchronous = NULL;
    //sqlite->heap_limit = 0;
    if(sqlite->foreign_keys)
        free(sqlite->foreign_keys);
    //sqlite->foreign_keys = NULL;
    memset(sqlite, 0, sizeof(config_sqlite_t));
    return;
}

//////////
// FREE //
//////////

inline void free_config_main(void **main){
    destroy_config_main((config_main_t*)*main);
    free(*main);
    *main = NULL;
    return;
}

inline void free_config_sqlite(void **sqlite){
    destroy_config_sqlite((config_sqlite_t*)*sqlite);
    free(*sqlite);
    *sqlite = NULL;
    return;
}
/*
inline void free_config_sigtran(void **sigtran){
    //TODO
    //destroy_config_sigtran(*sigtran);
    free(*sigtran);
    *sigtran = NULL;
    return;
}
*/
inline void free_config_client_smpp(void **c_smpp){
    destroy_config_client_smpp((config_client_smpp_t*)*c_smpp);
    free(*c_smpp);
    *c_smpp = NULL;
    return;
}

inline void free_config_smpp(void **smpp){
    destroy_config_smpp((config_smpp_t*)*smpp);
    free(*smpp);
    *smpp = NULL;
    return;
}

inline void free_config_sip(void **sip){
    destroy_config_sip((config_sip_t*)*sip);
    free(*sip);
    *sip = NULL;
    return;
}

///////////////

void free_config_file(enum_config_load_t flags, const char *name){
    if(flags & CONFIG_MAIN){
        free_config_main((void**)&cfg_main);
    }
    if(flags & CONFIG_SQLITE){
        free_config_sqlite((void**)&cfg_sqlite);
    }
    if((flags & CONFIG_SMPP) && name){
        map_erase(cfg_smpp, (void*)name);
    }else if(flags & CONFIG_SMPP){
        map_destroy(&cfg_smpp);
    }
    if((flags & CONFIG_SIP) && name){
        map_erase(cfg_sip, (void*)name);
    }else if(flags & CONFIG_SIP){
        map_destroy(&cfg_sip);
    }
//    if((flags & CONFIG_SIGTRAN) && name){
//        map_erase(cfg_sigtran, (void*)name);
//    }else if(flags & CONFIG_SIGTRAN){
//        map_destroy(&cfg_sigtran);
//    }
    return;
}

/////////////
// COMPARE //
////////////
/*
inline int compare_config_sigtran(const void *sigtran1, void *sigtran2){//TODO
    //config_sigtran_t *sig1 = (config_sigtran_t*)sigtran1;
    //config_sigtran_t *sig2 = (config_sigtran_t*)sigtran2;
    return (int) 0;
}
*/
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

int compare_config_sip(const void *sip1, const void *sip2){
    config_sip_t *s1 = (config_sip_t*)sip1;
    config_sip_t *s2 = (config_sip_t*)sip2;
    return (int) strcmp(s1->name, s2->name);
}





