
#include "config.h"
#include "minIni/minIni.c"

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

static inline int _load_config_main(void);
static inline int _load_config_sqlite(void);
//static inline int _load_config_interface_sigtran(const char *name);//TODO
static inline int _load_config_interface_client_smpp(const char *name);
static inline int _load_config_interface_smpp(const char *name);
static inline int _load_config_interface_sip(const char *name);
//static inline int _load_config_sigtran(void);//TODO
static inline int _load_config_smpp(void);
static inline int _load_config_sip(void);

static inline int _check_config_main(void);
static inline int _check_config_sqlite(void);
//static inline int _check_config_interface_sigtran(const char *name);//TODO
static inline int _check_config_interface_client_smpp(const char *name);
static inline int _check_config_interface_smpp(const char *name);
static inline int _check_config_interface_sip(const char *name);

/**
 * CONFIG MAIN
 */

config_main_t  *cfg_main;

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

void free_config_main(void **main){
    destroy_config_main((config_main_t*)*main);
    free(*main);
    *main = NULL;
    return;
}

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

/**
 * LOAD CONFIG
 */

static char *conffile = NULL;

static config_main_t         _main;
static config_sqlite_t       _sqlite;
static config_sip_t          _sip;
static config_client_smpp_t  _c_smpp;
static config_smpp_t         _smpp;
//static config_sigtran_t      _sigtran; //TODO

static inline int _load_config_main(void){
    int  error           = 0;
    char loglevel[15]    = { 0 };
    char launch_msg[256] = { 0 };
    char routing_module[256] = { 0 };

    destroy_config_main(&_main);

    ini_gets(STR_MAIN, STR_LOG_LEVEL, "DEBUG", loglevel, sizearray(loglevel), conffile);
    _main.log_level = str_to_loglevel(loglevel);

    _main.fork = (bool)ini_getbool(STR_MAIN, STR_FORK, false, conffile); 

    ini_gets(STR_MAIN, STR_ROUTING_MODULE, "none", routing_module, sizearray(routing_module), conffile);
    if(strcmp(routing_module, "none") != 0){
        _strcpy(_main.routing_module, routing_module);
    }
    
    ini_gets(STR_MAIN, STR_LAUNCH_MSG, "none", launch_msg, sizearray(launch_msg), conffile);
    if(strcmp(launch_msg, "none") != 0){
        _strcpy(_main.launch_msg, launch_msg);
    }
    
    if((error = _check_config_main()) != -1){
        memcpy(cfg_main, &_main, sizeof(config_main_t));
        memset(&_main, 0, sizeof(config_main_t));
    }else{
        destroy_config_main(&_main);
    }

    return (int) error;
}

