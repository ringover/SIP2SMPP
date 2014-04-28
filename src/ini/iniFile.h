#ifdef __cplusplus
extern "C"{
#endif

#ifndef INI_FILE_H
#define INI_FILE_H

#include "../type_projet.h"

typedef struct _Ini_main{
    char *sip_dest_ip;
    char *sip_dest_port;
    char *sip_local_ip;
    char *sip_local_port;
    char *smpp_server_ip;
    char *smpp_server_port;
    char *pass_smpp;
    char *user_smpp;
}Ini_main;

typedef struct _Ini_dbms{
    char *dbms_name;	//mysql OR sqlite3
    char *db_host;	//MySQL
    char *db_username;	//MySQL
    char *db_password;	//MySQL
    char *db_basename;	//MySQL and SQLite3
    char *db_dirname;	//SQLite3
    char *db_encoding;	//default : UTF-8
    char *db_ttl_sms;	//default : 0 (unlimited)
}Ini_dbms;

Ini_main mainIni;

Ini_dbms dbmsIni;

Boolean checkConfigIni();

Boolean loadFileIni(char *conffile);

#endif /*INI_FILE_H*/

#ifdef __cplusplus
}
#endif

