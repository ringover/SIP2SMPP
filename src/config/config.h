
#ifndef CONFIG_FILE_H
#define CONFIG_FILE_H

#include <string.h>
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#include "struct.h"
#include "minIni/minIni.h"
#include "../log/log.h"
#include "../type_projet.h"
#include "../linked_list/map.h"
#include "../linked_list/keys.h"
#include "../str.h"


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
//SMPP(client/server)
#define STR_LIST_INTERFACE  "list_interface"
#define STR_BIND            "bind"
#define STR_VERSION         "version"
#define STR_MODEL           "model"
#define STR_SYSTEM_ID       "system_id"
#define STR_PASSWORD        "password"
#define STR_SYSTEM_TYPE     "system_type"
#define STR_TON             "type_of_number"
#define STR_NPI             "numeric_plan_indicator"
#define STR_CLIENTS         "clients"
//TODO : SIGTRAN
//...

extern config_main_t       *cfg_main;
extern config_sqlite_t     *cfg_sqlite;
extern map                 *cfg_sip;  // <str, config_sip_t>
extern map                 *cfg_smpp; // <str, config_smpp_t>

int load_config_file(char *path_file, enum_config_load_t flags, const char *name);

#endif /* CONFIG_FILE_H */

