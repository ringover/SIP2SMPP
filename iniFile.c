/**
*   \file iniFile.c
*
*   \brief This part it used for read the INI File.
*
*/

#include "lib/minIni/minIni.c"
#include "iniFile.h"

/**
*  \brief This function check the configuration of the INI file
*
*  \return Retrun a boolean corresponding to the check
*
*/
Boolean checkConfigIni(){
    Boolean b = TRUE;

/*
    if(mainIni.capt_device == NULL) {
        fprintf(stderr,"\"device\" must be defined in the INI file!\n");
        b = FALSE;
    }
*/

    if(mainIni.sip_local_ip == NULL) {
        fprintf(stderr,"\"sip_src_ip\" must be defined in the INI file!\n");
        b = FALSE;
    }

    if(mainIni.sip_local_port == NULL) {
        fprintf(stderr,"\"sip_src_port\" must be defined in the INI file!\n");
        b = FALSE;
    }

    if(mainIni.sip_dest_ip == NULL) {
        fprintf(stderr,"\"sip_dest_ip\" must be defined in the INI file!\n");
        b = FALSE;
    }

    if(mainIni.sip_dest_port == NULL) {
        fprintf(stderr,"\"sip_dest_port\" must be defined in the INI file!\n");
        b = FALSE;
    }

    if(mainIni.smpp_server_ip == NULL) {
        fprintf(stderr,"\"smpp_server_ip\" must be defined in the INI file!\n");
        b = FALSE;
    }

    if(mainIni.smpp_server_port == NULL) {
        fprintf(stderr,"\"smpp_server_port\" must be defined in the INI file!\n");
        b = FALSE;
    }

    if(mainIni.user_smpp == NULL) {
        fprintf(stderr,"\"user_smpp\" must be defined in the INI file!\n");
        b = FALSE;
    }

    if(mainIni.pass_smpp == NULL) {
        fprintf(stderr,"\"pass_smpp\" must be defined in the INI file!\n");
        b = FALSE;
    }

    return b;
}

