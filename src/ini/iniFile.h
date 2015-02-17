#ifdef __cplusplus
extern "C"{
#endif

#ifndef INI_FILE_H
#define INI_FILE_H

#include "../log/log.h"
#include "../type_projet.h"

#define SQLITE "sqlite3"

typedef enum _section_flags{
    SECTION_MAIN         = 0x01,
    SECTION_SIP_REMOTE   = 0x02,
    SECTION_SIP_HOST     = 0x04,
    SECTION_SIP          = 0x06,
    SECTION_SMPP         = 0x08,
    SECTION_DBMS         = 0x10,
    SECTION_ALL          = 0xFF
}section_flags;

typedef enum _sock_model{
    MODEL_CLIENT = 0,
    MODEL_SERVER = 1
} sock_model;

typedef struct _ini_main{
    uint8_t *loglevel;
    uint8_t *fork;
} ini_main;

typedef struct _ini_sip{
    uint8_t   *interface_name;
    sock_model model;       //CLIENT/SERVER
    uint8_t   *sip_ip;
    uint32_t   sip_port;
} ini_sip;

//TODO: SERVER MODE NOT IMPLEMENTED
typedef struct _ini_smpp{
    uint8_t   *interface_name;
    sock_model model;       //CLIENT/SERVER
    uint8_t   *smpp_ip;     //Remote is client/Host if server
    uint32_t   smpp_port;   //Remote is client/Host if server
    uint8_t    nb_client;    //number client maximum if server mode
    uint8_t   *pass_smpp;
    uint8_t   *user_smpp;
    int8_t     npi_src;      //numeric_plan_indicator
    int8_t     ton_src;      //type_of_number
    int8_t     npi_dst;      //numeric_plan_indicator
    int8_t     ton_dst;      //type_of_number
    uint8_t   *system_type;
    uint8_t   *service_type;
    int8_t     command_id;
} ini_smpp;

typedef struct _ini_dbms{
    uint8_t *dbms_name; //SQLite3 only
    uint8_t *db_path;	//SQLite3
    uint8_t *db_encoding;	//default : UTF-8
    uint8_t *db_synchronous; //default : normal 
    uint32_t db_heap_limit;  //default : 8290304 Ko 
    uint8_t  db_foreign_keys;//default : on 
    uint16_t db_ttl_sms;
} ini_dbms;

ini_main       main_ini;
ini_sip        sip_remote_ini;
ini_sip        sip_host_ini;
ini_smpp       smpp_ini;//client model
ini_dbms       dbms_ini;

bool check_file_ini(section_flags sections);

bool load_file_ini(uint8_t *conffile, section_flags sections);

void free_file_ini(section_flags sections);

#endif /*INI_FILE_H*/

#ifdef __cplusplus
}
#endif

