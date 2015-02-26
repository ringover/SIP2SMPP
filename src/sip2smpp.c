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
#include "smpp_io.h"
//SIP
#include "sip_io.h"
#include "sip_create.h"
//INI FILE
#include "ini/iniFile.h"
//LOG
#include "log/log.h"
//OTHER
//#include "type_projet.h"
#include "sms_struct.h"
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
sip_socket  *p_sip_socket  = NULL;
smpp_socket *p_smpp_socket = NULL;

/**
 * count the total number of sms in the DB
 */
unsigned int count_sms = 0;

/**
 * routing thread
 */
static void* routing_thread(void *data){
    long long int *id          = (long long int*)((void**)data)[0];
    unsigned char *interface   = (unsigned char*)((void**)data)[1];
    unsigned char *ip_origin   = (unsigned char*)((void**)data)[2];
    unsigned int  *port_origin = (unsigned int*)((void**)data)[3];
    unsigned char *msisdn_src  = (unsigned char*)((void**)data)[4];
    unsigned char *msisdn_dst  = (unsigned char*)((void**)data)[5];
    unsigned char *message     = (unsigned char*)((void**)data)[6];
    db_status     *status      = (db_status*)((void**)data)[7];
    unsigned int  *ttl         = (unsigned int*)((void**)data)[8];
    int res = -1;
    //Update DB : PENDING -> ONGOING
    if(*status == PENDING){
        db_update_sms_status_by_id(*id, ONGOING);
    }
    //Routing SMS
    //if(p_fun_routing != NULL){
        //res = p_fun_routing(interface, ip_origin, port_origin, msisdn_src, msisdn_dst, message);
    //}else{
        //res = routing_default(interface, ip_origin, port_origin, msisdn_src, msisdn_dst, message);
    //}
    res = routing(interface, ip_origin, *port_origin, msisdn_src, msisdn_dst, message);

    //ERROR(LOG_SCREEN, "&&&&&&&&&&&&&&&&&&&&&&&&&&routing res = %d - id = %d", res, id)

    //Update DB : DELETE SMS if sent else --TTL
    if(res > 0){
        db_delete_sms_by_id(*id);
        count_sms--;
    }else{
        db_update_sms_ttl_by_id(*id, --(*ttl)); 
    }
    if(id){
        free(id);
        id = NULL;
    }
    if(interface){
        free(interface);
        interface = NULL;
    }
    if(ip_origin){
        free(ip_origin);
        ip_origin = NULL;
    }
    if(port_origin){
        free(port_origin);
        port_origin = NULL;
    }
    if(msisdn_src){
        free(msisdn_src);
        msisdn_src = NULL;
    }
    if(msisdn_dst){
        free(msisdn_dst);
        msisdn_dst = NULL;
    }
    if(message){
        free(message);
        message = NULL;
    }
    if(status){
        free(status);
        status = NULL;
    }
    if(ttl){
        free(ttl);
        ttl = NULL;
    }
    if(data){
        free(data);
        data = NULL;
    }
    return;
}


/**
*  Thread SMPP
*/

pthread_t listen_smpp;

