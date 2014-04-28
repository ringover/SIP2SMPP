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
//#include <thread>
#include <queue>

#include <pthread.h>

#include <sys/cdefs.h>
#include <sys/types.h>
#include <sys/socket.h>

#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <time.h>
#include <getopt.h>
#include <sstream>

#include <semaphore.h>

#include "connectionSMPP.h"
#include "connectionSIP.h"

//#include "type_projet.h"
#include "ini/iniFile.h"
#include "log/log.h"
#include "smpp/structSMPP.h"
#include "database.h"
#include "daemonize/daemonize.h"

using namespace std;

int running = 0;
char* pid_file = (char*)DEFAULT_PIDFILE;

/**
*  \brief This function is a signal handler function
*
*  \param value This parameter is a signal number
*
*/
void handler(int value){
    ERROR(LOG_FILE,"The process has been terminated");
    CONSOLE(LOG_SCREEN,"%sEnd\n%s",RED,NONE);
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
    printf("%sHelp :%s\n",RED,NONE);

    printf("Sip2Smpp [-f file.conf] [option -P file.pid] \n");
    printf("   -h  help\n");
    printf("   -v  show version\n");
    printf("   -f  use fork (parameter 1) | Not implemented\n");
    //printf("   -P  PID file. Default PID file is [%s]\n",DEFAULT_PIDFILE);
    printf("   -c  config file to use to specify some options. Default location is [%s]\n",DEFAULT_CONFIG);
    printf("   -l  log file to use to specify some options. Default location is [%s]\n",DEFAULT_CONFIG);
    printf("%s",NONE);

    handler(value);
}

/**
*  CONVERSIONS :
*    - trame SIP -> struct SMS
*/

#include "parseSip.h"
#include "createMessageSip.h"

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

        SMS *sms = new SMS();
        sms->src = (char*)malloc(sizeof(char)*21);
        sms->dst = (char*)malloc(sizeof(char)*21);
        sms->msg = (char*)malloc(sizeof(char)*256);
        sms->src = (char*)from.get_user_name().c_str();
        sms->dst = (char*)to.get_user_name().c_str();
        sms->msg = (char*)explode[explode.size()-1].c_str();

        return sms;
}

/**
*  Init Connection
*/
Connection_SMPP *smpp = NULL;
Connection_SIP  *sip = NULL;

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
		SMS *sms = (SMS*)malloc(sizeof(SMS));
		memset(sms,0,sizeof(SMS));
		
		sms_get(DB_TYPE_SMPP,sms);
		if(sms){
			smpp->sendSMS(*sms);
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
	SMS *sms = NULL;
	sem_wait(&mutex_smpp);
	
	sms = smpp->receiverSMS(true);
	if(sms && (sms->dst) && (sms->src) && (sms->msg)){
		Connection_SMPP::displaySMS(*sms);
		sms_set(DB_TYPE_SIP,sms->src,sms->dst,sms->msg);
		size_sip++;
	}	
	free_sms(&sms);

	sem_post(&mutex_smpp);
	return 0;
}

