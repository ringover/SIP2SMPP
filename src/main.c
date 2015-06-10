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
#include <dlfcn.h>
//THREAD - SEMAPHORE
#include <pthread.h>
#include <semaphore.h>
//MAP & LIST
#include "linked_list/map.h"
#include "linked_list/keys.h"
//Thread Pool
#include "threadpool/threadpool.h"
//SMPP
#include "net/smpp/smpp.h"
#include "smpp_io.h"
//SIP
#include "net/sip/sip.h"
#include "sip_io.h"
//INI FILE
#include "config/config.h"
//LOG
#include "log/log.h"
//OTHER
//#include "type_projet.h"
#include "sm_struct.h"
#include "routing_def.h"
#include "routing.h"
#include "database.h"
#include "daemonize/daemonize.h"

threadpool_t *p_threadpool = NULL;

int running = 1;
char* pid_file = (char*)DEFAULT_PIDFILE;

func_start_routing f_start_routing = NULL;
func_routing f_routing = NULL;
func_close_routing f_close_routing = NULL;

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
    printf("%s    -f  %s: use fork\n", CYAN, END_COLOR);
    printf("%s    -P  %s: PID file. Default PID file is [%s]\n", CYAN, END_COLOR, DEFAULT_PIDFILE);
    printf("%s    -c  %s: config file to use to specify some options. Default location is [%s]\n", CYAN, END_COLOR, DEFAULT_CONFIG);
    printf("%s    -l  %s: log file to use to specify some options. Default location is [%s]\n", CYAN, END_COLOR, DEFAULT_CONFIG);

    handler(value);
}

/**
 *  Init Connection
 */
config_sip_t  *p_config_sip  = NULL;
config_smpp_t *p_config_smpp = NULL;

/**
*  Thread SMPP
*/

/**
*  \brief This function is used for transfer all SMS received to the DB (SMPP->SIP)
*/
static void* func_listen_smpp(void *data){
/*
    config_smpp_t *p_socket = (config_smpp_t*)data;
    smpp_start_connection(p_socket);
    while(smpp_engine(p_socket) > 0);
    return NULL;
*/
    char cpt = 1;
    config_smpp_t *p_socket = (config_smpp_t*)data;

    while(cpt <= 60){
        if(smpp_start_connection(p_socket) != -1){
            cpt = 1;
            while(smpp_engine(p_socket) > 0);
        }
        sleep(cpt++);
        smpp_end_connection(p_socket);
    }
    return NULL;
}

/**
*  Thread SIP
*/

/**
*  \brief This function is used for transfer all SMS received to the SMS listend FIFO (SIP)
*/
static void* func_listen_sip(void *data){
    config_sip_t *p_socket = (config_sip_t*)data;

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
    //map used for sar msg
    map_sar_msg      = new_map(free_uint16, NULL, compare_uint16, free_tab_string, NULL, NULL);
}

map *m_threads = NULL;

void start_all_threads_interfaces(){
    if(!m_threads){
        m_threads = new_map(free_string, NULL, compare_string, free_uli, NULL, compare_uli);
    }
    if(cfg_sip){
        iterator_map *p_it = cfg_sip->begin;
        while(p_it){
            config_sip_t *sip = p_it->value;
            char *name = calloc(sizeof(sip->name)+1, sizeof(char));
            strcpy(name, sip->name);
            pthread_t *p_thread = calloc(1, sizeof(pthread_t));
            pthread_create(p_thread, NULL, func_listen_sip, sip);
            map_set(m_threads, name, p_thread);
            p_it = p_it->next;
        } 
    }
    if(cfg_smpp){
        iterator_map *p_it = cfg_smpp->begin;
        while(p_it){
            config_smpp_t *smpp = p_it->value;
            char *name = calloc(sizeof(smpp->name)+1, sizeof(char));
            strcpy(name, smpp->name);
            pthread_t *p_thread = calloc(1, sizeof(pthread_t));
            pthread_create(p_thread, NULL, func_listen_smpp, smpp);
            map_set(m_threads, name, p_thread);
            p_it = p_it->next;
        } 
    }
    return;
}