/**
*  \brief This function is used for transfer all SMS received to the DB (SMPP->SIP)
*/
static void* func_listen_smpp(void *data){
    smpp_socket   *p_socket      = (smpp_socket*)data;
    unsigned int   port_origin   = 0;
    long long int  id            = 0;
    long long int *p_id          = NULL;
    unsigned char *p_interface   = NULL;
    unsigned char *p_ip_origin   = NULL;
    unsigned int  *p_port_origin = NULL;
    unsigned char *p_from_msisdn = NULL;
    unsigned char *p_to_msisdn   = NULL;
    unsigned char *p_msg         = NULL;
    db_status     *p_status      = NULL;
    unsigned int  *p_ttl         = NULL;
    void         **p_t_data      = NULL;

    if(p_socket->status == SMPP_DISCONNECT){
        smpp_start_connection(p_socket);
    }

    while(p_socket->status == SMPP_CONNECT){
        smpp_receive_sms(p_socket, &p_from_msisdn, &p_to_msisdn, &p_msg);//&p_ip_origin, &port_origin);
        //TMP
        p_ip_origin = (unsigned char*)calloc(25, sizeof(char));
        strcpy(p_ip_origin,"not_implemented(smpp)");
        port_origin = 5060;
        //END TMP
        if(p_from_msisdn && p_to_msisdn && p_msg && p_ip_origin && port_origin > 0){
            display_sms(p_from_msisdn, p_to_msisdn, p_msg);//, p_ip_origin, port_origin);
            id = db_insert_sms(p_socket->interface_name, p_ip_origin, port_origin, p_from_msisdn, p_to_msisdn, p_msg, PENDING, 5);
            if(id <= 0){
                ERROR(LOG_SCREEN | LOG_FILE, "DB Error")
            }
            count_sms++;
            //create tableau data**
            p_t_data = (void**)calloc(9, sizeof(void*));
            p_id     = (long long int*)calloc(1, sizeof(long long int));
            *p_id    = id;
            p_t_data[0] = (void*)p_id;
            p_interface = (unsigned char*)calloc(strlen((char*)p_socket->interface_name), sizeof(char));
            strcpy((char*)p_interface, (char*)p_socket->interface_name);
            p_t_data[1] = (void*)p_interface; 
            p_t_data[2] = (void*)p_ip_origin; 
            p_port_origin  = (unsigned int *)calloc(1, sizeof(int));
            *p_port_origin = port_origin; 
            p_t_data[3] = (void*)p_port_origin; 
            p_t_data[4] = (void*)p_from_msisdn;
            p_t_data[5] = (void*)p_to_msisdn;
            p_t_data[6] = (void*)p_msg;
            p_status  = (unsigned int *)calloc(1, sizeof(int));
            *p_status = PENDING;
            p_t_data[7] = (void*)p_status;
            p_ttl   = (db_status*)calloc(1, sizeof(db_status));
            *p_ttl  = 5;
            p_t_data[8] = (void*)p_ttl;
            //create routing thread 
            //pthread_create();
            threadpool_add(p_threadpool, routing_thread, p_t_data, 0);
        }else{
            if(p_from_msisdn) free(p_from_msisdn);
            if(p_to_msisdn) free(p_to_msisdn);
            if(p_msg) free(p_msg);
            if(p_ip_origin) free(p_ip_origin);
        }
        p_id          = NULL;
        p_interface   = NULL;
        p_ip_origin   = NULL;
        p_port_origin = NULL;
        p_from_msisdn = NULL;
        p_to_msisdn   = NULL;
        p_msg         = NULL;
        data          = NULL;
    }
    return 0;
}

/**
*  Thread SIP
*/

pthread_t listen_sip;

