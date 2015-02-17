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

static ini_main   _main_ini;
static ini_sip    _sip_remote_ini;
static ini_sip    _sip_host_ini;
static ini_smpp   _smpp_ini;
static ini_dbms   _dbms_ini;

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
    if(smpp_ini.interface_name)
        free(smpp_ini.interface_name);
    smpp_ini.interface_name   = NULL;

    if(smpp_ini.pass_smpp)
        free(smpp_ini.pass_smpp);
    smpp_ini.pass_smpp        = NULL;

    if(smpp_ini.user_smpp)
        free(smpp_ini.user_smpp);
    smpp_ini.user_smpp        = NULL;

    if(smpp_ini.smpp_ip)
        free(smpp_ini.smpp_ip);
    smpp_ini.smpp_ip          = NULL;

    smpp_ini.smpp_port        = 0;

    smpp_ini.nb_client        = -1;
    
    smpp_ini.model            = MODEL_CLIENT;
    
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

    smpp_ini.command_id       = -1;

  }
  //sip_dest_ini
  if((SECTION_SIP_REMOTE & sections) != 0){
    if(sip_remote_ini.interface_name)
        free(sip_remote_ini.interface_name);
    sip_remote_ini.interface_name   = NULL;

    sip_remote_ini.model            = MODEL_CLIENT;

    if(sip_remote_ini.sip_ip)
        free(sip_remote_ini.sip_ip);
    sip_remote_ini.sip_ip           = NULL;

    sip_remote_ini.sip_port         = 0;
  }
  //sip_local_ini
  if((SECTION_SIP_HOST & sections) != 0){
    if(sip_host_ini.interface_name)
        free(sip_host_ini.interface_name);
    sip_host_ini.interface_name   = NULL;

    sip_host_ini.model            = MODEL_SERVER;

    if(sip_host_ini.sip_ip)
        free(sip_host_ini.sip_ip);
    sip_host_ini.sip_ip           = NULL;
    
    sip_host_ini.sip_port         = 0;
  }
  //dbms_ini
  if((SECTION_DBMS & sections) != 0){
    if(dbms_ini.dbms_name)
        free(dbms_ini.dbms_name);
    dbms_ini.dbms_name       = NULL;

    if(dbms_ini.db_path)
        free(dbms_ini.db_path);
    dbms_ini.db_path         = NULL;

    if(dbms_ini.db_encoding)
        free(dbms_ini.db_encoding);
    dbms_ini.db_encoding     = NULL;

    if(dbms_ini.db_synchronous)
        free(dbms_ini.db_synchronous);
    dbms_ini.db_synchronous  = NULL;

    dbms_ini.db_heap_limit   = 0;

    dbms_ini.db_foreign_keys = 0;

    dbms_ini.db_ttl_sms      = 0;
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
    smpp_ini.interface_name     = _smpp_ini.interface_name;
    _smpp_ini.interface_name    = NULL;

    smpp_ini.model              = _smpp_ini.model;
    _smpp_ini.model             = 0;

    smpp_ini.pass_smpp          = _smpp_ini.pass_smpp;
    _smpp_ini.pass_smpp         = NULL;

    smpp_ini.user_smpp          = _smpp_ini.user_smpp;
    _smpp_ini.user_smpp         = NULL;

    smpp_ini.smpp_ip            = _smpp_ini.smpp_ip;
    _smpp_ini.smpp_ip           = NULL;

    smpp_ini.smpp_port          = _smpp_ini.smpp_port;
    _smpp_ini.smpp_port         = 0;

    smpp_ini.npi_src            = _smpp_ini.npi_src;
    _smpp_ini.npi_src           = -1;

    smpp_ini.nb_client          = _smpp_ini.nb_client;
    _smpp_ini.nb_client         = 0;

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
  if((SECTION_SIP_REMOTE & sections) != 0){
    sip_remote_ini.interface_name   = _sip_remote_ini.interface_name;
    _sip_remote_ini.interface_name  = NULL;

    sip_remote_ini.model            = _sip_remote_ini.model;
    _sip_remote_ini.model           = 0;

    sip_remote_ini.sip_ip           = _sip_remote_ini.sip_ip;
    _sip_remote_ini.sip_ip          = NULL;

    sip_remote_ini.sip_port         = _sip_remote_ini.sip_port;
    _sip_remote_ini.sip_port        = 0;
  }
  if((SECTION_SIP_HOST & sections) != 0){
    sip_host_ini.interface_name   = _sip_host_ini.interface_name;
    _sip_host_ini.interface_name  = NULL;

    sip_host_ini.model            = _sip_host_ini.model;
    _sip_host_ini.model           = 0;

    sip_host_ini.sip_ip           = _sip_host_ini.sip_ip;
    _sip_host_ini.sip_ip          = NULL;

    sip_host_ini.sip_port         = _sip_host_ini.sip_port;
    _sip_host_ini.sip_port        = 0;
  }
  if((SECTION_DBMS & sections) != 0){
    dbms_ini.dbms_name         = _dbms_ini.dbms_name;
    _dbms_ini.dbms_name        = NULL;

    dbms_ini.db_path           = _dbms_ini.db_path;
    _dbms_ini.db_path          = NULL;

    dbms_ini.db_encoding       = _dbms_ini.db_encoding;
    _dbms_ini.db_encoding      = NULL;

    dbms_ini.db_synchronous    = _dbms_ini.db_synchronous;
    _dbms_ini.db_synchronous   = NULL;

    dbms_ini.db_heap_limit     = _dbms_ini.db_heap_limit;
    _dbms_ini.db_heap_limit    = 0;

    dbms_ini.db_foreign_keys   = _dbms_ini.db_foreign_keys;
    _dbms_ini.db_foreign_keys  = 0;
    
    dbms_ini.db_ttl_sms        = _dbms_ini.db_ttl_sms;
    _dbms_ini.db_ttl_sms       = 0;
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
    if(_smpp_ini.interface_name == NULL){
        ERROR(LOG_SCREEN,"\"interface_name = [interface_name]\" must be defined in the INI file!");
        b = false;
    }

    if(_smpp_ini.pass_smpp == NULL){
        ERROR(LOG_SCREEN,"\"pass_smpp = [password]\" must be defined in the INI file!");
        b = false;
    }

    if(_smpp_ini.user_smpp == NULL){
        ERROR(LOG_SCREEN,"\"user_smpp = [user_name]\" must be defined in the INI file!");
        b = false;
    }

    if(_smpp_ini.smpp_ip == NULL){
        ERROR(LOG_SCREEN,"\"smpp_ip = [xxx.xxx.xxx.xxx]\" must be defined in the INI file!");
        b = false;
    }

    if(_smpp_ini.smpp_port <= 0){
        ERROR(LOG_SCREEN,"\"smpp_port = [xxxxx]\" must be defined in the INI file!");
        b = false;
    }

/*    if(_smpp_ini.model == MODEL_SERVER || _smpp_ini.model == MODEL_CLIENT){
        ERROR(LOG_SCREEN,"\"model = [CLIENT(0)/SERVER(1)]\" must be defined in the INI file part SMPP!");
        b = false;
    }*/

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
  if((SECTION_SIP_REMOTE & sections) != 0){
    if(_sip_remote_ini.interface_name == NULL){
        ERROR(LOG_SCREEN,"\"interface_name = [interface_name]\" must be defined in the INI file!");
        b = false;
    }

    if(_sip_remote_ini.sip_ip == NULL){
        ERROR(LOG_SCREEN,"\"sip_ip = [xxx.xxx.xxx.xxx]\" must be defined in the INI file!");
        b = false;
    }

    if(_sip_remote_ini.sip_port == NULL){
        ERROR(LOG_SCREEN,"\"sip_port = [xxxxx]\" must be defined in the INI file!");
        b = false;
    }

/*    if(_sip_remote_ini.model == NULL){
        ERROR(LOG_SCREEN,"\"model = [CLIENT(0)/SERVER(1)]\" must be defined in the INI file part SIP_REMOTE!");
        b = false;
    }*/
  }
  if((SECTION_SIP_HOST & sections) != 0){
    if(_sip_host_ini.interface_name == NULL){
        ERROR(LOG_SCREEN,"\"interface_name = [xxx.xxx.xxx.xxx]\" must be defined in the INI file!");
        b = false;
    }

    if(_sip_host_ini.sip_port == NULL){
        ERROR(LOG_SCREEN,"\"sip_port = [xxxxx]\" must be defined in the INI file!");
        b = false;
    }

    if(_sip_host_ini.sip_ip == NULL){
        ERROR(LOG_SCREEN,"\"sip_ip = [xxx.xxx.xxx.xxx]\" must be defined in the INI file!");
        b = false;
    }

/*    if(_sip_host_ini.model == NULL){
        ERROR(LOG_SCREEN,"\"model = [CLIENT(0)/SERVER(1)]\" must be defined in the INI file part SIP_HOST!");
        b = false;
    }*/
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

/*        if(_dbms_ini.db_encoding == NULL){
            ERROR(LOG_SCREEN,"\"db_encoding = [UTF-8]\" must be defined in the INI file!");
            b = false;
        }

        if(_dbms_ini.db_synchronous == NULL){
            ERROR(LOG_SCREEN,"\"db_synchronous = [normal|full]\" must be defined in the INI file!");
            b = false;
        }

        if(_dbms_ini.heap_limit > 0){
            ERROR(LOG_SCREEN,"\"db_heap_limit = [size_ko]\" must be defined in the INI file!");
            b = false;
        }

        if(_dbms_ini.db_foreign_keys == 0 | ){
            ERROR(LOG_SCREEN,"\"db_foreign_keys = [bool]\" must be defined in the INI file!");
            b = false;
        }

        if(_dbms_ini.db_ttl_sms == NULL){
            ERROR(LOG_SCREEN,"\"db_ttl_sms = [int8]\" must be defined in the INI file!");
            b = false;
        }
*/
    }else{ 
        ERROR(LOG_SCREEN,"%s is not supported!",_dbms_ini.dbms_name);
        b = false;
    }
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
      uint8_t  smpp_interface_name_ini[50] = "SMPP_01";
      uint8_t  smpp_model_ini              = 0;
      uint8_t  smpp_ip_ini[100]            = "127.0.0.1";
      uint32_t smpp_port_ini               = 2775;
      uint8_t  smpp_nb_client_ini          = 0;
      uint8_t  smpp_user_ini[100]          = "user";
      uint8_t  smpp_pass_ini[100]          = "pass";
      int8_t   npi_ini                     = 1;//src and dst
      int8_t   ton_ini                     = 1;//src and dst
      uint8_t  system_type_ini[30]         = "WWW";
      uint8_t  service_type_ini[30]        = "";
      int8_t   command_id_ini              = 1;


      if(_smpp_ini.interface_name)
          free(_smpp_ini.interface_name);
      _smpp_ini.interface_name   = NULL;
      _smpp_ini.model     = 0;
      if(_smpp_ini.smpp_ip)
          free(_smpp_ini.smpp_ip);
      _smpp_ini.smpp_ip   = NULL;
      _smpp_ini.smpp_port = NULL;
      _smpp_ini.nb_client = 0;
      if(_smpp_ini.pass_smpp)
          free(_smpp_ini.pass_smpp);
      _smpp_ini.pass_smpp        = NULL;
      if(_smpp_ini.user_smpp)
          free(_smpp_ini.user_smpp);
      _smpp_ini.user_smpp        = NULL;
      _smpp_ini.npi_src   = -1;
      _smpp_ini.ton_src   = -1;
      _smpp_ini.npi_dst   = -1;
      _smpp_ini.ton_dst   = -1;
      if(_smpp_ini.system_type)
          free(_smpp_ini.system_type);
      _smpp_ini.system_type = NULL;
      if(_smpp_ini.service_type)
          free(_smpp_ini.service_type);
      _smpp_ini.service_type = NULL;
      _smpp_ini.command_id = -1;


      if(_smpp_ini.interface_name == NULL) {
          ini_gets("SMPP", "interface_name", "none", smpp_interface_name_ini, sizearray(smpp_interface_name_ini), conffile);
          if(strcmp(smpp_interface_name_ini, "none") != 0) {
              _strcpy(&(_smpp_ini.interface_name),smpp_interface_name_ini);
          }
      }
      
      if(_smpp_ini.model == 0) {
          smpp_model_ini = ini_getl("SMPP", "model", 0, conffile);
          if(smpp_model_ini == 0 || smpp_model_ini == 1) {
              _smpp_ini.model = smpp_model_ini;
          }
      }

      if(_smpp_ini.smpp_ip == NULL) {
          ini_gets("SMPP", "smpp_ip", "none", smpp_ip_ini, sizearray(smpp_ip_ini), conffile);
          if(strcmp(smpp_ip_ini, "none") != 0) {
              _strcpy(&(_smpp_ini.smpp_ip), smpp_ip_ini);
          }
      }

      if(_smpp_ini.smpp_port == 0) {
          smpp_port_ini = ini_getl("SMPP", "smpp_port", 0, conffile);
          if(smpp_port_ini > 0) {
              _smpp_ini.smpp_port = smpp_port_ini;
          }
      }

      if(_smpp_ini.nb_client == 0) {
          smpp_nb_client_ini = ini_getl("SMPP", "nb_client", 1, conffile);
          if(smpp_nb_client_ini > 0) {
              _smpp_ini.nb_client = smpp_nb_client_ini;
          }
      }

      if(_smpp_ini.user_smpp == NULL){
          ini_gets("SMPP", "system_id_smpp", "none", smpp_user_ini, sizearray(smpp_user_ini), conffile);
          if(strcmp(smpp_user_ini, "none") != 0) {
              _strcpy(&(_smpp_ini.user_smpp), smpp_user_ini);
          }
      }

      if(_smpp_ini.pass_smpp == NULL) {
          ini_gets("SMPP", "pass_smpp", "none", smpp_pass_ini, sizearray(smpp_pass_ini), conffile);
          if(strcmp(smpp_pass_ini, "none") != 0) {
              _strcpy(&(_smpp_ini.pass_smpp), smpp_pass_ini);
          }
      }

      if(_smpp_ini.npi_src == -1 || _smpp_ini.npi_dst == -1){
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

    if((SECTION_SIP_REMOTE & sections) != 0){
      uint8_t  sip_interface_name_ini[50] = "SIP_REMOTE";
      uint8_t  sip_model_ini       = 0;
      uint8_t  sip_ip_ini[20]      = "127.0.0.1";
      uint16_t sip_port_ini        = 5060;

      if(_sip_remote_ini.interface_name)
          free(_sip_remote_ini.interface_name);
      _sip_remote_ini.interface_name = NULL;
      _sip_remote_ini.model    = 0;
      if(_sip_remote_ini.sip_ip)
          free(_sip_remote_ini.sip_ip);
      _sip_remote_ini.sip_ip      = NULL;
      _sip_remote_ini.sip_port    = 0;


      if(_sip_remote_ini.interface_name == NULL) {
          ini_gets("SIP_REMOTE", "interface_name", "none", sip_interface_name_ini, sizearray(sip_interface_name_ini), conffile);
          if(strcmp(sip_interface_name_ini, "none") != 0) {
              _strcpy(&(_sip_remote_ini.interface_name), sip_interface_name_ini);
          }
      }
      
      if(_sip_remote_ini.model == 0) {
          sip_model_ini = ini_getl("SIP_REMOTE", "model", 0, conffile);
          if(sip_model_ini == 0 || sip_model_ini == 1) {
              _sip_remote_ini.model = sip_model_ini;
          }
      }

      if(_sip_remote_ini.sip_ip == NULL){
          ini_gets("SIP_REMOTE", "sip_ip", "none", sip_ip_ini, sizearray(sip_ip_ini), conffile);
          if(strcmp(sip_ip_ini, "none") != 0){
              _strcpy(&(_sip_remote_ini.sip_ip), sip_ip_ini);
          }
      }

      if(_sip_remote_ini.sip_port == 0) {
          sip_port_ini = ini_getl("SIP_REMOTE", "sip_port", 0, conffile);
          if(sip_port_ini > 0) {
              _sip_remote_ini.sip_port = sip_port_ini;
          }
      }

    }

    if((SECTION_SIP_HOST & sections) != 0){
      uint8_t  sip_interface_name_ini[50] = "SIP_REMOTE";
      uint8_t  sip_model_ini       = 0;
      uint8_t  sip_ip_ini[20]      = "127.0.0.1";
      uint16_t sip_port_ini        = 5060;

      if(_sip_host_ini.interface_name)
          free(_sip_host_ini.interface_name);
      _sip_host_ini.interface_name = NULL;
      _sip_host_ini.model    = 0;
      if(_sip_host_ini.sip_ip)
          free(_sip_host_ini.sip_ip);
      _sip_host_ini.sip_ip      = NULL;
      _sip_host_ini.sip_port    = 0;


      if(_sip_host_ini.interface_name == NULL) {
          ini_gets("SIP_HOST", "interface_name", "none", sip_interface_name_ini, sizearray(sip_interface_name_ini), conffile);
          if(strcmp(sip_interface_name_ini, "none") != 0) {
              _strcpy(&(_sip_host_ini.interface_name), sip_interface_name_ini);
          }
      }
      
      if(_sip_host_ini.model == 0) {
          sip_model_ini = ini_getl("SIP_HOST", "model", 0, conffile);
          if(sip_model_ini == 0 || sip_model_ini == 1) {
              _sip_host_ini.model = sip_model_ini;
          }
      }

      if(_sip_host_ini.sip_ip == NULL){
          ini_gets("SIP_HOST", "sip_ip", "none", sip_ip_ini, sizearray(sip_ip_ini), conffile);
          if(strcmp(sip_ip_ini, "none") != 0){
              _strcpy(&(_sip_host_ini.sip_ip), sip_ip_ini);
          }
      }

      if(_sip_host_ini.sip_port == 0) {
          sip_port_ini = ini_getl("SIP_HOST", "sip_port", 0, conffile);
          if(sip_port_ini > 0) {
              _sip_host_ini.sip_port = sip_port_ini;
          }
      }
    }

    if((SECTION_DBMS & sections) != 0){
      uint8_t  dbms_name_ini[20]        = "sqlite3";
      uint8_t  db_path_ini[200]         = "/etc/sip2smpp/sip2smpp.sqlite3";
      uint8_t  db_encoding_ini[10]      = "UTF-8";
      uint8_t  db_synchronous_ini[10]   = "normal";
      uint32_t db_heap_limit_ini        = 8096;
      uint8_t  db_foreign_keys_ini      = 0;
      uint8_t  db_ttl_sms_ini	          = 0;

      if(_dbms_ini.dbms_name)
          free(_dbms_ini.dbms_name);
      _dbms_ini.dbms_name        = NULL;
      if(_dbms_ini.db_path)
          free(_dbms_ini.db_path);
      _dbms_ini.db_path          = NULL;
      if(_dbms_ini.db_encoding)
          free(_dbms_ini.db_encoding);
      _dbms_ini.db_encoding      = NULL;
      if(_dbms_ini.db_synchronous)
          free(_dbms_ini.db_synchronous);
      _dbms_ini.db_synchronous   = NULL;
      _dbms_ini.db_heap_limit    = 0;
      _dbms_ini.db_encoding      = 0;
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

      if(_dbms_ini.db_encoding == NULL){
          ini_gets("DBMS", "db_encoding", "none", db_encoding_ini, sizearray(db_encoding_ini), conffile);
          if(strcmp(db_encoding_ini, "none") != 0) {
              _strcpy(&(_dbms_ini.db_encoding),db_encoding_ini);
          }
      }

      if(_dbms_ini.db_synchronous == NULL){
          ini_gets("DBMS", "db_synchronous", "none", db_synchronous_ini, sizearray(db_synchronous_ini), conffile);
          if(strcmp(db_synchronous_ini, "none") != 0) {
	      _strcpy(&(_dbms_ini.db_synchronous),db_synchronous_ini);
          }
      }

      if(_dbms_ini.db_heap_limit == NULL) {
          db_heap_limit_ini = ini_getl("DBMS", "db_heap_limit", 0, conffile);
          if(db_heap_limit_ini > 0) {
              _dbms_ini.db_heap_limit = db_heap_limit_ini;
          }
      }

      if(_dbms_ini.db_foreign_keys == NULL) {
          db_foreign_keys_ini = ini_getl("DBMS", "db_foreign_keys", 0, conffile);
          if(db_foreign_keys_ini > 0) {
              _dbms_ini.db_foreign_keys = db_foreign_keys_ini;
          }
      }

      if(_dbms_ini.db_ttl_sms == NULL) {
          db_ttl_sms_ini = ini_getl("DBMS", "db_ttl_sms", 0, conffile);
          if(db_ttl_sms_ini > 0) {
              _dbms_ini.db_ttl_sms = db_ttl_sms_ini;
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

