/**
*   \file iniFile.c
*
*   \brief This part it used for read the INI File.
*
*/

#include "minIni/minIni.c"
#include "iniFile.h"

static void _strcpy(char** str1, char* str2);
static void _saveFileIni(SectionFlags sections);

static Ini_main       _mainIni;
static Ini_sip_dest   _sipDestIni;
static Ini_sip_local  _sipLocalIni;
static Ini_smpp       _smppConnectIni;
static Ini_dbms       _dbmsIni;

void freeFileIni(SectionFlags sections){
  //mainIni
  if((SECTION_MAIN & sections) != 0){
    if(mainIni.loglevel)
        free(mainIni.loglevel);
    mainIni.loglevel  = NULL;
    
    if(mainIni.fork)
        free(mainIni.fork);
    mainIni.fork      = NULL;
  }
  //smppConnectIni
  if((SECTION_SMPP & sections) != 0){
    if(smppConnectIni.pass_smpp)
        free(smppConnectIni.pass_smpp);
    smppConnectIni.pass_smpp        = NULL;

    if(smppConnectIni.user_smpp)
        free(smppConnectIni.user_smpp);
    smppConnectIni.user_smpp        = NULL;

    if(smppConnectIni.smpp_server_ip)
        free(smppConnectIni.smpp_server_ip);
    smppConnectIni.smpp_server_ip   = NULL;

    if(smppConnectIni.smpp_server_port)
        free(smppConnectIni.smpp_server_port);
    smppConnectIni.smpp_server_port = NULL;
  }
  //sipDestIni
  if((SECTION_SIP_DEST & sections) != 0){
    if(sipDestIni.sip_dest_ip)
        free(sipDestIni.sip_dest_ip);
    sipDestIni.sip_dest_ip      = NULL;

    if(sipDestIni.sip_dest_port)
        free(sipDestIni.sip_dest_port);
    sipDestIni.sip_dest_port    = NULL;
  }
  //sipLocalIni
  if((SECTION_SIP_LOCAL & sections) != 0){
    if(sipLocalIni.sip_local_port)
        free(sipLocalIni.sip_local_port);
    sipLocalIni.sip_local_port   = NULL;

    if(sipLocalIni.sip_local_ip)
        free(sipLocalIni.sip_local_ip);
    sipLocalIni.sip_local_ip     = NULL;
  }
  //dbmsIni
  if((SECTION_DBMS & sections) != 0){
    if(dbmsIni.dbms_name)
        free(dbmsIni.dbms_name);
    dbmsIni.dbms_name        = NULL;

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
  }

    return;
}

