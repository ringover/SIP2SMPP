
#ifndef CONFIG_FILE_H
#define CONFIG_FILE_H

#include <string.h>
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#include "../net/net.h"
#include "../smpp_io.h"
#include "../sip_io.h"
#include "../database.h"

#include "../conv/conv.h"

#include "../log/log.h"
#include "../type_projet.h"
#include "../linked_list/map.h"
#include "../linked_list/keys.h"
#include "../str.h"

#define STR_LIST_DELIMITER  ","

//Title
#define STR_MAIN            "main"
#define STR_SQLITE          "sqlite3"
#define STR_SIP             "sip"
#define STR_SMPP            "smpp"
#define STR_SIGTRAN         "sigtran"
//SIP & SMPP(server/client)
#define STR_NAME            "name"
#define STR_IP              "ip"
#define STR_PORT            "port"
#define STR_ADDRESS_RANGE   "address_range"
#define STR_ROUTING_TO      "routing_to"
//DBMS - SQlite3
#define STR_PATH            "path"
#define STR_ENCODING        "encoding"
#define STR_SYNCHRONOUS     "synchronous"
#define STR_HEAP_LIMIT      "heap_limit"
#define STR_FOREIGN_KEYS    "foreign_keys"
//MAIN
#define STR_LOG_LEVEL       "log_level"
#define STR_FORK            "fork"
#define STR_LAUNCH_MSG      "launch_msg"
#define STR_ROUTING_MODULE  "routing_module"
#define STR_SYSTEM_CHARSET  "system_charset"
//SMPP(client/server)
#define STR_LIST_INTERFACE  "list_interface"
#define STR_BIND            "bind"
#define STR_VERSION         "version"
#define STR_MODEL           "model"
#define STR_SYSTEM_ID       "system_id"
#define STR_PASSWORD        "password"
#define STR_SYSTEM_TYPE     "system_type"
#define STR_SERVICE_TYPE    "service_type"
#define STR_TON             "type_of_number"
#define STR_NPI             "numeric_plan_indicator"
#define STR_CLIENTS         "clients"
#define STR_DEFAULT_CODING  "default_data_coding"
//SIP - SMPP (- SIGTRAN)
#define STR_DATA_CODING     "data_coding"
//TODO : SIGTRAN
//...

#define MAX_DATA_CODING     MAX_TAB_CHARSET

typedef enum _enum_config_load{
    CONFIG_MAIN     = 0x01,
    CONFIG_SQLITE   = 0x02,
    CONFIG_SIP      = 0x04,
    CONFIG_SMPP     = 0x08, 
    CONFIG_SIGTRAN  = 0x10, //TODO
    CONFIG_ALL      = 0xFF
} enum_config_load_t;

//CONFIG Main
typedef struct _config_main{
    char    log_level;
    char    *launch_msg;
    bool    fork;
    char    *system_charset;
    char    *routing_module;
} config_main_t;
#define new_config_main()   (config_main_t*)calloc(1, sizeof(config_main_t))

extern config_main_t  *cfg_main;

inline void destroy_config_main(config_main_t *main);
void free_config_main(void **main);
inline void display_config_main(config_main_t *main);

//LOAD/FREE/DISPLAY config file
int load_config_file(char *path_file, enum_config_load_t flags, const char *name);
void free_config_file(enum_config_load_t flags, const char *name);
void display_config_file(enum_config_load_t flags, const char *name);

#endif /* CONFIG_FILE_H */