static inline int _load_config_sqlite(void){
    int   error            = 0;
    char  path[255]        = { 0 }; //SQLite3
    char  encoding[25]     = { 0 }; //default : UTF-8
    char  synchronous[20]  = { 0 }; //default : normal 
    char  foreign_keys[10] = { 0 }; //default : on

    destroy_config_sqlite(&_sqlite);

    ini_gets(STR_SQLITE, STR_PATH, "/etc/sip2smpp/sm.db", path, sizearray(path), conffile);
    _strcpy(_sqlite.path, path);

    ini_gets(STR_SQLITE, STR_ENCODING, "utf8", encoding, sizearray(encoding), conffile);
    _strcpy(_sqlite.encoding, encoding);

    ini_gets(STR_SQLITE, STR_SYNCHRONOUS, "normal", synchronous, sizearray(synchronous), conffile);
    _strcpy(_sqlite.synchronous, synchronous);

    _sqlite.heap_limit = ini_getl(STR_SQLITE, STR_HEAP_LIMIT, 8290304, conffile);

    ini_gets(STR_SQLITE, STR_FOREIGN_KEYS, "on", foreign_keys, sizearray(foreign_keys), conffile);
    _strcpy(_sqlite.foreign_keys, foreign_keys);


    if((error = _check_config_sqlite()) != -1){
        memcpy(cfg_sqlite, &_sqlite, sizeof(config_sqlite_t));
        memset(&_sqlite, 0, sizeof(config_sqlite_t));
    }else{
        destroy_config_sqlite(&_sqlite);
    }

    return (int) error;
}
/*
static inline int _load_config_interface_sigtran(const char *name){
    //int error = 0;
    //TODO
    //destroy_config_interface_sigtran(...);
    //...
    return (int) _check_config_interface_sigtran(name);
}
*/
static inline int _load_config_interface_client_smpp(const char *name){
    int   error             = 0;
    char  ip[17]            = { 0 };  //Remote is client/Host if server
    char  system_id[17]     = { 0 };  //User ID
    char  password[10]      = { 0 };  //Passwd
    char  address_range[41] = { 0 };  //Routing to
    char  routing_to[25]    = { 0 };  //Routing to

    destroy_config_client_smpp(&_c_smpp);

    _strcpy(_c_smpp.name, name);

    ini_gets(name, STR_IP, "none", ip, sizearray(ip), conffile);
    if(strcmp(ip, "none") != 0){
        _strcpy(_c_smpp.ip, ip);
    }

    _c_smpp.port = (int)ini_getl(name, STR_PORT, 0, conffile);

    ini_gets(name, STR_SYSTEM_ID, "none", system_id, sizearray(system_id), conffile);
    if(strcmp(system_id, "none") != 0){
        _strcpy(_c_smpp.system_id, system_id);
    }

    ini_gets(name, STR_PASSWORD, "none", password, sizearray(password), conffile);
    if(strcmp(password, "none") != 0){
        _strcpy(_c_smpp.password, password);
    }

//    ini_gets(name, STR_NPI, STR_NPI_ISDN_E163_E164, npi, sizearray(npi), conffile);
//    _c_smpp.npi = str_to_npi(npi);
//
//    ini_gets(name, STR_TON, STR_TON_International, ton, sizearray(ton), conffile);
//    _c_smpp.ton = str_to_ton(ton);

    ini_gets(name, STR_ADDRESS_RANGE, "none", address_range, sizearray(address_range), conffile);
    if(strcmp(address_range, "none") != 0){
        _strcpy(_c_smpp.address_range, address_range);
    }

    ini_gets(name, STR_ROUTING_TO, "none", routing_to, sizearray(routing_to), conffile);
    if(strcmp(routing_to, "none") != 0){
        _strcpy(_c_smpp.routing_to, routing_to);
    }

    if((error |= _check_config_interface_client_smpp(name)) != -1){
        char *key = calloc(strlen(name)+1, sizeof(char));
        config_client_smpp_t *value = new_config_client_smpp();
        if(!_smpp.list_c_smpp){
            _smpp.list_c_smpp = new_map(free_string, copy_string, compare_string, free_config_client_smpp, NULL, compare_config_client_smpp);
        }
        strcpy(key, name);
        memcpy(value, &_c_smpp, sizeof(config_client_smpp_t));
        memset(&_c_smpp, 0, sizeof(config_client_smpp_t));
        map_set(_smpp.list_c_smpp, key, value);
    }else{
        destroy_config_client_smpp(&_c_smpp);
    }

    return (int) error;
}

