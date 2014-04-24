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

    if(dbmsIni.dbms_name == NULL) {
	fprintf(stderr,"\"dbms_name\" must be defined in the INI file!\n");
        b = FALSE;
    }else{

	if(strcmp(dbmsIni.dbms_name,"mysql")==0){
		//MySQL
		if(dbmsIni.db_host == NULL) {
        		fprintf(stderr,"\"db_host\" must be defined in the INI file!\n");
		        b = FALSE;
		}

		if(dbmsIni.db_username == NULL) {
			fprintf(stderr,"\"db_username\" must be defined in the INI file!\n");
		        b = FALSE;
		}

		if(dbmsIni.db_password == NULL) {
       		 	fprintf(stderr,"\"db_password\" must be defined in the INI file!\n");
		        b = FALSE;
		}

		if(dbmsIni.db_basename == NULL) {
        		fprintf(stderr,"\"db_basename\" must be defined in the INI file!\n");
		        b = FALSE;
		}

	}else if(strcmp(dbmsIni.dbms_name,"sqlite3")==0){
		//SQLite3
		if(dbmsIni.db_dirname == NULL) {
        		fprintf(stderr,"\"db_dirname\" must be defined in the INI file!\n");
		        b = FALSE;
		}

		if(dbmsIni.db_basename == NULL) {
        		fprintf(stderr,"\"db_basename\" must be defined in the INI file!\n");
		        b = FALSE;
		}

	}else{
		fprintf(stderr,"\"dbms_name\" is wrong, please your choice have to be : mysql or sqlite3\n");
		b = FALSE;
	}
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
    char user_smpp_ini[100]     ="user";
    char pass_smpp_ini[100]     ="pass";

    char dbms_name_ini[100]     = "mysql";
    char db_host_ini[100]       = "127.0.0.1";
    char db_username_ini[100]   = "root";
    char db_password_ini[100]   = "";
    char db_basename_ini[100]   = "sip2smpp";
    char db_dirname_ini[100]    = "/var/sqlite3/sip2smpp";
    char db_encoding_ini[100]   = "UTF-8";
    char db_ttl_sms_ini[100]	= "0";

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

    if(dbmsIni.dbms_name)
	free(dbmsIni.dbms_name);
    dbmsIni.dbms_name        = NULL;

    if(dbmsIni.db_host)
	free(dbmsIni.db_host);
    dbmsIni.db_host          = NULL;

    if(dbmsIni.db_username)
	free(dbmsIni.db_username);
    dbmsIni.db_username      = NULL;

    if(dbmsIni.db_password)
	free(dbmsIni.db_password);
    dbmsIni.db_password     = NULL;

    if(dbmsIni.db_basename) 
	free(dbmsIni.db_basename);
    dbmsIni.db_basename      = NULL;

    if(dbmsIni.db_dirname)
	free(dbmsIni.db_dirname);
    dbmsIni.db_dirname       = NULL;

    if(dbmsIni.db_encoding)
	free(dbmsIni.db_encoding);
    dbmsIni.db_encoding      = NULL;

    if(dbmsIni.db_ttl_sms)
	free(dbmsIni.db_ttl_sms);
    dbmsIni.db_ttl_sms       = NULL;

    ///////

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

    if(dbmsIni.dbms_name == NULL){
	ini_gets("dbms", "dbms_name", "none", dbms_name_ini, sizearray(dbms_name_ini), conffile);
	if(strcmp(dbms_name_ini, "none") != 0) {
		dbmsIni.dbms_name =(char*)malloc(sizeof(char)*(strlen(dbms_name_ini)+1));
		memset(dbmsIni.dbms_name,0,sizeof(char)*(strlen(dbms_name_ini)+1));
		strcpy(dbmsIni.dbms_name,dbms_name_ini);
	}
    }

    if(dbmsIni.db_host == NULL){
	ini_gets("dbms", "db_host", "none", db_host_ini, sizearray(db_host_ini), conffile);
        if(strcmp(db_host_ini, "none") != 0) {
                dbmsIni.db_host =(char*)malloc(sizeof(char)*(strlen(db_host_ini)+1));
                memset(dbmsIni.db_host,0,sizeof(char)*(strlen(db_host_ini)+1));
                strcpy(dbmsIni.db_host,db_host_ini);
        }
    }

    if(dbmsIni.db_username == NULL){
        ini_gets("dbms", "db_username", "none", db_username_ini, sizearray(db_username_ini), conffile);
        if(strcmp(db_username_ini, "none") != 0) {
                dbmsIni.db_username =(char*)malloc(sizeof(char)*(strlen(db_username_ini)+1));
                memset(dbmsIni.db_username,0,sizeof(char)*(strlen(db_username_ini)+1));
                strcpy(dbmsIni.db_username,db_username_ini);
        }
    }

    if(dbmsIni.db_password == NULL){
        ini_gets("dbms", "db_password", "none", db_password_ini, sizearray(db_password_ini), conffile);
        if(strcmp(db_password_ini, "none") != 0) {
                dbmsIni.db_password =(char*)malloc(sizeof(char)*(strlen(db_password_ini)+1));
                memset(dbmsIni.db_password,0,sizeof(char)*(strlen(db_password_ini)+1));
                strcpy(dbmsIni.db_password,db_password_ini);
        }
    }

    if(dbmsIni.db_basename == NULL){
        ini_gets("dbms", "db_basename", "none", db_basename_ini, sizearray(db_basename_ini), conffile);
        if(strcmp(db_basename_ini, "none") != 0) {
                dbmsIni.db_basename =(char*)malloc(sizeof(char)*(strlen(db_basename_ini)+1));
                memset(dbmsIni.db_basename,0,sizeof(char)*(strlen(db_basename_ini)+1));
                strcpy(dbmsIni.db_basename,db_basename_ini);
        }
    }

    if(dbmsIni.db_dirname == NULL){
        ini_gets("dbms", "db_dirname", "none", db_dirname_ini, sizearray(db_dirname_ini), conffile);
        if(strcmp(db_dirname_ini, "none") != 0) {
                dbmsIni.db_dirname =(char*)malloc(sizeof(char)*(strlen(db_dirname_ini)+1));
                memset(dbmsIni.db_dirname,0,sizeof(char)*(strlen(db_dirname_ini)+1));
                strcpy(dbmsIni.db_dirname,db_dirname_ini);
        }
    }

    if(dbmsIni.db_encoding == NULL){
        ini_gets("dbms", "db_encoding", "UTF-8", db_encoding_ini, sizearray(db_encoding_ini), conffile);
        if(strcmp(db_encoding_ini, "none") != 0) {
                dbmsIni.db_encoding =(char*)malloc(sizeof(char)*(strlen(db_encoding_ini)+1));
                memset(dbmsIni.db_encoding,0,sizeof(char)*(strlen(db_encoding_ini)+1));
                strcpy(dbmsIni.db_encoding,db_encoding_ini);
        }
    }

    if(dbmsIni.db_ttl_sms == NULL){
        ini_gets("dbms", "db_ttl_sms", "0", db_ttl_sms_ini, sizearray(db_ttl_sms_ini), conffile);
        if(strcmp(db_ttl_sms_ini, "none") != 0) {
                dbmsIni.db_ttl_sms =(char*)malloc(sizeof(char)*(strlen(db_ttl_sms_ini)+1));
                memset(dbmsIni.db_ttl_sms,0,sizeof(char)*(strlen(db_ttl_sms_ini)+1));
                strcpy(dbmsIni.db_ttl_sms,db_ttl_sms_ini);
        }
    }

    return checkConfigIni();
}