/**
*  \brief This function is used for transfer all SMS received to the SMS listend FIFO (SIP)
*/
static void* func_listen_sip(void *data){
    sip_socket *p_socket = (sip_socket*)data;
    unsigned int   port_origin   = 0;
    long long int  id            = 0;
    long long int *p_id          = NULL;
    unsigned char *p_interface   = NULL;
    unsigned char *p_ip_origin   = NULL;
    unsigned int  *p_port_origin = NULL;
    unsigned char *p_from_msisdn = NULL;
    unsigned char *p_to_msisdn   = NULL;
    unsigned char *p_msg         = NULL;
    db_status     *p_status      = NULL;
    unsigned int  *p_ttl         = NULL;
    void         **p_t_data      = NULL;
    extern map    *map_str_sip;
    extern map    *map_str_smpp;
    
    if(p_socket->status == SIP_DISCONNECT){
        sip_start_connection(p_socket);
    }

    while(p_socket->status == SMPP_CONNECT){
        sip_receive_sms(p_socket, &p_from_msisdn, &p_to_msisdn, &p_msg);//&p_ip_origin, &port_origin);
        //TMP
        p_ip_origin = (unsigned char*)calloc(25, sizeof(char));
        strcpy(p_ip_origin,"not_implemented(smpp)");
        port_origin = 5060;
        //END TMP
        
        if(p_from_msisdn && p_to_msisdn && p_msg && p_ip_origin && port_origin > 0){
            display_sms(p_from_msisdn, p_to_msisdn, p_msg);//, p_ip_origin, port_origin);
            id = db_insert_sms(p_socket->interface_name, p_ip_origin, port_origin, p_from_msisdn, p_to_msisdn, p_msg, PENDING, 5);
            if(id <= 0){
                ERROR(LOG_SCREEN | LOG_FILE, "DB Error")
            }
            count_sms++;
            //create tableau data**
            p_t_data    = (void**)calloc(9, sizeof(void*));
            p_id    = (long long int*)calloc(1, sizeof(long long int));
            *p_id   = id;
            p_t_data[0] = (void*)p_id;
            p_interface = (unsigned char*)calloc(strlen((char*)p_socket->interface_name), sizeof(char));
            strcpy((char*)p_interface, (char*)p_socket->interface_name);
            p_t_data[1] = (void*)p_interface; 
            p_t_data[2] = (void*)p_ip_origin; 
            p_port_origin  = (unsigned int *)calloc(1, sizeof(int));
            *p_port_origin = port_origin; 
            p_t_data[3] = (void*)p_port_origin; 
            p_t_data[4] = (void*)p_from_msisdn;
            p_t_data[5] = (void*)p_to_msisdn;
            p_t_data[6] = (void*)p_msg;
            p_status  = (unsigned int *)calloc(1, sizeof(int));
            *p_status = PENDING;
            p_t_data[7] = (void*)p_status;
            p_ttl   = (db_status*)calloc(1, sizeof(db_status));
            *p_ttl  = 5;
            p_t_data[8] = (void*)p_ttl;
            //create routing thread 
            //pthread_create();
            threadpool_add(p_threadpool, routing_thread, p_t_data, 0);
        }else{
            if(p_from_msisdn) free(p_from_msisdn);
            if(p_to_msisdn) free(p_to_msisdn);
            if(p_msg) free(p_msg);
            if(p_ip_origin) free(p_ip_origin);
        }
        p_id          = NULL;
        p_interface   = NULL;
        p_ip_origin   = NULL;
        p_port_origin = NULL;
        p_from_msisdn = NULL;
        p_to_msisdn   = NULL;
        p_msg         = NULL;
        data          = NULL;
    }
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
    FILE *file = NULL;
    char *conffile = NULL;
    char str[100];
    memset(&str,0,100*sizeof(char));
    log_init("logFile",NULL);
    log2display(LOG_ALERT);
   
    p_threadpool = threadpool_create(5, 8096, 0);
 
    map_str_sip  = new_map(free_string, copy_string, compare_string, free_sip_socket, copy_sip_socket, compare_sip_socket);
    map_str_smpp = new_map(free_string, copy_string, compare_string, free_smpp_socket, copy_smpp_socket, compare_smpp_socket);

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

    p_sip_socket  = new_sip_socket(sip_host_ini.interface_name, sip_host_ini.sip_ip, (unsigned int)sip_host_ini.sip_port);
    p_smpp_socket = new_smpp_socket(smpp_ini.interface_name, smpp_ini.smpp_ip, (unsigned int)smpp_ini.smpp_port, smpp_ini.user_smpp, smpp_ini.pass_smpp, /*smpp_ini.command_id*/BIND_TRANSCEIVER, smpp_ini.system_type, smpp_ini.ton_src, smpp_ini.npi_src, smpp_ini.ton_dst, smpp_ini.npi_dst);
    
    map_set(map_str_sip, sip_host_ini.interface_name, p_sip_socket);
    map_set(map_str_smpp, smpp_ini.interface_name, p_smpp_socket);

    if(p_sip_socket){
        pthread_create(&listen_sip, NULL, func_listen_sip, p_sip_socket);
    }
    if(p_smpp_socket){
        pthread_create(&listen_smpp, NULL, func_listen_smpp, p_smpp_socket);
    }
    
//    pthread_t checkDBconnection;
//    pthread_create(&checkDBconnection,NULL,checkDB,NULL);

//  command line system (not finished)
    while(true){
        scanf("%s",(char*)str);
        if(strcmp((char*)str,(char*)"help") == 0){
            printf( "List of commands :\n"
                    "  help        : display the commands\n"
                    "  size_list   : display the sizes of list SMS in DB\n"
                    "  reload_sip  : reload SIP\n"
                    "  reload_smpp : reload SMPP\n"
                    "  log         : choice the log level\n"
                    "  shutdown    : exit the program\n");
        }else if(strncmp((char*)str,(char*)"size_list",9) == 0){
            printf("count_sms  : %d", count_sms);
        }else if(strncmp((char*)str,(char*)"reload_sip",10) == 0){
            if(!load_file_ini((uint8_t*)conffile,SECTION_SIP) || sip_restart_connection(p_sip_socket) == -1 ){
                ERROR(LOG_FILE | LOG_SCREEN,"Reload SIP failed! There are errors in the INI file!\n");
            }
        }else if(strncmp((char*)str,(char*)"reload_smpp",11) == 0){
            if(!load_file_ini((uint8_t*)conffile,SECTION_SMPP) || smpp_restart_connection(p_smpp_socket) == -1){
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

    pthread_join(listen_sip,NULL);
    pthread_join(listen_smpp,NULL);

    threadpool_destroy(p_threadpool, threadpool_graceful);
//    pthread_join(checkDBconnection,NULL);

    free_file_ini(SECTION_ALL);

    close_routing();

    handler(0);
    return 0;
}