static inline int _load_config_interface_smpp(const char *name){
    int   error             = 0;
    char  model[8]          = { 0 };    //client | server
    char  ip[16]            = { 0 };    //Remote is client/Host if server
    char  system_id[17]     = { 0 };    //User ID
    char  password[10]      = { 0 };    //Passwd
    char  ton[20]           = { 0 };    //type_of_number
    char  npi[20]           = { 0 };    //numeric_plan_indicator
    char  system_type[10]   = { 0 };    //WWW | EMAIL | VMS | OTA ...
    char  command_id[15]    = { 0 };    //bind : transceiver | receiver | transmitter
    //char  service_type[20] = { 0 };
    char  address_range[41]  = { 0 };  //Routing to
    char  routing_to[25]     = { 0 };    //Routing to - Only if client model
    char  list_clients[1024] = { 0 };

    destroy_config_smpp(&_smpp);

    _strcpy(_smpp.name, name);

    ini_gets(name, STR_MODEL, "client", model, sizearray(model), conffile);
    _smpp.model = str_to_sock_model(model);

    ini_gets(name, STR_IP, "none", ip, sizearray(ip), conffile);
    if(strcmp(ip, "none") != 0){
        _strcpy(_smpp.ip, ip);
    }

    _smpp.port = (int)ini_getl(name, STR_PORT, 0, conffile);

    ini_gets(name, STR_SYSTEM_ID, "none", system_id, sizearray(system_id), conffile);
    if(strcmp(system_id, "none") != 0){
        _strcpy(_smpp.system_id, system_id);
    }

    ini_gets(name, STR_PASSWORD, "none", password, sizearray(password), conffile);
    if(strcmp(password, "none") != 0){
        _strcpy(_smpp.password, password);
    }

    ini_gets(name, STR_NPI, STR_NPI_ISDN_E163_E164, npi, sizearray(npi), conffile);
    _smpp.npi = str_to_npi(npi);
//    _smpp.npi_src = str_to_npi(npi);
//    _smpp.npi_dst = _smpp.npi_src;

    ini_gets(name, STR_TON, STR_TON_International, ton, sizearray(ton), conffile);
    _smpp.ton = str_to_ton(ton);
//    _smpp.ton_src = str_to_ton(ton);
//    _smpp.ton_dst = _smpp.ton_src;

    ini_gets(name, STR_SYSTEM_TYPE, "WWW", system_type, sizearray(system_type), conffile);
    _strcpy(_smpp.system_type, system_type);

    ini_gets(name, STR_BIND, STR_BIND_TRANSCEIVER, command_id, sizearray(command_id), conffile);
    _smpp.command_id =  str_to_bind(command_id);

    ini_gets(name, STR_ADDRESS_RANGE, "none", address_range, sizearray(address_range), conffile);
    if(strcmp(address_range, "none") != 0){
        _strcpy(_smpp.address_range, address_range);
    }

    ini_gets(name, STR_ROUTING_TO, "none", routing_to, sizearray(routing_to), conffile);
    if(strcmp(routing_to, "none") != 0){
        _strcpy(_smpp.routing_to, routing_to);
    }

    //List clients SMPP for server model
    ini_gets(name, STR_CLIENTS, "none", list_clients, sizearray(list_clients), conffile);
    if(strcmp(list_clients, "none") != 0){
        int i = 0;
        char **clients = explode(list_clients, STR_LIST_DELIMITER);
        while(*(clients + i)){
            error |= _load_config_interface_client_smpp(*(clients + i++));
        }
        i = 0;
        while(*(clients + i)){
            free(*(clients + i++));
        }
        free(clients);
    }

    if((error |= _check_config_interface_smpp(name)) != -1){
        char *key = calloc(strlen(name)+1, sizeof(char));
        strcpy(key, name);
        config_smpp_t *value = new_config_smpp();
        memcpy(value, &_smpp, sizeof(config_smpp_t));
        memset(&_smpp, 0, sizeof(config_smpp_t));
        map_set(cfg_smpp, key, value);
    }else{
        destroy_config_smpp(&_smpp);
    }

    return (int) error;
}

static inline int _load_config_interface_sip(const char *name){
    int   error           = 0;
    char  ip[16]          = { 0 }; //IP host
    char  routing_to[30]  = { 0 };

    destroy_config_sip(&_sip);

    _strcpy(_sip.name, name);
    
    ini_gets(name, STR_IP, "none", ip, sizearray(ip), conffile);
    if(strcmp(ip, "none") != 0){
        _strcpy(_sip.ip, ip);
    }

    _sip.port = (int)ini_getl(name, STR_PORT, 0, conffile);

    ini_gets(name, STR_ROUTING_TO, "none", routing_to, sizearray(routing_to), conffile);
    if(strcmp(routing_to, "none") != 0){
        _strcpy(_sip.routing_to, routing_to);
    }

    if((error = _check_config_interface_sip(name)) != -1){
        char *key = calloc(strlen(name)+1, sizeof(char));
        strcpy(key, name);
        config_sip_t *value = new_config_sip();
        memcpy(value, &_sip, sizeof(config_sip_t));
        memset(&_sip, 0, sizeof(config_sip_t));
        map_set(cfg_sip, key, value);
    }else{
        destroy_config_sip(&_sip);
    }
 
    return (int) error;
}
/*
static inline int _load_config_sigtran(void){
    //TODO

    char list_interface[1024] = { 0 };
    int error = 0;
    ini_gets(STR_SIGTRAN, STR_LIST_INTERFACE, "none", list_interface, sizearray(list_interface), conffile);
    if(strcmp(list_interface, "none") != 0){
        int i = 0;
        char **interfaces = explode(list_interface, STR_LIST_DELIMITER);
        while(interfaces + i){
            error |= _load_config_interface_sigtran(interfaces + i++);
        }
        i = 0;
        while(interfaces + i){
            free(interfaces + i++);
        }
        free(interfaces);
    }
    return (int) error;
}
*/

static inline int _load_config_smpp(void){
    char list_interface[1024] = { 0 };
    int error = 0;
    ini_gets(STR_SMPP, STR_LIST_INTERFACE, "none", list_interface, sizearray(list_interface), conffile);
    if(strcmp(list_interface, "none") != 0){
        int i = 0;
        char **interfaces = explode(list_interface, STR_LIST_DELIMITER);
        while(*(interfaces + i)){
            error |= _load_config_interface_smpp(*(interfaces + i++));
        }
        i = 0;
        while(*(interfaces + i)){
            free(*(interfaces + i++));
        }
        free(interfaces);
    }
    return (int) error;
}