static void _saveFileIni(SectionFlags sections){
  if((SECTION_MAIN & sections) != 0){
    if(mainIni.loglevel)
        free(mainIni.loglevel);
    mainIni.loglevel   = _mainIni.loglevel;
    _mainIni.loglevel  = NULL;
    
    if(mainIni.fork)
        free(mainIni.fork);
    mainIni.fork       = _mainIni.fork;
    _mainIni.fork      = NULL;
  }
  if((SECTION_SMPP & sections) != 0){
    if(smppConnectIni.pass_smpp)
        free(smppConnectIni.pass_smpp);
    smppConnectIni.pass_smpp         = _smppConnectIni.pass_smpp;
    _smppConnectIni.pass_smpp        = NULL;

    if(smppConnectIni.user_smpp)
        free(smppConnectIni.user_smpp);
    smppConnectIni.user_smpp         = _smppConnectIni.user_smpp;
    _smppConnectIni.user_smpp        = NULL;

    if(smppConnectIni.smpp_server_ip)
        free(smppConnectIni.smpp_server_ip);
    smppConnectIni.smpp_server_ip    = _smppConnectIni.smpp_server_ip;
    _smppConnectIni.smpp_server_ip   = NULL;

    if(smppConnectIni.smpp_server_port)
        free(smppConnectIni.smpp_server_port);
    smppConnectIni.smpp_server_port  = _smppConnectIni.smpp_server_port;
    _smppConnectIni.smpp_server_port = NULL;
  }
  if((SECTION_SIP_DEST & sections) != 0){
    if(sipDestIni.sip_dest_ip)
        free(sipDestIni.sip_dest_ip);
    sipDestIni.sip_dest_ip       = _sipDestIni.sip_dest_ip;
    _sipDestIni.sip_dest_ip      = NULL;

    if(sipDestIni.sip_dest_port)
        free(sipDestIni.sip_dest_port);
    sipDestIni.sip_dest_port     = _sipDestIni.sip_dest_port;
    _sipDestIni.sip_dest_port    = NULL;
  }
  if((SECTION_SIP_LOCAL & sections) != 0){
    if(sipLocalIni.sip_local_port)
        free(sipLocalIni.sip_local_port);
    sipLocalIni.sip_local_port    = _sipLocalIni.sip_local_port;
    _sipLocalIni.sip_local_port   = NULL;

    if(sipLocalIni.sip_local_ip)
        free(sipLocalIni.sip_local_ip);
    sipLocalIni.sip_local_ip      = _sipLocalIni.sip_local_ip;
    _sipLocalIni.sip_local_ip     = NULL;
  }
  if((SECTION_DBMS & sections) != 0){
    if(dbmsIni.dbms_name)
        free(dbmsIni.dbms_name);
    dbmsIni.dbms_name         = _dbmsIni.dbms_name;
    _dbmsIni.dbms_name        = NULL;

    if(dbmsIni.db_basename)
        free(dbmsIni.db_basename);
    dbmsIni.db_basename       = _dbmsIni.db_basename;
    _dbmsIni.db_basename      = NULL;

    if(dbmsIni.db_dirname)
        free(dbmsIni.db_dirname);
    dbmsIni.db_dirname        = _dbmsIni.db_dirname;
    _dbmsIni.db_dirname       = NULL;

    if(dbmsIni.db_encoding)
        free(dbmsIni.db_encoding);
    dbmsIni.db_encoding       = _dbmsIni.db_encoding;
    _dbmsIni.db_encoding      = NULL;

    if(dbmsIni.db_ttl_sms)
        free(dbmsIni.db_ttl_sms);
    dbmsIni.db_ttl_sms        = _dbmsIni.db_ttl_sms;
    _dbmsIni.db_ttl_sms       = NULL;
  }
    INFO(LOG_SCREEN,"The config file is loaded");

    return;
}

