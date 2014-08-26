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
    SECTION_SIP_DEST     = 0x02,
    SECTION_SIP_LOCAL    = 0x04,
    SECTION_SIP          = 0x06,
    SECTION_SMPP         = 0x08,
    SECTION_DBMS         = 0x10,
    SECTION_ALL          = 0xFF
}section_flags;

typedef struct _ini_main{
    uint8_t *loglevel;
    uint8_t *fork;
} ini_main;

typedef struct _ini_sip_dest{
    uint8_t *sip_dest_ip;
    uint8_t *sip_dest_port;
} ini_sip_dest;

typedef struct _Ini_sip_local{
    uint8_t *sip_local_ip;
    uint8_t *sip_local_port;
} ini_sip_local;

typedef struct _ini_smpp{
    uint8_t *smpp_server_ip;
    uint8_t *smpp_server_port;
    uint8_t *pass_smpp;
    uint8_t *user_smpp;
    int8_t   npi_src; //numeric_plan_indicator
    int8_t   ton_src; //type_of_number
    int8_t   npi_dst; //numeric_plan_indicator
    int8_t   ton_dst; //type_of_number
    uint8_t *system_type;
    uint8_t *service_type;
    int8_t   command_id;
} ini_smpp;

typedef struct _ini_dbms{
    uint8_t *dbms_name;
    uint8_t *db_path;	//SQLite3
    uint8_t *db_basename;	//SQLite3
    uint8_t *db_dirname;	//SQLite3
    uint8_t *db_encoding;	//default : UTF-8
    uint8_t *db_ttl_sms;	//default : 0 (unlimited)
} ini_dbms;

ini_main       main_ini;
ini_sip_dest   sip_dest_ini;
ini_sip_local  sip_local_ini;
ini_smpp       smpp_ini;
ini_dbms       dbms_ini;

bool check_file_ini(section_flags sections);

bool load_file_ini(uint8_t *conffile, section_flags sections);

void free_file_ini(section_flags sections);

#endif /*INI_FILE_H*/

#ifdef __cplusplus
}
#endif