static inline int _load_config_sip(void){
    char list_interface[1024] = { 0 };
    int error = 0;
    ini_gets(STR_SIP, STR_LIST_INTERFACE, "none", list_interface, sizearray(list_interface), conffile);
    if(strcmp(list_interface, "none") != 0){
        int i = 0;
        char **interfaces = explode(list_interface, STR_LIST_DELIMITER);
        while(*(interfaces + i)){
            error |= _load_config_interface_sip(*(interfaces + i++));
        }
        i = 0;
        while(*(interfaces + i)){
            free(*(interfaces + i++));
        }
        free(interfaces);
    }
    return (int) error;
}

/**
 * CHECK CONFIG
 */

static inline int _check_config_main(void){
    //_main
    //empty
    return (int) 0;
}

static inline int _check_config_sqlite(void){
    //_sqlite
    //empty
    return (int) 0;
}
/*
static inline int _check_config_interface_sigtran(const char *name){
    //TODO
    return (int) 0;
}
*/
static inline int _check_config_interface_client_smpp(const char *name){
    int error = 0;
    //_c_smpp
    if(_c_smpp.ip == NULL){
        ERROR(LOG_SCREEN, "%s : ip missing", name)
        error |= -1;
    }
    if(_c_smpp.port == 0){
        ERROR(LOG_SCREEN, "%s : port missing", name)
        error |= -1;
    }
    if(_c_smpp.system_id == NULL){
        ERROR(LOG_SCREEN, "%s : system_id missing", name)
        error |= -1;
    }
    if(_c_smpp.password == 0){
        ERROR(LOG_SCREEN, "%s : password missing", name)
        error |= -1;
    }
    return (int) error;
}

static inline int _check_config_interface_smpp(const char *name){
    int error = 0;
    //_smpp
    if(_smpp.ip == NULL){
        ERROR(LOG_SCREEN, "%s : ip missing", name)
        error |= -1;
    }
    if(_smpp.port == 0){
        ERROR(LOG_SCREEN, "%s : port missing", name)
        error |= -1;
    }
    if(_smpp.system_id == NULL){
        ERROR(LOG_SCREEN, "%s : system_id missing", name)
        error |= -1;
    }
    if(_smpp.password == 0){
        ERROR(LOG_SCREEN, "%s : password missing", name)
        error |= -1;
    }
    return (int) error;
}

static inline int _check_config_interface_sip(const char *name){
    int error = 0;
    //_sip
    if(_sip.ip == NULL){
        ERROR(LOG_SCREEN, "%s : ip missing", name)
        error |= -1;
    }
    if(_sip.port == 0){
        ERROR(LOG_SCREEN, "%s : port missing", name)
        error |= -1;
    }
    return (int) error;
}


/**
 * LOAD/FREE/DISPLAY config file
 */

int load_config_file(char *path_file, enum_config_load_t flags, const char *name){
    int error = 0;
    //init
    if(path_file){
        conffile = path_file;
    }
    if(!cfg_main){
        cfg_main = new_config_main();
    }
    if(!cfg_sqlite){
        cfg_sqlite = new_config_sqlite();
    }
    if(!cfg_sip){
        cfg_sip = new_map(free_string, copy_string, compare_string, free_config_sip, NULL, compare_config_sip);
    }
    if(!cfg_smpp){
        cfg_smpp = new_map(free_string, copy_string, compare_string, free_config_smpp, NULL, compare_config_smpp);
    }
    if(path_file){
        if(flags & CONFIG_MAIN){
            error |= _load_config_main();
        }
        if(flags & CONFIG_SQLITE){
            error |= _load_config_sqlite();
        }
        if((flags & CONFIG_SMPP) && name){
            error |= _load_config_interface_smpp(name);
        }else if(flags & CONFIG_SMPP){
            error |= _load_config_smpp();
        }
        if((flags & CONFIG_SIP) && name){
            error |= _load_config_interface_sip(name);
        }else if(flags & CONFIG_SIP){
            error |= _load_config_sip();
        }
//        if((flags & CONFIG_SIGTRAN) && name){
//            error |= _load_config_interface_sigtran(name);
//        }else if(flags & CONFIG_SIGTRAN){
//            error |= _load_config_sigtran();
//        }
    }
    return (int) error;
}

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