/**
*  \brief This function check the configuration of the INI file
*
*  \return Retrun a boolean corresponding to the check
*
*/
boolean checkFileIni(SectionFlags sections){
    boolean b = TRUE;
  
  if((SECTION_MAIN & sections) != 0){
    if(_mainIni.loglevel == NULL){
        ERROR(LOG_SCREEN,"\"loglevel = [0-8]\" must be defined in the INI file!");
	b = FALSE;
    }
    
    if(_mainIni.fork == NULL){
        ERROR(LOG_SCREEN,"\"fork = [0-1]\" must be defined in the INI file!");
	b = FALSE;
    }

  }
  if((SECTION_SMPP & sections) != 0){
    if(_smppConnectIni.pass_smpp == NULL){
        ERROR(LOG_SCREEN,"\"pass_smpp = [password]\" must be defined in the INI file!");
	b = FALSE;
    }

    if(_smppConnectIni.user_smpp == NULL){
        ERROR(LOG_SCREEN,"\"user_smpp = [user_name]\" must be defined in the INI file!");
	b = FALSE;
    }

    if(_smppConnectIni.smpp_server_ip == NULL){
        ERROR(LOG_SCREEN,"\"smpp_server_ip = [xxx.xxx.xxx.xxx]\" must be defined in the INI file!");
	b = FALSE;
    }

    if(_smppConnectIni.smpp_server_port == NULL){
        ERROR(LOG_SCREEN,"\"smpp_server_port = [xxxxx]\" must be defined in the INI file!");
	b = FALSE;
    }
  }
  if((SECTION_SIP_DEST & sections) != 0){
    if(_sipDestIni.sip_dest_ip == NULL){
        ERROR(LOG_SCREEN,"\"sip_dest_ip = [xxx.xxx.xxx.xxx]\" must be defined in the INI file!");
	b = FALSE;
    }

    if(_sipDestIni.sip_dest_port == NULL){
        ERROR(LOG_SCREEN,"\"sip_dest_port = [xxxxx]\" must be defined in the INI file!");
	b = FALSE;
    }
  }
  if((SECTION_SIP_LOCAL & sections) != 0){
    if(_sipLocalIni.sip_local_port == NULL){
        ERROR(LOG_SCREEN,"\"sip_local_port = [xxxxx]\" must be defined in the INI file!");
	b = FALSE;
    }

    if(_sipLocalIni.sip_local_ip == NULL){
        ERROR(LOG_SCREEN,"\"sip_local_ip = [xxx.xxx.xxx.xxx]\" must be defined in the INI file!");
	b = FALSE;
    }
  }
  if((SECTION_DBMS & sections) != 0){
    if(_dbmsIni.dbms_name == NULL){
        ERROR(LOG_SCREEN,"\"dbms_name = [sqlite3]\" must be defined in the INI file!");
	b = FALSE;
    }else if(strcmp(_dbmsIni.dbms_name,SQLITE) == 0){
        if(_dbmsIni.db_basename == NULL){
            ERROR(LOG_SCREEN,"\"db_basename = [base_name]\" must be defined in the INI file!");
	    b = FALSE;
        }

        if(_dbmsIni.db_dirname == NULL){
            ERROR(LOG_SCREEN,"\"db_dirname = [path]\" must be defined in the INI file!");
            b = FALSE;
        }
    }else{ 
        ERROR(LOG_SCREEN,"%s is not supported!",_dbmsIni.dbms_name);
        b = FALSE;
    }
/*
    if(_dbmsIni.db_encoding == NULL){
        ERROR(LOG_SCREEN,"\"db_encoding = [UTF-8]\" must be defined in the INI file!");
	b = FALSE;
    }

    if(_dbmsIni.db_ttl_sms == NULL){
        ERROR(LOG_SCREEN,"\"db_ttl_sms = [int]\" must be defined in the INI file!");
	b = FALSE;
    }
*/
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
boolean loadFileIni(char *conffile, SectionFlags sections){ 
    boolean b = FALSE;
    
    if((SECTION_MAIN & sections) != 0){
      char loglevel_ini[10]   ="0";
      char fork_ini[10]       ="0";

      if(_mainIni.loglevel)
          free(_mainIni.loglevel);
      _mainIni.loglevel  = NULL;
      
      if(_mainIni.fork)
          free(_mainIni.fork);
      _mainIni.fork      = NULL;

      if(_mainIni.loglevel == NULL){
          ini_gets("MAIN", "loglevel", "none", loglevel_ini, sizearray(loglevel_ini), conffile);
          if(strcmp(loglevel_ini, "none") != 0){
              _strcpy(&(_mainIni.loglevel),loglevel_ini);
          }
      }

      if(_mainIni.fork == NULL){
          ini_gets("MAIN", "fork", "none", fork_ini, sizearray(fork_ini), conffile);
          if(strcmp(fork_ini, "none") != 0){
              _strcpy(&(_mainIni.fork),fork_ini);
          }
      }
    }

    if((SECTION_SMPP & sections) != 0){
      char smpp_server_ip_ini[100]   ="127.0.0.1";
      char smpp_server_port_ini[100] ="2775";
      char user_smpp_ini[100]        ="user";
      char pass_smpp_ini[100]        ="pass";

      if(_smppConnectIni.pass_smpp)
          free(_smppConnectIni.pass_smpp);
      _smppConnectIni.pass_smpp        = NULL;

      if(_smppConnectIni.user_smpp)
          free(_smppConnectIni.user_smpp);
      _smppConnectIni.user_smpp        = NULL;

      if(_smppConnectIni.smpp_server_ip)
          free(_smppConnectIni.smpp_server_ip);
      _smppConnectIni.smpp_server_ip   = NULL;

      if(_smppConnectIni.smpp_server_port)
          free(_smppConnectIni.smpp_server_port);
      _smppConnectIni.smpp_server_port = NULL;

      if(_smppConnectIni.smpp_server_ip == NULL) {
          ini_gets("SMPP_CONNECT", "smpp_server_ip", "none", smpp_server_ip_ini, sizearray(smpp_server_ip_ini), conffile);
          if(strcmp(smpp_server_ip_ini, "none") != 0) {
              _strcpy(&(_smppConnectIni.smpp_server_ip),smpp_server_ip_ini);
          }
      }

      if(_smppConnectIni.smpp_server_port == NULL) {
          ini_gets("SMPP_CONNECT", "smpp_server_port", "none", smpp_server_port_ini, sizearray(smpp_server_port_ini), conffile);
          if(strcmp(smpp_server_port_ini, "none") != 0) {
              _strcpy(&(_smppConnectIni.smpp_server_port),smpp_server_port_ini);
          }
      }

      if(_smppConnectIni.user_smpp == NULL) {
          ini_gets("SMPP_CONNECT", "system_id_smpp", "none", user_smpp_ini, sizearray(user_smpp_ini), conffile);
          if(strcmp(user_smpp_ini, "none") != 0) {
              _strcpy(&(_smppConnectIni.user_smpp),user_smpp_ini);
          }
      }

      if(_smppConnectIni.pass_smpp == NULL) {
          ini_gets("SMPP_CONNECT", "pass_smpp", "none", pass_smpp_ini, sizearray(pass_smpp_ini), conffile);
          if(strcmp(pass_smpp_ini, "none") != 0) {
              _strcpy(&(_smppConnectIni.pass_smpp),pass_smpp_ini);
          }
      }
    }

    if((SECTION_SIP_DEST & sections) != 0){
      char sip_dest_port_ini[20]    ="127.0.0.1";
      char sip_dest_ip_ini[20]      ="5060";

      if(_sipDestIni.sip_dest_ip)
          free(_sipDestIni.sip_dest_ip);
      _sipDestIni.sip_dest_ip      = NULL;

      if(_sipDestIni.sip_dest_port)
          free(_sipDestIni.sip_dest_port);
      _sipDestIni.sip_dest_port    = NULL;

      if(_sipDestIni.sip_dest_ip == NULL) {
          ini_gets("SIP_DEST", "sip_dest_ip", "none", sip_dest_ip_ini, sizearray(sip_dest_ip_ini), conffile);
          if(strcmp(sip_dest_ip_ini, "none") != 0){
              _strcpy(&(_sipDestIni.sip_dest_ip),sip_dest_ip_ini);
          }
      }

      if(_sipDestIni.sip_dest_port == NULL) {
          ini_gets("SIP_DEST", "sip_dest_port", "none", sip_dest_port_ini, sizearray(sip_dest_port_ini), conffile);
          if(strcmp(sip_dest_port_ini, "none") != 0) {
              _strcpy(&(_sipDestIni.sip_dest_port),sip_dest_port_ini);
          }
      }
    }

    if((SECTION_SIP_LOCAL & sections) != 0){
      char sip_local_port_ini[20]   ="127.0.0.1";
      char sip_local_ip_ini[20]     ="5080";

      if(_sipLocalIni.sip_local_port)
          free(_sipLocalIni.sip_local_port);
      _sipLocalIni.sip_local_port   = NULL;

      if(_sipLocalIni.sip_local_ip)
          free(_sipLocalIni.sip_local_ip);
      _sipLocalIni.sip_local_ip     = NULL;
    
      if(_sipLocalIni.sip_local_ip == NULL) {
          ini_gets("SIP_LOCAL", "sip_local_ip", "none", sip_local_ip_ini, sizearray(sip_local_ip_ini), conffile);
          if(strcmp(sip_local_ip_ini, "none") != 0){
              _strcpy(&(_sipLocalIni.sip_local_ip),sip_local_ip_ini);
          }
      }

      if(_sipLocalIni.sip_local_port == NULL) {
          ini_gets("SIP_LOCAL", "sip_local_port", "none", sip_local_port_ini, sizearray(sip_local_port_ini), conffile);
          if(strcmp(sip_local_port_ini, "none") != 0) {
              _strcpy(&(_sipLocalIni.sip_local_port),sip_local_port_ini);
          }
      }
    }

    if((SECTION_DBMS & sections) != 0){
      char dbms_name_ini[20]        ="sqlite3";
      char db_basename_ini[50]      ="sip2smpp";
      char db_dirname_ini[200]      ="/var/sqlite3/sip2smpp";
      char db_encoding_ini[10]      ="UTF-8";
      char db_ttl_sms_ini[10]	    ="0";

      if(_dbmsIni.dbms_name)
          free(_dbmsIni.dbms_name);
      _dbmsIni.dbms_name        = NULL;

      if(_dbmsIni.db_basename)
          free(_dbmsIni.db_basename);
      _dbmsIni.db_basename      = NULL;

      if(_dbmsIni.db_dirname)
          free(_dbmsIni.db_dirname);
      _dbmsIni.db_dirname       = NULL;

      if(_dbmsIni.db_encoding)
          free(_dbmsIni.db_encoding);
      _dbmsIni.db_encoding      = NULL;

      if(_dbmsIni.db_ttl_sms)
          free(_dbmsIni.db_ttl_sms);
      _dbmsIni.db_ttl_sms       = NULL;
    
      if(_dbmsIni.dbms_name == NULL){
          ini_gets("DBMS", "dbms_name", "none", dbms_name_ini, sizearray(dbms_name_ini), conffile);
	  if(strcmp(dbms_name_ini, "none") != 0) {
              _strcpy(&(_dbmsIni.dbms_name),dbms_name_ini);
	  }
      }

      if(_dbmsIni.db_basename == NULL){
          ini_gets("DBMS", "db_basename", "none", db_basename_ini, sizearray(db_basename_ini), conffile);
          if(strcmp(db_basename_ini, "none") != 0) {
              _strcpy(&(_dbmsIni.db_basename),db_basename_ini);
          }
      }

      if(_dbmsIni.db_dirname == NULL){
          ini_gets("DBMS", "db_dirname", "none", db_dirname_ini, sizearray(db_dirname_ini), conffile);
          if(strcmp(db_dirname_ini, "none") != 0) {
	      _strcpy(&(_dbmsIni.db_dirname),db_dirname_ini);
          }
      }

      if(_dbmsIni.db_encoding == NULL){
          ini_gets("DBMS", "db_encoding", "UTF-8", db_encoding_ini, sizearray(db_encoding_ini), conffile);
          if(strcmp(db_encoding_ini, "none") != 0) {
	      _strcpy(&(_dbmsIni.db_encoding),db_encoding_ini);
          }
      }

      if(_dbmsIni.db_ttl_sms == NULL){
          ini_gets("DBMS", "db_ttl_sms", "0", db_ttl_sms_ini, sizearray(db_ttl_sms_ini), conffile);
          if(strcmp(db_ttl_sms_ini, "none") != 0) {
	      _strcpy(&(_dbmsIni.db_ttl_sms),db_ttl_sms_ini);
          }
      }
    }

    if((b = checkFileIni(sections)) == TRUE){
       _saveFileIni(sections);
    }

    return b;
}

static void _strcpy(char** str1, char* str2){
	*str1=(char*)malloc(sizeof(char)*(strlen(str2)+1));
	memset(*str1,0,sizeof(char)*(strlen(str2)+1));
	strcpy(*str1,str2);
	return;
}