void join_all_threads(){
    if(m_threads){
        iterator_map *p_it = m_threads->begin;
        while(p_it){
            pthread_t *p_thread = p_it->value;
            pthread_join(*p_thread,NULL);
            map_erase(m_threads, p_it->key);
            p_it = m_threads->begin;
        } 
    }
    map_destroy(&m_threads);
    return;
} 


int main(int argc, char **argv){
    void *mod_routing = NULL;
    char *conffile = NULL;
    FILE *file = NULL;
    int nofork = 1;
    char str[100];
    int c = 0;
    char b_log  = 1;
    char b_fork = 1;
    memset(&str, 0, 100*sizeof(char));
    log_init("logFile",NULL);
    log2display(LOG_ALERT);
   
    p_threadpool = threadpool_create(5, 8096, 0);

    init_maps();
    init_call_id(NULL);
 
    while((c=getopt(argc, argv, "c:vP:fhD:"))!=-1) {
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
                    b_fork = 0;
                    break;
            case 'h':
                    usage(0);
                    break;
            case 'D':
                    {
                      char log = atoi(optarg);
                      if(log >= 0 && log <= 8){
                         log2display((Loglevel)log);
                         b_log = 0;
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

    if(load_config_file((uint8_t*)conffile, CONFIG_ALL, NULL) == -1){
        ERROR(LOG_FILE | LOG_SCREEN,"There are errors in the INI file!");
        free_config_file(CONFIG_ALL, NULL);
        handler(-1);
    }

    if(b_log){
        log2display((Loglevel)cfg_main->log_level);
    }

    if(b_fork){
        nofork = !cfg_main->fork;
    }

    if(daemonize(nofork) != 0){
        ERROR(LOG_FILE | LOG_SCREEN,"Daemoniize failed");
        exit(-1);
    }

    //Load routing module
    void* functions[2] = { send_sms_to_smpp, send_sms_to_sip };
    void* cfgs[2] = { cfg_smpp, cfg_sip };
    if(cfg_main->routing_module){
        mod_routing = dlopen(cfg_main->routing_module, RTLD_NOW | RTLD_GLOBAL);
        if(!mod_routing){
            ERROR(LOG_SCREEN | LOG_FILE, "%s", dlerror());
            handler(-1);
        }
        f_start_routing = dlsym(mod_routing, "start_routing");
        f_routing       = dlsym(mod_routing, "routing");
        f_close_routing = dlsym(mod_routing, "close_routing");
    }else{
        f_start_routing = default_start_routing;
        f_routing       = default_routing;
        f_close_routing = default_close_routing;
    }

    if(db_init() == -1){
        ERROR(LOG_FILE | LOG_SCREEN,"There are errors when the DB connection!");
        handler(-1);
    }else{
        //TODO: send sms saved in db
    }

    if(f_start_routing(functions, cfgs) != 0){
        ERROR(LOG_FILE | LOG_SCREEN, "Routing loading failed");
        handler(-1);
    }

    if(cfg_main && cfg_main->launch_msg){
        printf("\033[0;36m%s\033[0m\n", cfg_main->launch_msg);
    }

    printf("SIP 2 SMPP Version  [%s]\n", VERSION);
    printf("Pid file            [%s]\n", pid_file);
    printf("Config File         [%s]\n", conffile);

    display_config_file(CONFIG_ALL, NULL);

    start_all_threads_interfaces();

    join_all_threads();  

    threadpool_destroy(p_threadpool, threadpool_graceful);

    f_close_routing();

    if(cfg_main->routing_module){
       dlclose(mod_routing); 
    }

    free_config_file(CONFIG_ALL, NULL);

    handler(0);

    return 0;
}


