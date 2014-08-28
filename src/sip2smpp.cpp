/**
*   \file main.cpp
*
*   \brief The main file of the SIP2SMPP project
*
*/

/**
*  \todo   1) use a log system (like : Log4c)
*          2) add a deamon mode
*/

#include <iostream>
#include <queue>

#include <sys/cdefs.h>
#include <sys/types.h>
#include <sys/socket.h>

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <errno.h>
#include <string.h>
#include <ctype.h>
#include <time.h>
#include <getopt.h>
#include <sstream>
//THREAD - SEMAPHORE
#include <pthread.h>
#include <semaphore.h>
//SMPP
#include "smpp_io.h"
//SIP
#include "sip_io.h"
#include "sip_create.h"
#include "sip_parse.h"
//INI FILE
#include "ini/iniFile.h"
//LOG
#include "log/log.h"
//OTHER
//#include "type_projet.h"
#include "sms_struct.h"
#include "database.h"
#include "daemonize/daemonize.h"

using namespace std;

int running = 1;
char* pid_file = (char*)DEFAULT_PIDFILE;

/**
*  \brief This function is a signal handler function
*
*  \param value This parameter is a signal number
*
*/
void handler(int value){
    INFO(LOG_SCREEN | LOG_FILE,"The process has been terminated");
    log_destroy();
    exit(value);
}

/**
*  \brief This function print the help of this program
*
*  \param e  This parameter is used for define the signal number
*
*/
void usage(int8_t value){
    printf("%sHelp :%s\n", RED, END_COLOR);

    printf("sip2smpp [%soptions%s]  \n", CYAN, END_COLOR);
    printf("%s    -h  %s: help\n", CYAN, END_COLOR);
    printf("%s    -v  %s: show version\n", CYAN, END_COLOR);
    printf("%s    -D  %s: debug level (0-8)\n", CYAN, END_COLOR);
    printf("%s    -f  %s: use fork (parameter 1) | Not implemented\n", CYAN, END_COLOR);
    printf("%s    -P  %s: PID file. Default PID file is [%s]\n", CYAN, END_COLOR, DEFAULT_PIDFILE);
    printf("%s    -c  %s: config file to use to specify some options. Default location is [%s]\n", CYAN, END_COLOR, DEFAULT_CONFIG);
    printf("%s    -l  %s: log file to use to specify some options. Default location is [%s]\n", CYAN, END_COLOR, DEFAULT_CONFIG);

    handler(value);
}

/**
*  CONVERSIONS :
*    - trame SIP -> struct SMS
*/

std::vector<std::string> &split(const std::string &s, char delim, std::vector<std::string> &elems) {
    std::stringstream ss(s);
    std::string item;
    while (std::getline(ss, item, delim)) {
        elems.push_back(item);
    }
    return elems;
}

std::vector<std::string> split(const std::string &s, char delim) {
    std::vector<std::string> elems;
    split(s, delim, elems);
    return elems;
}

SMS* sip2sms(string str){
    std::vector<std::string> explode = split(str,'\n');
    int i = 0; // search FROM in the SIP trame
    while(strncmp((char*)explode[i++].c_str(),(char*)"From",4)!=0);
    Sip_From from(explode[i-1]);

    i = 0;// search TO in the SIP trame
    while(strncmp((char*)explode[i++].c_str(),(char*)"To",2)!=0);
    Sip_To to(explode[i-1]);

    SMS *sms = (SMS*)calloc(1,sizeof(SMS));
    sms->src = (uint8_t*)calloc(21,sizeof(uint8_t));
    sms->dst = (uint8_t*)calloc(21,sizeof(uint8_t));
    sms->msg = (uint8_t*)calloc(256,sizeof(uint8_t));
    strcpy((char*)sms->src,(char*)from.get_user_name().c_str());
    strcpy((char*)sms->dst,(char*)to.get_user_name().c_str());
    strcpy((char*)sms->msg,(char*)explode[explode.size()-1].c_str());

    return sms;
}

