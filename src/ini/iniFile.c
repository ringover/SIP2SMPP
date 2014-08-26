/**
*   \file iniFile.c
*
*   \brief This part it used for read the INI File.
*
*/

#include "minIni/minIni.c"
#include "iniFile.h"

static void _strcpy(uint8_t **str_out, uint8_t *str_in);
static void _save_file_ini(section_flags sections);

static ini_main       _main_ini;
static ini_sip_dest   _sip_dest_ini;
static ini_sip_local  _sip_local_ini;
static ini_smpp       _smpp_ini;
static ini_dbms       _dbms_ini;

void free_file_ini(section_flags sections){
  //main_ini
  if((SECTION_MAIN & sections) != 0){
    if(main_ini.loglevel)
        free(main_ini.loglevel);
    main_ini.loglevel  = NULL;
    
    if(main_ini.fork)
        free(main_ini.fork);
    main_ini.fork      = NULL;
  }
  //smpp_ini
  if((SECTION_SMPP & sections) != 0){
    if(smpp_ini.pass_smpp)
        free(smpp_ini.pass_smpp);
    smpp_ini.pass_smpp        = NULL;

    if(smpp_ini.user_smpp)
        free(smpp_ini.user_smpp);
    smpp_ini.user_smpp        = NULL;

    if(smpp_ini.smpp_server_ip)
        free(smpp_ini.smpp_server_ip);
    smpp_ini.smpp_server_ip   = NULL;

    if(smpp_ini.smpp_server_port)
        free(smpp_ini.smpp_server_port);
    smpp_ini.smpp_server_port = NULL;

    smpp_ini.npi_src          = -1;

    smpp_ini.ton_src          = -1;

    smpp_ini.npi_dst          = -1;

    smpp_ini.ton_dst          = -1;

    if(smpp_ini.system_type)
        free(smpp_ini.system_type);
    smpp_ini.system_type      = NULL;

    if(smpp_ini.service_type)
        free(smpp_ini.service_type);
    smpp_ini.service_type     = NULL;

    smpp_ini.command_id = -1;

  }
  //sip_dest_ini
  if((SECTION_SIP_DEST & sections) != 0){
    if(sip_dest_ini.sip_dest_ip)
        free(sip_dest_ini.sip_dest_ip);
    sip_dest_ini.sip_dest_ip      = NULL;

    if(sip_dest_ini.sip_dest_port)
        free(sip_dest_ini.sip_dest_port);
    sip_dest_ini.sip_dest_port    = NULL;
  }
  //sip_local_ini
  if((SECTION_SIP_LOCAL & sections) != 0){
    if(sip_local_ini.sip_local_port)
        free(sip_local_ini.sip_local_port);
    sip_local_ini.sip_local_port   = NULL;

    if(sip_local_ini.sip_local_ip)
        free(sip_local_ini.sip_local_ip);
    sip_local_ini.sip_local_ip     = NULL;
  }
  //dbms_ini
  if((SECTION_DBMS & sections) != 0){
    if(dbms_ini.dbms_name)
        free(dbms_ini.dbms_name);
    dbms_ini.dbms_name        = NULL;

    if(dbms_ini.db_path)
        free(dbms_ini.db_path);
    dbms_ini.db_path      = NULL;

    if(dbms_ini.db_basename)
        free(dbms_ini.db_basename);
    dbms_ini.db_basename      = NULL;

    if(dbms_ini.db_dirname)
        free(dbms_ini.db_dirname);
    dbms_ini.db_dirname       = NULL;

    if(dbms_ini.db_encoding)
        free(dbms_ini.db_encoding);
    dbms_ini.db_encoding      = NULL;

    if(dbms_ini.db_ttl_sms)
        free(dbms_ini.db_ttl_sms);
    dbms_ini.db_ttl_sms       = NULL;
  }

    return;
}

