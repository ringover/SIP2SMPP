/**
*   \file main.cpp
*
*   \brief The main file of the SIP2SMPP project
*
*/

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
//THREAD - SEMAPHORE
#include <pthread.h>
#include <semaphore.h>
//MAP & LIST
#include "linked_list/map.h"
//Thread Pool
#include "threadpool/threadpool.h"
//#include "linked_list/list.h"
//SMPP
#include "net/smpp/smpp.h"
#include "smpp_io.h"
//SIP
#include "net/sip/sip.h"
#include "sip_io.h"
//INI FILE
#include "ini/iniFile.h"
//LOG
#include "log/log.h"
//OTHER
//#include "type_projet.h"
#include "sm_struct.h"
#include "routing.h"
#include "database.h"
#include "daemonize/daemonize.h"

threadpool_t *p_threadpool = NULL;

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
 *  Init Connection
 */
sip_socket_t  *p_sip_socket  = NULL;
smpp_socket_t *p_smpp_socket = NULL;

/**
*  Thread SMPP
*/

pthread_t listen_smpp;

/**
*  \brief This function is used for transfer all SMS received to the DB (SMPP->SIP)
*/
static void* func_listen_smpp(void *data){
    smpp_socket_t *p_socket = (smpp_socket_t*)data;

    smpp_start_connection(p_socket);

    while(smpp_engine(p_socket) != -1);
    return NULL;
}

/**
*  Thread SIP
*/

pthread_t listen_sip;

/**
*  \brief This function is used for transfer all SMS received to the SMS listend FIFO (SIP)
*/
static void* func_listen_sip(void *data){
    sip_socket_t *p_socket = (sip_socket_t*)data;

    sip_start_connection(p_socket);

    while(sip_engine(p_socket) != -1);
	  return 0;
}

void init_maps(){
    //map used for save the state of a transation
    map_session_smpp = new_map(free_uint32, NULL, compare_uint32, free_smpp_session, NULL, NULL);
    map_session_sip  = new_map(free_string, NULL, compare_string, free_sip_session,  NULL, NULL);
    //map used for save all sm in transation in memory
    map_sm           = new_map(free_uint32, NULL, compare_uint32, free_sm_data, NULL, NULL);
    //map used for save all interface client/server
    map_iface_sip    = new_map(free_string, NULL, compare_string, free_sip_socket, NULL, NULL);
    map_iface_smpp   = new_map(free_string, NULL, compare_string, free_smpp_socket, NULL, NULL);
}

int main(int argc,char **argv){
    int c, nofork=1;
    FILE *file = NULL;
    char *conffile = NULL;
    char str[100];
    memset(&str, 0, 100*sizeof(char));
    log_init("logFile",NULL);
    log2display(LOG_ALERT);
   
    p_threadpool = threadpool_create(5, 8096, 0);

    init_maps();
    init_call_id(NULL);
 
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

    if((file = fopen(conffile,"r")) != NULL){
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
        //db_select_sms_count(PENDING, &count_sms);
    }

    if(start_routing() != 0){
        ERROR(LOG_FILE | LOG_SCREEN, "Routing loading failed");
    }

    printf("%sVersion          %s: [%s]\n", GREEN, END_COLOR, VERSION);
    printf("%sPid file         %s: [%s]\n", GREEN, END_COLOR, pid_file);
    printf("-------     %s     -------\n" , conffile);
    printf("%sSIP remote name  %s: [%s]\n", GREEN, END_COLOR, sip_remote_ini.interface_name);
    printf("%sSIP remote IP    %s: [%s]\n", GREEN, END_COLOR, sip_remote_ini.sip_ip);
    printf("%sSIP remote Port  %s: [%d]\n", GREEN, END_COLOR, sip_remote_ini.sip_port);
    printf("%sSIP remote model %s: [%d]\n", GREEN, END_COLOR, sip_remote_ini.model);
    printf("%sSIP host name    %s: [%s]\n", GREEN, END_COLOR, sip_host_ini.interface_name);
    printf("%sSIP host IP      %s: [%s]\n", GREEN, END_COLOR, sip_host_ini.sip_ip);
    printf("%sSIP host Port    %s: [%d]\n", GREEN, END_COLOR, sip_host_ini.sip_port);
    printf("%sSIP host model   %s: [%d]\n", GREEN, END_COLOR, sip_host_ini.model);
    printf("\n");
    printf("%sSMPP name        %s: [%s]\n", GREEN, END_COLOR, smpp_ini.interface_name);
    printf("%sSMPP IP          %s: [%s]\n", GREEN, END_COLOR, smpp_ini.smpp_ip);
    printf("%sSMPP Port        %s: [%d]\n", GREEN, END_COLOR, smpp_ini.smpp_port);
    printf("%sSMPP model       %s: [%d]\n", GREEN, END_COLOR, smpp_ini.model);
//    printf("%sCommand ID       %s: [%s]\n", GREEN, END_COLOR, type_bind_smpp_enum_to_str((type_bind_smpp_enum)smpp_ini.command_id));
    printf("%sSystem Type      %s: [%s]\n", GREEN, END_COLOR, smpp_ini.system_type);
    printf("%sTON src          %s: [%d]\n", GREEN, END_COLOR, smpp_ini.ton_src);
    printf("%sTON dst          %s: [%d]\n", GREEN, END_COLOR, smpp_ini.ton_dst);
    printf("%sNPI src          %s: [%d]\n", GREEN, END_COLOR, smpp_ini.npi_src);
    printf("%sNPI dst          %s: [%d]\n", GREEN, END_COLOR, smpp_ini.npi_dst);
    printf("\n");
    printf("%sDBMS             %s: [%s]\n", GREEN, END_COLOR, dbms_ini.dbms_name);
    printf("%sDB path          %s: [%s]\n", GREEN, END_COLOR, dbms_ini.db_path);

    p_sip_socket  = new_sip_socket_t();
    init_sip_socket_t(p_sip_socket, sip_host_ini.interface_name, sip_host_ini.sip_ip, (unsigned int)sip_host_ini.sip_port);

    p_smpp_socket = new_smpp_socket_t();
    init_smpp_socket_t(p_smpp_socket, smpp_ini.interface_name, NULL, 0, smpp_ini.smpp_ip, (unsigned int)smpp_ini.smpp_port, smpp_ini.user_smpp, smpp_ini.pass_smpp, /*smpp_ini.command_id*/BIND_TRANSCEIVER, smpp_ini.system_type, smpp_ini.ton_src, smpp_ini.npi_src, smpp_ini.ton_dst, smpp_ini.npi_dst);
    
    map_set(map_iface_sip, sip_host_ini.interface_name, p_sip_socket);
    map_set(map_iface_smpp, smpp_ini.interface_name, p_smpp_socket);

    if(p_sip_socket){
        pthread_create(&listen_sip, NULL, func_listen_sip, p_sip_socket);
    }
    if(p_smpp_socket){
        pthread_create(&listen_smpp, NULL, func_listen_smpp, p_smpp_socket);
    }
   
    while(true);
 
    pthread_join(listen_sip,NULL);
    pthread_join(listen_smpp,NULL);

    threadpool_destroy(p_threadpool, threadpool_graceful);
    free_file_ini(SECTION_ALL);
    close_routing();
    handler(0);
    return 0;
}
