#ifdef __cplusplus
extern "C"{
#endif

#ifndef INI_FILE_H
#define INI_FILE_H

#include "../log/log.h"
#include "../type_projet.h"

#define SQLITE "sqlite3"

typedef enum _SectionFlags{
    SECTION_MAIN         = 0x01,
    SECTION_SIP_DEST     = 0x02,
    SECTION_SIP_LOCAL    = 0x04,
    SECTION_SIP          = 0x06,
    SECTION_SMPP         = 0x08,
    SECTION_DBMS         = 0x10,
    SECTION_ALL          = 0xFF
}SectionFlags;

typedef struct _Ini_main{
    char *loglevel;
    char *fork;
}Ini_main;

typedef struct _Ini_sip_dest{
    char *sip_dest_ip;
    char *sip_dest_port;
}Ini_sip_dest;

typedef struct _Ini_sip_local{
    char *sip_local_ip;
    char *sip_local_port;
}Ini_sip_local;

typedef struct _Ini_smpp{
    char *smpp_server_ip;
    char *smpp_server_port;
    char *pass_smpp;
    char *user_smpp;
}Ini_smpp;

typedef struct _Ini_dbms{
    char *dbms_name;
    char *db_path;	//SQLite3
    char *db_basename;	//SQLite3
    char *db_dirname;	//SQLite3
    char *db_encoding;	//default : UTF-8
    char *db_ttl_sms;	//default : 0 (unlimited)
}Ini_dbms;

Ini_main       mainIni;
Ini_sip_dest   sipDestIni;
Ini_sip_local  sipLocalIni;
Ini_smpp       smppConnectIni;
Ini_dbms       dbmsIni;

boolean checkFileIni(SectionFlags sections);

boolean loadFileIni(char *conffile, SectionFlags sections);

void freeFileIni(SectionFlags sections);

#endif /*INI_FILE_H*/

#ifdef __cplusplus
}
#endif