/**
*  Init Connection
*/
sip_socket  *p_sip_socket  = NULL;
smpp_socket *p_smpp_socket = NULL;

/**
 * size_db : allow to decrease SQL queries
 */
static unsigned int size_smpp = 0;
static unsigned int size_sip  = 0;

/**
* CONNEXION SMPP
*/
sem_t mutex_smpp;

/**
*  \brief This function is used for send all SMS of the DB (SMPP)
*/
static void* gestionSMPP_send(void *data){
	sem_wait(&mutex_smpp);
	
	if(size_smpp>0){
		SMS *sms = (SMS*)calloc(1,sizeof(SMS));
		
		sms_get(DB_TYPE_SMPP,sms);
		if(sms){
			smpp_send_message(p_smpp_socket,sms->src,sms->dst,sms->msg);
			sms_rm(sms);
			size_smpp--;
		}else{
			sms_cls(sms);
		}
		free_sms(&sms);
	}
	
	sem_post(&mutex_smpp);
	return 0;
}

/**
*  \brief This function is used for transfer all SMS received to the DB (SMPP->SIP)
*/
static void* gestionSMPP_listend(void *data){
    SMS sms;
    memset(&sms,0,sizeof(SMS));
    sem_wait(&mutex_smpp);

    smpp_receive_message(p_smpp_socket,&(sms.src),&(sms.dst),&(sms.msg));
    if(sms.dst && sms.src && sms.msg){
        display_smpp_message(sms.src,sms.dst,sms.msg);
        sms_set(DB_TYPE_SIP,sms.src,sms.dst,sms.msg);
        size_sip++;
    }	

    sem_post(&mutex_smpp);
    return 0;
}

/**
*  \brief This function is used for managed all input and output SMPP trafic
*/
bool warning_smpp = true;
static void* gestionSMPP(void *data){
    smpp_start_connection(p_smpp_socket);

    while(running){
        sem_init(&mutex_smpp, 0, 1);

        while(p_smpp_socket && p_smpp_socket->status == SMPP_CONNECT){
            warning_smpp = true;
            pthread_t thread_listend;
            pthread_t thread_send;

            pthread_create(&thread_listend,NULL,gestionSMPP_listend,NULL);
            pthread_create(&thread_send,NULL,gestionSMPP_send,NULL);
      
            pthread_join(thread_listend,NULL);
            pthread_join(thread_send,NULL);
        }

        if(warning_smpp){
            WARNING(LOG_SCREEN | LOG_FILE, "smpp socket is null")
            warning_smpp = false;
        }
        sem_destroy(&mutex_smpp);
        usleep(2000);
    }

    return 0;
}

/**
*  CONNEXION SIP
*/
sem_t mutex_sip;

/**
*  \brief This function is used for transfer all SMS received to the SMS listend FIFO (SIP)
*/
static void* gestionSIP_listend(void *data){
	uint8_t *trame_sip = NULL;
	sem_wait(&mutex_sip);
	
  if(sip_receive_message(p_sip_socket,&trame_sip) == SIP_ERROR_NO && trame_sip && strlen((char*)trame_sip)>0){
    uint8_t *buf_sip_ok = NULL;
		SMS *sms = sip2sms((char*)trame_sip);
		if(sms && (sms->dst) && (sms->src) && (sms->msg)){
			sms_set(DB_TYPE_SMPP,sms->src,sms->dst,sms->msg);
			size_smpp++;
		}

		buf_sip_ok = create_trame_sip_200_ok(sip_dest_ini.sip_dest_ip, sip_dest_ini.sip_dest_port,
		      			    sip_local_ini.sip_local_ip, sip_local_ini.sip_local_port,
					          sms->dst, sms->src, get_call_id(trame_sip));
    sip_send_message(p_sip_socket, buf_sip_ok, sip_dest_ini.sip_dest_ip, sip_dest_ini.sip_dest_port);

		free_sms(&sms);
		free(trame_sip);
    if(buf_sip_ok){
        free(buf_sip_ok);
    }
	}
	
	sem_post(&mutex_sip);
	return 0;
}