/**
*  \brief This function load the INI file
*
*  \param conffile This parameter is a path of the INI file
*
*  \return Retrun TRUE if the file is loaded
*
*/
Boolean loadFileIni(char *conffile){
    //Init
    //long n; //use for the ini_gets function
    /*************
    *   [MAIN]   * 
    *************/
    char sip_src_port_ini[100]  ="127.0.0.1";
    char sip_src_ip_ini[100]    ="5080";
    char sip_dest_port_ini[100] ="127.0.0.1";
    char sip_dest_ip_ini[100]   ="5060";

    char smpp_ip_ini[100]       ="127.0.0.1";
    char smpp_port_ini[100]     ="2775";
//    char device_ini[100]        ="eth0";
    char user_smpp_ini[100]     ="user";
    char pass_smpp_ini[100]     ="pass";

    if(mainIni.pass_smpp)
        free(mainIni.pass_smpp);
    mainIni.pass_smpp        = NULL;

    if(mainIni.user_smpp)
        free(mainIni.user_smpp);
    mainIni.user_smpp        = NULL;

    if(mainIni.smpp_server_ip)
        free(mainIni.smpp_server_ip);
    mainIni.smpp_server_ip   = NULL;

    if(mainIni.smpp_server_port)
        free(mainIni.smpp_server_port);
    mainIni.smpp_server_port = NULL;

    if(mainIni.sip_dest_ip)
        free(mainIni.sip_dest_ip);
    mainIni.sip_dest_ip      = NULL;

    if(mainIni.sip_dest_port)
        free(mainIni.sip_dest_port);
    mainIni.sip_dest_port    = NULL;

    if(mainIni.sip_local_port)
        free(mainIni.sip_local_port);
    mainIni.sip_local_port   = NULL;

    if(mainIni.sip_local_ip)
        free(mainIni.sip_local_ip);
    mainIni.sip_local_ip     = NULL;

    /*
    if(mainIni.capt_device)
        free(mainIni.capt_device);
    mainIni.capt_device      = NULL;
    */

    if(mainIni.sip_dest_ip == NULL) {
	//ini-gets return 0 or the size of Buffer (here : sip_dest_ip_ini)
        ini_gets("main", "sip_dest_ip", "none", sip_dest_ip_ini, sizearray(sip_dest_ip_ini), conffile);
        if(strcmp(sip_dest_ip_ini, "none") != 0){
            mainIni.sip_dest_ip=(char*)malloc(sizeof(char)*(strlen(sip_dest_ip_ini)+1));
            memset(mainIni.sip_dest_ip,0,sizeof(char)*(strlen(sip_dest_ip_ini)+1));
            strcpy(mainIni.sip_dest_ip,sip_dest_ip_ini);
        }
    }

    if(mainIni.sip_dest_port == NULL) {
        ini_gets("main", "sip_dest_port", "none", sip_dest_port_ini, sizearray(sip_dest_port_ini), conffile);
        if(strcmp(sip_dest_port_ini, "none") != 0) {
            mainIni.sip_dest_port=(char*)malloc(sizeof(char)*(strlen(sip_dest_port_ini)+1));
            memset(mainIni.sip_dest_port,0,sizeof(char)*(strlen(sip_dest_port_ini)+1));
            strcpy(mainIni.sip_dest_port,sip_dest_port_ini);
        }
    }

    if(mainIni.sip_local_ip == NULL) {
        ini_gets("main", "sip_local_ip", "none", sip_src_ip_ini, sizearray(sip_src_ip_ini), conffile);
        if(strcmp(sip_src_ip_ini, "none") != 0){
            mainIni.sip_local_ip=(char*)malloc(sizeof(char)*(strlen(sip_src_ip_ini)+1));
            memset(mainIni.sip_local_ip,0,sizeof(char)*(strlen(sip_src_ip_ini)+1));
            strcpy(mainIni.sip_local_ip,sip_src_ip_ini);
        }
    }

    if(mainIni.sip_local_port == NULL) {
        ini_gets("main", "sip_local_port", "none", sip_src_port_ini, sizearray(sip_src_port_ini), conffile);
        if(strcmp(sip_src_port_ini, "none") != 0) {
            mainIni.sip_local_port=(char*)malloc(sizeof(char)*(strlen(sip_src_port_ini)+1));
            memset(mainIni.sip_local_port,0,sizeof(char)*(strlen(sip_src_port_ini)+1));
            strcpy(mainIni.sip_local_port,sip_src_port_ini);
        }
    }

    if(mainIni.smpp_server_ip == NULL) {
        ini_gets("main", "smpp_server_ip", "none", smpp_ip_ini, sizearray(smpp_ip_ini), conffile);
        if(strcmp(smpp_ip_ini, "none") != 0) {
            mainIni.smpp_server_ip=(char*)malloc(sizeof(char)*(strlen(smpp_ip_ini)+1));
            memset(mainIni.smpp_server_ip,0,sizeof(char)*(strlen(smpp_ip_ini)+1));
            strcpy(mainIni.smpp_server_ip,smpp_ip_ini);
        }
    }

    if(mainIni.smpp_server_port == NULL) {
        ini_gets("main", "smpp_server_port", "none", smpp_port_ini, sizearray(smpp_port_ini), conffile);
        if(strcmp(smpp_port_ini, "none") != 0) {
            mainIni.smpp_server_port=(char*)malloc(sizeof(char)*(strlen(smpp_port_ini)+1));
            memset(mainIni.smpp_server_port,0,sizeof(char)*(strlen(smpp_port_ini)+1));
            strcpy(mainIni.smpp_server_port,smpp_port_ini);
        }
    }

    if(mainIni.user_smpp == NULL) {
        ini_gets("main", "system_id_smpp", "none", user_smpp_ini, sizearray(user_smpp_ini), conffile);
        if(strcmp(user_smpp_ini, "none") != 0) {
            mainIni.user_smpp=(char*)malloc(sizeof(char)*(strlen(user_smpp_ini)+1));
            memset(mainIni.user_smpp,0,sizeof(char)*(strlen(user_smpp_ini)+1));
            strcpy(mainIni.user_smpp,user_smpp_ini);
        }
    }

    if(mainIni.pass_smpp == NULL) {
        ini_gets("main", "pass_smpp", "none", pass_smpp_ini, sizearray(pass_smpp_ini), conffile);
        if(strcmp(pass_smpp_ini, "none") != 0) {
            mainIni.pass_smpp=(char*)malloc(sizeof(char)*(strlen(pass_smpp_ini)+1));
            memset(mainIni.pass_smpp,0,sizeof(char)*(strlen(pass_smpp_ini)+1));
            strcpy(mainIni.pass_smpp,pass_smpp_ini);
        }
    }

    /*
    if(mainIni.capt_device == NULL) {
        ini_gets("main", "device", "none", device_ini, sizearray(device_ini), conffile);
        if(strcmp(device_ini, "none") != 0) {
            mainIni.capt_device=(char*)malloc(sizeof(char)*(strlen(device_ini)+1));
            memset(mainIni.capt_device,0,sizeof(char)*(strlen(device_ini)+1));
            strcpy(mainIni.capt_device,device_ini);
        }
    }
    */

    return checkConfigIni();
}
