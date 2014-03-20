#ifndef INI_FILE_H
#define INI_FILE_H

#include "type_projet.h"

typedef struct Ini_main{
    char *sip_dest_ip;
    char *sip_dest_port;
    char *sip_local_ip;
    char *sip_local_port;
    char *smpp_server_ip;
    char *smpp_server_port;
    char *capt_device;
    char *pass_smpp;
    char *user_smpp;
}Ini_main;

static Ini_main mainIni;

Boolean checkConfigIni();

Boolean loadFileIni(char *conffile);

#endif /*INI_FILE*/