/**
*  \brief This function is used for send all SMS of the SMS send FIFO (SIP)
*/
static void* gestionSIP_send(void *data){
    sem_wait(&mutex_sip);

    if(size_sip>0){
        uint8_t *trame_sip = NULL;
        SMS *sms = (SMS*)calloc(1,sizeof(SMS));
        sms_get(DB_TYPE_SIP,sms);
		
        if(!sms || !(sms->dst) || !(sms->src) || !(sms->msg)){
            ERROR(LOG_SCREEN | LOG_FILE,"SMS failed...");
            free_sms(&sms);
            return 0;
        }

        trame_sip = create_trame_sip_message(sip_dest_ini.sip_dest_ip, sip_dest_ini.sip_dest_port,
                          sip_local_ini.sip_local_ip, sip_local_ini.sip_local_port,
                          sms->src, sms->dst, sms->msg);
		
        if(trame_sip && strlen((char*)trame_sip) > 0){
            sip_send_message(p_sip_socket, trame_sip, sip_dest_ini.sip_dest_ip, sip_dest_ini.sip_dest_port);
            //sms_cls(sms);
            sms_rm(sms);
            size_sip--;
        }else{
            sms_cls(sms);
        }
        free_sms(&sms);
        if(trame_sip){
            free(trame_sip);
        }
    }

    sem_post(&mutex_sip);
    return 0;
}

/**
*  \brief This function is used for managed all input and output SIP trafic
*/
bool warning_sip = true;
static void* gestionSIP(void *data){
    sip_start_connection(p_sip_socket);
    while(running){
        sem_init(&mutex_sip, 0, 1);
        /* initialize mutex to 1 - binary semaphore   */
        /* second param = 0      - semaphore is local */

      	while(p_sip_socket && p_sip_socket->status == SIP_CONNECT){
            warning_sip = true;
        		pthread_t thread_listend;
            pthread_t thread_send;

            pthread_create(&thread_listend,NULL,gestionSIP_listend,NULL);
            pthread_create(&thread_send,NULL,gestionSIP_send,NULL);

            pthread_join(thread_listend,NULL);
            pthread_join(thread_send,NULL);
        }
        
        if(warning_sip){
            WARNING(LOG_SCREEN | LOG_FILE, "sip socket is null")
            warning_sip = false;
        }
        sem_destroy(&mutex_sip);
        usleep(2000);
    }
    return NULL;
}

static void* checkDB(void *data){
/*    while(running){
        if(dbi_conn_ping(conn)==0){
            ERROR(LOG_FILE | LOG_SCREEN, "Reconnect to the DB...");
            printf("%sDBMS             %s: [%s]\n", GREEN, END_COLOR, dbms_ini.dbms_name);
            printf("%sDB dir name      %s: [%s]\n", GREEN, END_COLOR, dbms_ini.db_dirname);
            printf("%sDB base name     %s: [%s]\n", GREEN, END_COLOR, dbms_ini.db_basename);
        }
        sleep(2);
    }*/
    return 0;
}