/**
*  \brief This function is used for managed all input and output SMPP trafic
*/
static void* gestionSMPP(void *data){
	smpp = new Connection_SMPP(mainIni.smpp_server_ip,mainIni.smpp_server_port,
					mainIni.user_smpp,mainIni.pass_smpp,BIND_TRANSCEIVER,true);
 	sem_init(&mutex_smpp, 0, 1);
	
	while(smpp && smpp->connect && running){
		pthread_t thread_listend;
		pthread_t thread_send;

		pthread_create(&thread_listend,NULL,gestionSMPP_listend,NULL);
		pthread_create(&thread_send,NULL,gestionSMPP_send,NULL);
		
		pthread_join(thread_listend,NULL);
		pthread_join(thread_send,NULL);
	}

	if(smpp){
		delete smpp;
		smpp = NULL;
	}
	
	sem_destroy(&mutex_smpp);
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
	sem_wait(&mutex_sip);
	
	char* str = sip->receiveSIP();
	if(str){
		SMS *sms = sip2sms(str);
		if(sms && (sms->dst) && (sms->src) && (sms->msg)){
			sms_set(DB_TYPE_SMPP,sms->src,sms->dst,sms->msg);
			size_smpp++;
		}
		free_sms(&sms);
		free(str);        
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
		SMS *sms = (SMS*)malloc(sizeof(SMS));
		memset(sms,0,sizeof(SMS));
		sms_get(DB_TYPE_SIP,sms);
		
		if(!sms || !(sms->dst) || !(sms->src) || !(sms->msg)){
    			ERROR(LOG_SCREEN | LOG_FILE,"SMS failed...");
			free_sms(&sms);
			return 0;
		}
		
		string str = createTrameSipSMS(mainIni.sip_dest_ip,mainIni.sip_dest_port,
				mainIni.sip_local_ip,mainIni.sip_local_port,sms->dst,sms->src,sms->msg);
		
		if(str.size()>0){
			//cout << "mainIni.sip_dest_ip   = " << mainIni.sip_dest_ip << endl;
			//cout << "mainIni.sip_dest_port = " << mainIni.sip_dest_port << endl;
			sip->sendSIP(str,mainIni.sip_dest_ip,mainIni.sip_dest_port);
			//sms_cls(sms);
			sms_rm(sms);
			size_sip--;
		}else{
			sms_cls(sms);
		}
		free_sms(&sms);
	}
	
	sem_post(&mutex_sip);
	return 0;
}

/**
*  \brief This function is used for managed all input and output SIP trafic
*/
static void* gestionSIP(void *data){
	sip = new Connection_SIP(mainIni.sip_local_ip,mainIni.sip_local_port,SIP_TRANSCEIVER,true);
 	sem_init(&mutex_sip, 0, 1);
	/* initialize mutex to 1 - binary semaphore   */
	/* second param = 0      - semaphore is local */

	while( sip && sip->connect && running ){
		pthread_t thread_listend;
                pthread_t thread_send;

                pthread_create(&thread_listend,NULL,gestionSIP_listend,NULL);
                pthread_create(&thread_send,NULL,gestionSIP_send,NULL);

                pthread_join(thread_listend,NULL);
                pthread_join(thread_send,NULL);
	}

	if(sip){
		delete sip;
		sip = NULL;
	}

	sem_destroy(&mutex_sip);
	return 0;
}