static void _save_file_ini(section_flags sections){
  free_file_ini(sections);
  if((SECTION_MAIN & sections) != 0){
    main_ini.loglevel   = _main_ini.loglevel;
    _main_ini.loglevel  = NULL;
    
    main_ini.fork       = _main_ini.fork;
    _main_ini.fork      = NULL;
  }
  if((SECTION_SMPP & sections) != 0){
    smpp_ini.pass_smpp          = _smpp_ini.pass_smpp;
    _smpp_ini.pass_smpp         = NULL;

    smpp_ini.user_smpp          = _smpp_ini.user_smpp;
    _smpp_ini.user_smpp         = NULL;

    smpp_ini.smpp_server_ip     = _smpp_ini.smpp_server_ip;
    _smpp_ini.smpp_server_ip    = NULL;

    smpp_ini.smpp_server_port   = _smpp_ini.smpp_server_port;
    _smpp_ini.smpp_server_port  = NULL;

    smpp_ini.npi_src            = _smpp_ini.npi_src;
    _smpp_ini.npi_src           = -1;

    smpp_ini.ton_src            = _smpp_ini.ton_src;
    _smpp_ini.ton_src           = -1;

    smpp_ini.npi_dst            = _smpp_ini.npi_dst;
    _smpp_ini.npi_dst           = -1;

    smpp_ini.ton_dst            = _smpp_ini.ton_dst;
    _smpp_ini.ton_dst           = -1;

    smpp_ini.system_type        = _smpp_ini.system_type;
    _smpp_ini.system_type       = NULL;

    smpp_ini.service_type       = _smpp_ini.service_type;
    _smpp_ini.service_type      = NULL;
 
    smpp_ini.command_id  = _smpp_ini.command_id;
    _smpp_ini.command_id = -1;
  }
  if((SECTION_SIP_DEST & sections) != 0){
    sip_dest_ini.sip_dest_ip        = _sip_dest_ini.sip_dest_ip;
    _sip_dest_ini.sip_dest_ip       = NULL;

    sip_dest_ini.sip_dest_port      = _sip_dest_ini.sip_dest_port;
    _sip_dest_ini.sip_dest_port     = NULL;
  }
  if((SECTION_SIP_LOCAL & sections) != 0){
    sip_local_ini.sip_local_port    = _sip_local_ini.sip_local_port;
    _sip_local_ini.sip_local_port   = NULL;

    sip_local_ini.sip_local_ip      = _sip_local_ini.sip_local_ip;
    _sip_local_ini.sip_local_ip     = NULL;
  }
  if((SECTION_DBMS & sections) != 0){
    dbms_ini.dbms_name         = _dbms_ini.dbms_name;
    _dbms_ini.dbms_name        = NULL;

    dbms_ini.db_path           = _dbms_ini.db_path;
    _dbms_ini.db_path          = NULL;

    dbms_ini.db_basename       = _dbms_ini.db_basename;
    _dbms_ini.db_basename      = NULL;

    dbms_ini.db_dirname        = _dbms_ini.db_dirname;
    _dbms_ini.db_dirname       = NULL;

    dbms_ini.db_encoding       = _dbms_ini.db_encoding;
    _dbms_ini.db_encoding      = NULL;

    dbms_ini.db_ttl_sms        = _dbms_ini.db_ttl_sms;
    _dbms_ini.db_ttl_sms       = NULL;
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
bool check_file_ini(section_flags sections){
    bool b = true;
  
  if((SECTION_MAIN & sections) != 0){
    if(_main_ini.loglevel == NULL){
        ERROR(LOG_SCREEN,"\"loglevel = [0-8]\" must be defined in the INI file!");
        b = false;
    }
    
    if(_main_ini.fork == NULL){
        ERROR(LOG_SCREEN,"\"fork = [0-1]\" must be defined in the INI file!");
        b = false;
    }

  }
  if((SECTION_SMPP & sections) != 0){
    if(_smpp_ini.pass_smpp == NULL){
        ERROR(LOG_SCREEN,"\"pass_smpp = [password]\" must be defined in the INI file!");
        b = false;
    }

    if(_smpp_ini.user_smpp == NULL){
        ERROR(LOG_SCREEN,"\"user_smpp = [user_name]\" must be defined in the INI file!");
        b = false;
    }

    if(_smpp_ini.smpp_server_ip == NULL){
        ERROR(LOG_SCREEN,"\"smpp_server_ip = [xxx.xxx.xxx.xxx]\" must be defined in the INI file!");
        b = false;
    }

    if(_smpp_ini.smpp_server_port == NULL){
        ERROR(LOG_SCREEN,"\"smpp_server_port = [xxxxx]\" must be defined in the INI file!");
        b = false;
    }

    if(_smpp_ini.ton_src == -1 || _smpp_ini.ton_dst == -1){
        ERROR(LOG_SCREEN,"\"type_of_number = [0-6]\" or both \"type_of_number_dst and"
                         "type_of_number_src\" must be defined in the INI file!");
        b = false;
    }
    
    if(_smpp_ini.npi_src == -1 || _smpp_ini.npi_dst == -1){
        ERROR(LOG_SCREEN,"\"numeric_plan_indicator = [0,1,3,4,6,8,9,10,14,18]\""
                         " or both \"numeric_plan_indicator_dst and"
                         "numeric_plan_indicator_src\" must be defined in the INI file!");
        b = false;
    }

    if(_smpp_ini.system_type == NULL){
        ERROR(LOG_SCREEN,"\"system_type = [xxxxx]\" must be defined in the INI file!");
        b = false;
    }

    if(_smpp_ini.command_id == -1){
        ERROR(LOG_SCREEN,"\"command_id = [1-3]\" must be defined in the INI file!");
        b = false;
    }

    //if(_smpp_ini.service_type == NULL){
    //    ERROR(LOG_SCREEN,"\"service_type = [xxxxx]\" must be defined in the INI file!");
    //    b = false;
    //}
  }
  if((SECTION_SIP_DEST & sections) != 0){
    if(_sip_dest_ini.sip_dest_ip == NULL){
        ERROR(LOG_SCREEN,"\"sip_dest_ip = [xxx.xxx.xxx.xxx]\" must be defined in the INI file!");
        b = false;
    }

    if(_sip_dest_ini.sip_dest_port == NULL){
        ERROR(LOG_SCREEN,"\"sip_dest_port = [xxxxx]\" must be defined in the INI file!");
        b = false;
    }
  }
  if((SECTION_SIP_LOCAL & sections) != 0){
    if(_sip_local_ini.sip_local_port == NULL){
        ERROR(LOG_SCREEN,"\"sip_local_port = [xxxxx]\" must be defined in the INI file!");
        b = false;
    }

    if(_sip_local_ini.sip_local_ip == NULL){
        ERROR(LOG_SCREEN,"\"sip_local_ip = [xxx.xxx.xxx.xxx]\" must be defined in the INI file!");
        b = false;
    }
  }
  if((SECTION_DBMS & sections) != 0){
    if(_dbms_ini.dbms_name == NULL){
        ERROR(LOG_SCREEN,"\"dbms_name = [sqlite3]\" must be defined in the INI file!");
        b = false;
    }else if(strcmp(_dbms_ini.dbms_name,SQLITE) == 0){
        if(_dbms_ini.db_path == NULL){
            ERROR(LOG_SCREEN,"\"db_path = [path]\" must be defined in the INI file!");
            b = false;
        }
/*
        if(_dbms_ini.db_basename == NULL){
            ERROR(LOG_SCREEN,"\"db_basename = [base_name]\" must be defined in the INI file!");
	    b = false;
        }

        if(_dbms_ini.db_dirname == NULL){
            ERROR(LOG_SCREEN,"\"db_dirname = [path]\" must be defined in the INI file!");
            b = false;
        }
*/
    }else{ 
        ERROR(LOG_SCREEN,"%s is not supported!",_dbms_ini.dbms_name);
        b = false;
    }
/*
    if(_dbms_ini.db_encoding == NULL){
        ERROR(LOG_SCREEN,"\"db_encoding = [UTF-8]\" must be defined in the INI file!");
	b = false;
    }

    if(_dbms_ini.db_ttl_sms == NULL){
        ERROR(LOG_SCREEN,"\"db_ttl_sms = [int]\" must be defined in the INI file!");
	b = false;
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
*  \return Retrun true if the file is loaded
*
*/
bool load_file_ini(uint8_t *conffile, section_flags sections){ 
    bool b = false;
    
    if((SECTION_MAIN & sections) != 0){
      uint8_t loglevel_ini[10]   ="0";
      uint8_t fork_ini[10]       ="0";

      if(_main_ini.loglevel)
          free(_main_ini.loglevel);
      _main_ini.loglevel  = NULL;
      
      if(_main_ini.fork)
          free(_main_ini.fork);
      _main_ini.fork      = NULL;

      if(_main_ini.loglevel == NULL){
          ini_gets("MAIN", "loglevel", "none", loglevel_ini, sizearray(loglevel_ini), conffile);
          if(strcmp(loglevel_ini, "none") != 0){
              _strcpy(&(_main_ini.loglevel),loglevel_ini);
          }
      }

      if(_main_ini.fork == NULL){
          ini_gets("MAIN", "fork", "none", fork_ini, sizearray(fork_ini), conffile);
          if(strcmp(fork_ini, "none") != 0){
              _strcpy(&(_main_ini.fork),fork_ini);
          }
      }
    }

    if((SECTION_SMPP & sections) != 0){
      uint8_t smpp_server_ip_ini[100]   = "127.0.0.1";
      uint8_t smpp_server_port_ini[100] = "2775";
      uint8_t user_smpp_ini[100]        = "user";
      uint8_t pass_smpp_ini[100]        = "pass";
      int8_t  npi_ini                   = 1;//src and dst
      int8_t  ton_ini                   = 1;//src and dst
      uint8_t system_type_ini[30]       = "WWW";
      uint8_t service_type_ini[30]      = "";
      int8_t  command_id_ini            = 1;

      if(_smpp_ini.pass_smpp)
          free(_smpp_ini.pass_smpp);
      _smpp_ini.pass_smpp        = NULL;

      if(_smpp_ini.user_smpp)
          free(_smpp_ini.user_smpp);
      _smpp_ini.user_smpp        = NULL;

      if(_smpp_ini.smpp_server_ip)
          free(_smpp_ini.smpp_server_ip);
      _smpp_ini.smpp_server_ip   = NULL;

      if(_smpp_ini.smpp_server_port)
          free(_smpp_ini.smpp_server_port);
      _smpp_ini.smpp_server_port = NULL;

      _smpp_ini.npi_src = -1;
      _smpp_ini.ton_src = -1;
      _smpp_ini.npi_dst = -1;
      _smpp_ini.ton_dst = -1;

      if(_smpp_ini.system_type)
          free(_smpp_ini.system_type);
      _smpp_ini.system_type = NULL;

      if(_smpp_ini.service_type)
          free(_smpp_ini.service_type);
      _smpp_ini.service_type = NULL;
      
      _smpp_ini.command_id = -1;

      if(_smpp_ini.smpp_server_ip == NULL) {
          ini_gets("SMPP", "smpp_server_ip", "none", smpp_server_ip_ini, sizearray(smpp_server_ip_ini), conffile);
          if(strcmp(smpp_server_ip_ini, "none") != 0) {
              _strcpy(&(_smpp_ini.smpp_server_ip),smpp_server_ip_ini);
          }
      }

      if(_smpp_ini.smpp_server_port == NULL) {
          ini_gets("SMPP", "smpp_server_port", "none", smpp_server_port_ini, sizearray(smpp_server_port_ini), conffile);
          if(strcmp(smpp_server_port_ini, "none") != 0) {
              _strcpy(&(_smpp_ini.smpp_server_port),smpp_server_port_ini);
          }
      }

      if(_smpp_ini.user_smpp == NULL) {
          ini_gets("SMPP", "system_id_smpp", "none", user_smpp_ini, sizearray(user_smpp_ini), conffile);
          if(strcmp(user_smpp_ini, "none") != 0) {
              _strcpy(&(_smpp_ini.user_smpp),user_smpp_ini);
          }
      }

      if(_smpp_ini.pass_smpp == NULL) {
          ini_gets("SMPP", "pass_smpp", "none", pass_smpp_ini, sizearray(pass_smpp_ini), conffile);
          if(strcmp(pass_smpp_ini, "none") != 0) {
              _strcpy(&(_smpp_ini.pass_smpp),pass_smpp_ini);
          }
      }

      if(_smpp_ini.npi_src == -1 || _smpp_ini.npi_dst == -1) {
          npi_ini = ini_getl("SMPP", "numeric_plan_indicator", -1, conffile);
          if(npi_ini != -1) {
              _smpp_ini.npi_src = npi_ini;
              _smpp_ini.npi_dst = npi_ini;
          }else{
              if(_smpp_ini.npi_src == -1) {
                    npi_ini = ini_getl("SMPP", "numeric_plan_indicator_src", -1, conffile);
                    if(npi_ini != -1) {
                        _smpp_ini.npi_src = npi_ini;
                    }
              }
              if(_smpp_ini.npi_dst == -1) {
                    npi_ini = ini_getl("SMPP", "numeric_plan_indicator_dst", -1, conffile);
                    if(npi_ini != -1) {
                        _smpp_ini.npi_dst = npi_ini;
                    }
              }
          }
      }

      if(_smpp_ini.ton_src == -1 || _smpp_ini.ton_dst == -1) {
          ton_ini = ini_getl("SMPP", "type_of_number", -1, conffile);
          if(ton_ini != -1) {
              _smpp_ini.ton_src = ton_ini;
              _smpp_ini.ton_dst = ton_ini;
          }else{
              if(_smpp_ini.ton_src == -1) {
                    ton_ini = ini_getl("SMPP", "type_of_number_src", -1, conffile);
                    if(ton_ini != -1) {
                        _smpp_ini.ton_src = ton_ini;
                    }
              }
              if(_smpp_ini.ton_dst == -1) {
                    ton_ini = ini_getl("SMPP", "type_of_number_dst", -1, conffile);
                    if(ton_ini != -1) {
                        _smpp_ini.ton_dst = ton_ini;
                    }
              }
          }
      }

      if(_smpp_ini.system_type == NULL) {
          ini_gets("SMPP", "system_type", "none", system_type_ini, sizearray(system_type_ini), conffile);
          if(strcmp(system_type_ini, "none") != 0) {
              _strcpy(&(_smpp_ini.system_type),system_type_ini);
          }
      }

      if(_smpp_ini.service_type == NULL) {
          ini_gets("SMPP", "service_type", "none", service_type_ini, sizearray(service_type_ini), conffile);
          if(strcmp(service_type_ini, "none") != 0) {
              _strcpy(&(_smpp_ini.service_type),service_type_ini);
          }
      }

      if(_smpp_ini.command_id == -1) {
          command_id_ini = ini_getl("SMPP", "command_id", -1, conffile);
          if(command_id_ini != -1 && command_id_ini >= 1 && command_id_ini <= 3) {
              _smpp_ini.command_id = command_id_ini;
          }
      }
    }

    if((SECTION_SIP_DEST & sections) != 0){
      uint8_t sip_dest_port_ini[20]    ="127.0.0.1";
      uint8_t sip_dest_ip_ini[20]      ="5060";

      if(_sip_dest_ini.sip_dest_ip)
          free(_sip_dest_ini.sip_dest_ip);
      _sip_dest_ini.sip_dest_ip      = NULL;

      if(_sip_dest_ini.sip_dest_port)
          free(_sip_dest_ini.sip_dest_port);
      _sip_dest_ini.sip_dest_port    = NULL;

      if(_sip_dest_ini.sip_dest_ip == NULL) {
          ini_gets("SIP_DEST", "sip_dest_ip", "none", sip_dest_ip_ini, sizearray(sip_dest_ip_ini), conffile);
          if(strcmp(sip_dest_ip_ini, "none") != 0){
              _strcpy(&(_sip_dest_ini.sip_dest_ip),sip_dest_ip_ini);
          }
      }

      if(_sip_dest_ini.sip_dest_port == NULL) {
          ini_gets("SIP_DEST", "sip_dest_port", "none", sip_dest_port_ini, sizearray(sip_dest_port_ini), conffile);
          if(strcmp(sip_dest_port_ini, "none") != 0) {
              _strcpy(&(_sip_dest_ini.sip_dest_port),sip_dest_port_ini);
          }
      }
    }

    if((SECTION_SIP_LOCAL & sections) != 0){
      uint8_t sip_local_port_ini[20]   ="127.0.0.1";
      uint8_t sip_local_ip_ini[20]     ="5080";

      if(_sip_local_ini.sip_local_port)
          free(_sip_local_ini.sip_local_port);
      _sip_local_ini.sip_local_port   = NULL;

      if(_sip_local_ini.sip_local_ip)
          free(_sip_local_ini.sip_local_ip);
      _sip_local_ini.sip_local_ip     = NULL;
    
      if(_sip_local_ini.sip_local_ip == NULL) {
          ini_gets("SIP_LOCAL", "sip_local_ip", "none", sip_local_ip_ini, sizearray(sip_local_ip_ini), conffile);
          if(strcmp(sip_local_ip_ini, "none") != 0){
              _strcpy(&(_sip_local_ini.sip_local_ip),sip_local_ip_ini);
          }
      }

      if(_sip_local_ini.sip_local_port == NULL) {
          ini_gets("SIP_LOCAL", "sip_local_port", "none", sip_local_port_ini, sizearray(sip_local_port_ini), conffile);
          if(strcmp(sip_local_port_ini, "none") != 0) {
              _strcpy(&(_sip_local_ini.sip_local_port),sip_local_port_ini);
          }
      }
    }

    if((SECTION_DBMS & sections) != 0){
      uint8_t dbms_name_ini[20]        ="sqlite3";
      uint8_t db_path_ini[200]         ="/etc/sip2smpp/sip2smpp.sqlite3";
      uint8_t db_basename_ini[50]      ="sip2smpp";
      uint8_t db_dirname_ini[200]      ="/var/sqlite3/sip2smpp";
      uint8_t db_encoding_ini[10]      ="UTF-8";
      uint8_t db_ttl_sms_ini[10]	     ="0";

      if(_dbms_ini.dbms_name)
          free(_dbms_ini.dbms_name);
      _dbms_ini.dbms_name        = NULL;

      if(_dbms_ini.db_path)
          free(_dbms_ini.db_path);
      _dbms_ini.db_path      = NULL;

      if(_dbms_ini.db_basename)
          free(_dbms_ini.db_basename);
      _dbms_ini.db_basename      = NULL;

      if(_dbms_ini.db_dirname)
          free(_dbms_ini.db_dirname);
      _dbms_ini.db_dirname       = NULL;

      if(_dbms_ini.db_encoding)
          free(_dbms_ini.db_encoding);
      _dbms_ini.db_encoding      = NULL;

      if(_dbms_ini.db_ttl_sms)
          free(_dbms_ini.db_ttl_sms);
      _dbms_ini.db_ttl_sms       = NULL;
    
      if(_dbms_ini.dbms_name == NULL){
          ini_gets("DBMS", "dbms_name", "none", dbms_name_ini, sizearray(dbms_name_ini), conffile);
	  if(strcmp(dbms_name_ini, "none") != 0) {
              _strcpy(&(_dbms_ini.dbms_name),dbms_name_ini);
	  }
      }

      if(_dbms_ini.db_path == NULL){
          ini_gets("DBMS", "db_path", "none", db_path_ini, sizearray(db_path_ini), conffile);
          if(strcmp(db_path_ini, "none") != 0) {
              _strcpy(&(_dbms_ini.db_path),db_path_ini);
          }
      }

      if(_dbms_ini.db_basename == NULL){
          ini_gets("DBMS", "db_basename", "none", db_basename_ini, sizearray(db_basename_ini), conffile);
          if(strcmp(db_basename_ini, "none") != 0) {
              _strcpy(&(_dbms_ini.db_basename),db_basename_ini);
          }
      }

      if(_dbms_ini.db_dirname == NULL){
          ini_gets("DBMS", "db_dirname", "none", db_dirname_ini, sizearray(db_dirname_ini), conffile);
          if(strcmp(db_dirname_ini, "none") != 0) {
	      _strcpy(&(_dbms_ini.db_dirname),db_dirname_ini);
          }
      }

      if(_dbms_ini.db_encoding == NULL){
          ini_gets("DBMS", "db_encoding", "UTF-8", db_encoding_ini, sizearray(db_encoding_ini), conffile);
          if(strcmp(db_encoding_ini, "none") != 0) {
	      _strcpy(&(_dbms_ini.db_encoding),db_encoding_ini);
          }
      }

      if(_dbms_ini.db_ttl_sms == NULL){
          ini_gets("DBMS", "db_ttl_sms", "0", db_ttl_sms_ini, sizearray(db_ttl_sms_ini), conffile);
          if(strcmp(db_ttl_sms_ini, "none") != 0) {
	      _strcpy(&(_dbms_ini.db_ttl_sms),db_ttl_sms_ini);
          }
      }
    }

    if((b = check_file_ini(sections)) == true){
       _save_file_ini(sections);
    }

    return b;
}

static void _strcpy(uint8_t** str_out, uint8_t* str_in){
	*str_out = (uint8_t*)calloc(strlen((char*)str_in)+1,sizeof(uint8_t));
	strcpy((char*)*str_out,(char*)str_in);
	return;
}