/**
*  \brief Main funtion
*
*  \param argc This parameter is used to hold the number of arguments that we passed on the command line 
*  \param argv This parameter is an array of char pointer containing the value of arguments
*
*/
int main(int argc,char **argv){
    int c, nofork=1;
    char *conffile = NULL;
    char str[100];
    memset(&str,0,100*sizeof(char));
    log_init("logFile",NULL);
    log2display(LOG_ALERT);

    while((c=getopt(argc, argv, "c:vp:fhD:"))!=-1) {
        switch(c) {
            case 'c':
                    conffile = optarg;
                    break;
            case 'v':
                    printf("sip2smpp version: %s\n", VERSION);
                    exit(0);
                    break;
            case 'P':
                    pid_file = optarg;
                    break;
            case 'f':
                    nofork = 0;
                    log2display(LOG_NONE);
                    break;
            case 'h':
                    usage(0);
                    break;
            case 'D':
                    {
                      char log = atoi(optarg);
                      if(log >= 0 && log <= 8){
                         printf("%d\n",log);
                         log2display((Loglevel)log);
                      }
                      break;
                    }
            default:
                    abort();
        }
    }

    if(!conffile){
        conffile = (char*)malloc(sizeof(char)*strlen(DEFAULT_CONFIG)+1);
        strcpy(conffile,DEFAULT_CONFIG);
    }

    if(FILE *file = fopen(conffile,"r")){
        fclose(file);
    }else{
        ERROR(LOG_FILE | LOG_SCREEN,"The INI file isn't found!");
        handler(-1);
    }

    if(!load_file_ini((uint8_t*)conffile,SECTION_ALL)){
        ERROR(LOG_FILE | LOG_SCREEN,"There are errors in the INI file!");
        handler(-1);
    }

    if(daemonize(nofork) != 0){
        ERROR(LOG_FILE | LOG_SCREEN,"Daemoniize failed");
        exit(-1);
    }

    if(db_init() == -1){
        ERROR(LOG_FILE | LOG_SCREEN,"There are errors when the DB connection!");
        handler(-1);
    }else{
        size_sip  = sms_count(DB_TYPE_SIP);
        size_smpp = sms_count(DB_TYPE_SMPP);
    }

    printf("%sVersion          %s: [%s]\n", GREEN, END_COLOR, VERSION);
    printf("%sPid file         %s: [%s]\n", GREEN, END_COLOR, pid_file);
    printf("-------     %s     -------\n" , conffile);
    printf("%sSIP dest IP      %s: [%s]\n", GREEN, END_COLOR, sip_dest_ini.sip_dest_ip);
    printf("%sSIP dest Port    %s: [%s]\n", GREEN, END_COLOR, sip_dest_ini.sip_dest_port);
    printf("%sSIP Local IP     %s: [%s]\n", GREEN, END_COLOR, sip_local_ini.sip_local_ip);
    printf("%sSIP Local Port   %s: [%s]\n", GREEN, END_COLOR, sip_local_ini.sip_local_port);
    printf("\n");
    printf("%sSMPP Server IP   %s: [%s]\n", GREEN, END_COLOR, smpp_ini.smpp_server_ip);
    printf("%sSMPP Server Port %s: [%s]\n", GREEN, END_COLOR, smpp_ini.smpp_server_port);
    printf("%sCommand ID       %s: [%s]\n", GREEN, END_COLOR, type_bind_smpp_enum_to_str((type_bind_smpp_enum)smpp_ini.command_id));
    printf("%sSystem Type      %s: [%s]\n", GREEN, END_COLOR, smpp_ini.system_type);
    printf("%sTON src          %s: [%d]\n", GREEN, END_COLOR, smpp_ini.ton_src);
    printf("%sTON dst          %s: [%d]\n", GREEN, END_COLOR, smpp_ini.ton_dst);
    printf("%sNPI src          %s: [%d]\n", GREEN, END_COLOR, smpp_ini.npi_src);
    printf("%sNPI dst          %s: [%d]\n", GREEN, END_COLOR, smpp_ini.npi_dst);
    printf("\n");
    printf("%sDBMS             %s: [%s]\n", GREEN, END_COLOR, dbms_ini.dbms_name);
    printf("%sDB path          %s: [%s]\n", GREEN, END_COLOR, dbms_ini.db_path);

    p_sip_socket  = new_sip_socket(sip_local_ini.sip_local_ip, (uint16_t)atoi((char*)sip_local_ini.sip_local_port), (type_bind_sip_enum)SIP_BIND_TRANSCEIVER);
    p_smpp_socket = new_smpp_socket(smpp_ini.smpp_server_ip, (uint16_t)atoi((char*)smpp_ini.smpp_server_port), smpp_ini.user_smpp, smpp_ini.pass_smpp, (type_bind_smpp_enum)smpp_ini.command_id, smpp_ini.system_type, smpp_ini.ton_src, smpp_ini.npi_src, smpp_ini.ton_dst, smpp_ini.npi_dst);

    pthread_t transceiverSIP;
    if(p_sip_socket){
        pthread_create(&transceiverSIP,NULL,gestionSIP,NULL);
    }
    pthread_t transceiverSMPP;
    if(p_smpp_socket){
        pthread_create(&transceiverSMPP,NULL,gestionSMPP,NULL);
    }
    pthread_t checkDBconnection;
    pthread_create(&checkDBconnection,NULL,checkDB,NULL);

//  command line system (not finished)
    while(true){
        scanf("%s",(char*)str);
        if(strcmp((char*)str,(char*)"help") == 0){
            printf( "List of commands :\n"
                    "  help        : display the commands\n"
                    "  sms         : create a SMS to send\n"
                    "  size_list   : display the sizes of list SMS in DB\n"
                    "  reload_sip  : reload SIP\n"
                    "  reload_smpp : reload SMPP\n"
                    "  log         : choice the log level\n"
                    "  shutdown    : exit the program\n");
        }else if(strncmp((char*)str,(char*)"sms",3) == 0){
            int i = 0;
            db_type db_t;
            char src[25]  = "",
                 dst[25]  = "",
                 msg[161] = "",
                 type[5]  = "",
                 how[5]   = "";
            printf("From           : "); scanf("%s",src);
            printf("To             : "); scanf("%s",dst);
            printf("Msg            : "); scanf("%s",msg);
            printf("Type(sip/smpp) : "); scanf("%s",type);
            if(strncmp((char*)type,(char*)"sip",3)==0){
                db_t = DB_TYPE_SIP;
            }else if(strncmp((char*)type,(char*)"smpp",4)==0){
                db_t = DB_TYPE_SMPP;
            }else{
                printf("Type is wrong\n");
                continue;
            }
            printf("How many       : "); scanf("%s",how);
            if(isdigit(how[0])){
                i = atoi(how);
            }else{
                printf("Is not a number\n");
                continue;
            }
            while(i-- > 0){
                sms_set(db_t,(const uint8_t*)&src,(const uint8_t*)&dst,(const uint8_t*)&msg);
                if(db_t == DB_TYPE_SIP){
                    size_sip++;
                }else if(db_t == DB_TYPE_SMPP){
                    size_smpp++;
                }
            }
        }else if(strncmp((char*)str,(char*)"size_list",9) == 0){
            printf("size_smpp : %d", size_smpp);
            printf("size_sip  : %d", size_sip);
        }else if(strncmp((char*)str,(char*)"reload_sip",10) == 0){
            if(!load_file_ini((uint8_t*)conffile,SECTION_SIP) || sip_restart_connection(p_sip_socket) != SIP_ERROR_NO){
                ERROR(LOG_FILE | LOG_SCREEN,"Reload SIP failed! There are errors in the INI file!\n");
            }
        }else if(strncmp((char*)str,(char*)"reload_smpp",11) == 0){
            if(!load_file_ini((uint8_t*)conffile,SECTION_SMPP) || smpp_restart_connection(p_smpp_socket) != SMPP_ERROR_NO){
                ERROR(LOG_FILE | LOG_SCREEN,"Reload SMPP failed! There are errors in the INI file!\n");
            }
        }else if(strncmp((char*)str,(char*)"log",3) == 0){
            char lvl[2] = "";
            printf("log lvl : ");
            scanf("%s", lvl);
            if(isdigit(lvl[0])){
                log2display((Loglevel)atoi(lvl));
            }
        }else if(strncmp((char*)str,(char*)"shutdown",8) == 0){
            break;
        }
        memset(&str,0,100*sizeof(char));
    }//End While
	
    running = 0;

    pthread_join(transceiverSMPP,NULL);
    pthread_join(transceiverSIP,NULL);
    pthread_join(checkDBconnection,NULL);

    free_file_ini(SECTION_ALL);

    handler(0);
    return 0;
}