static void* checkDB(void *data){
	while(running){
		if(dbi_conn_ping(conn)==0){
			ERROR(LOG_FILE | LOG_SCREEN, "Reconnect to the DB...");
		}
		sleep(2);
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
    char *conffile = NULL;
    log_init("logFile",NULL);

    CONSOLE(LOG_SCREEN,"%sStart\n%s",RED,NONE);

    while((c=getopt(argc, argv, "c:vp:fh"))!=-1) {//: quand il y a un paramettre
        switch(c) {
            case 'c':
                    conffile = optarg;
                    break;
            case 'v':
                    printf("version: %s\n", VERSION);
                    exit(0);
                    break;
            case 'P':
                    //pid_file = optarg;
                    break;
            case 'f':
                    nofork = 0;
                    log2display(LOG_NONE);
                    break;
            case 'h':
                    usage(0);
                    break;
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

    if(!loadFileIni(conffile)){
        ERROR(LOG_FILE | LOG_SCREEN,"There are errors in the INI file!");
        handler(-1);
    }

    if(db_init() == -1){
	ERROR(LOG_FILE | LOG_SCREEN,"There are errors when the DB connection!");
	handler(-1);
    }else{
	size_sip  = sms_count(DB_TYPE_SIP);
	size_smpp = sms_count(DB_TYPE_SMPP);
    }


    if(daemonize(nofork) != 0){
        ERROR(LOG_FILE | LOG_SCREEN,"Daemoniize failed");
        exit(-1);
    }


    printf("Version          : [%s]\n", VERSION);
    printf("INI file         : [%s]\n", conffile);
    printf("------------------\n", conffile);
    printf("SIP dest IP      : [%s]\n", mainIni.sip_dest_ip);
    printf("SIP dest Port    : [%s]\n", mainIni.sip_dest_port);
    printf("SIP Local IP     : [%s]\n", mainIni.sip_local_ip);
    printf("SIP Local Port   : [%s]\n", mainIni.sip_local_port);

    printf("SMPP Server IP   : [%s]\n", mainIni.smpp_server_ip);
    printf("SMPP Server Port : [%s]\n", mainIni.smpp_server_port);
//  printf("Pid file         : [%s]\n", pid_file);
    printf("DBMS             : [%s]\n", dbmsIni.dbms_name);
    printf("DB dir name      : [%s]\n", dbmsIni.db_dirname);
    printf("DB base name     : [%s]\n", dbmsIni.db_basename);

//  pthread_t transceiverSMPP;
    pthread_t transceiverSIP;
    pthread_t checkDBconnection;

//  pthread_create(&transceiverSMPP,NULL,gestionSMPP,NULL);
    pthread_create(&transceiverSIP,NULL,gestionSIP,NULL);
    pthread_create(&checkDBconnection,NULL,checkDB,NULL);

//  command line system (not finished)
    string str = "";
    while(str != "shutdown"){
	cin >> str;
	if(str == "help"){
		cout << "List of commands :"                                       << endl;
		cout << "  help        : display the commands"                     << endl;
		cout << "  sms         : create a SMS to send"                     << endl;
		cout << "  size_list   : display the sizes of list SMS"            << endl;
		cout << "  reload sip  : NOT IMPLEMENTED : reload sip connection"  << endl;
		cout << "  reload smpp : NOT IMPLEMENTED : reload smpp connection" << endl;
		cout << "  reload conf : NOT IMPLEMENTED : reload the INI file"    << endl;
		cout << "  loglvl      : NOT IMPLEMENTED : choice the log level"   << endl;
		cout << "  shutdown    : exit the program"                         << endl;
	}
	if(str == "sms"){
		short i = 1;
		char src[25], dst[25], msg[160], type[5];
		cout << "From           : ";  cin >> src;
		cout << "To             : ";  cin >> dst;
		cout << "Msg            : ";  cin >> msg;
		cout << "Type(sip/smpp) : ";  cin >> type;
		cout << "How many       : ";  cin >> i;
		if(strcmp(type,"sip")==0){
		    while(i-- > 0){
			sms_set(DB_TYPE_SIP,(const char*)&src,(const char*)&dst,(const char*)&msg);
			size_sip++;
		    }
		}else if(strcmp(type,"smpp")==0){
		    while(i-- > 0){
			sms_set(DB_TYPE_SMPP,(const char*)&src,(const char*)&dst,(const char*)&msg);
                        size_smpp++;
		    }
		}else{
			printf("Type is wrong\n");
		}
	}
	if(str == "size_list"){
		cout << "size_smpp    : " << size_smpp    << endl;
		cout << "size_sip     : " << size_sip     << endl;
	}
	if(str == "reload_sip"){
	   if(!loadFileIni(conffile)){
	        ERROR(LOG_FILE | LOG_SCREEN,"There are errors in the INI file!\n");
	   }else{
		if(sip){
			delete sip;
			sip = NULL;
		}
	   	sleep(1);
           	pthread_create(&transceiverSIP,NULL,gestionSIP,NULL);
	   }
	}
	if(str == "reload_smpp"){
    		if(!loadFileIni(conffile)){
		        ERROR(LOG_FILE | LOG_SCREEN,"There are errors in the INI file!");
		}else{
/*			if(smpp){
				delete smpp;
				smpp = NULL;
			}
			sleep(1);
        		pthread_create(&transceiverSMPP,NULL,gestionSMP,NULL);
*/		}
	}
	if(str == "shutdown"){
		cout << "Bey" << endl;
	}
    }//End While
	
    running = 0;

    if(smpp) {
       delete smpp;
       smpp = NULL;
    }
    if(sip)  {
       delete sip;
       sip = NULL;
    }

//  pthread_join(transceiverSMPP,NULL);
    pthread_join(transceiverSIP,NULL);
    pthread_join(checkDBconnection,NULL);

    handler(0);
    return 0;
}
