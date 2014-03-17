/**
*   The main of the SIP-SMPP_Converter project
*
*   @author : Mickael ROUSSE
*/

#include <iostream>

#include <sys/cdefs.h>
#include <sys/types.h>
#include <sys/socket.h>

#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <time.h>
#include <getopt.h>

#include <semaphore.h>

#include "connectionSMPP.h"
#include "connectionSIP.h"
//#include "sip/sipStruct.h"

#include "type_smpp_projet.h"
#include "iniFile.c"

#include "lib/daemonize/daemonize.h"


using namespace std;

/* sender socket */
//int sock;

int debug = 1;

void handler(int value){
	fprintf(stderr, "The process has been terminated\n");
//	if(sock)
//      close(sock);
//  if(pid_file)
//      unlink(pid_file);
    if(debug){
        printf("%sEnd\n%s",RED,NONE);
    }
    exit(value);
}

void usage(int8_t e){

    printf("%sHelp :%s\n",RED,NONE);

    printf("Sip2Smpp [-f file.conf] [option -P file.pid] \n");
    printf("   -h  help\n");
    printf("   -v  show version\n");
    printf("   -f  use fork (parameter 1) | Not implemented\n");
    printf("   -P  PID file. Default PID file is [%s]\n",DEFAULT_PIDFILE);
    printf("   -c  config file to use to specify some options. Default location is [%s]\n",DEFAULT_CONFIG);
    printf("%s",NONE);
	exit(e);
}

/**
*	DECLARATION DES CONNEXION
*/
Connection_SMPP *smpp = NULL;
Connection_SIP  *sip = NULL;
//Connection_SIP  *sipT = NULL;
//Connection_SIP  *sipR = NULL;

/**
*	DECLARATION DES FIFOs
*/
std::queue<SMS*>    sms_send_smpp;
std::queue<SMS*>    sms_listend_smpp;

std::queue<string> sms_send_sip;
std::queue<string> sms_listend_sip;

/**
*	GESTION DES CONNEXION SMPP
*/
sem_t mutex_smpp;

void gestionSMPP_send(){//Connection_SMPP *smpp){//, sem_t &mutex){
	sem_wait(&mutex_smpp);
	
	if(sms_send_smpp.size()>0){
		SMS *sms = NULL;
		sms = sms_send_smpp.front();
		//Connection_SMPP::dislaySMS(*sms);
		if(sms){
			smpp->sendSMS(*sms);
		}
		//delete sms->src;
		//delete sms->dst;
		//delete sms->msg;
		//delete sms;//a verrif
		sms_send_smpp.pop();
	}
	
	sem_post(&mutex_smpp);
	return;
}

void gestionSMPP_listend(){//Connection_SMPP *smpp){//, sem_t &mutex){
	sem_wait(&mutex_smpp);
	
	SMS *sms = NULL;
	if((sms = smpp->receiverSMS(true)) != NULL){
		//if(log_lvl_2){
			Connection_SMPP::displaySMS(*sms);
		//}
		sms_listend_smpp.push(sms);
	}
	
        sem_post(&mutex_smpp);
        return;
}

void gestionSMPP(){//Connection_SMPP *smpp){
	smpp = new Connection_SMPP(mainIni.smpp_server_ip,mainIni.smpp_server_port,
					mainIni.user_smpp,mainIni.pass_smpp,BIND_TRANSCEIVER,true);
	//Connection_SMPP smpp(mainIni.smpp_server_ip,mainIni.smpp_server_port,
	//		mainIni.user_smpp,mainIni.pass_smpp,BIND_TRANSCEIVER,verbos);
	//SMS *sms;
 	sem_init(&mutex_smpp, 0, 1);
	
	while(smpp && smpp->connect == true){
		//cout << "test" << endl;
		std::thread thread_listend(gestionSMPP_listend);//,smpp);//,mutex);
		std::thread thread_send(gestionSMPP_send);//,smpp);//,mutex);
		thread_listend.join();
		thread_send.join();
		sleep(1);
	}
	
	sem_destroy(&mutex_smpp);
	return;
}

/**
*	GESTION DES CONNEXION SIP
*/

sem_t mutex_sip;

void gestionSIP_listend(){//Connection_SIP *sipR){
	sem_wait(&mutex_sip);
	char* str = sip->receiveSIP();
	if(str && strlen((char*)str)>0){
		//cout << str << endl;
		sms_listend_sip.push(str);
	}
	if(str){
		delete str;
	}

        sem_post(&mutex_sip);
	return;
}

void gestionSIP_send(){//Connection_SIP *sipT){
	sem_wait(&mutex_sip);
	
	if(sms_send_sip.size()>0){
		string str = sms_send_sip.front();
		if(str.size()>0){
			cout << "mainIni.sip_dest_ip   = " << mainIni.sip_dest_ip << endl;
			cout << "mainIni.sip_dest_port = " << mainIni.sip_dest_port << endl;
			sip->sendSIP(str,mainIni.sip_dest_ip,mainIni.sip_dest_port);
		}
		sms_send_sip.pop();
	}
	
	sem_post(&mutex_sip);
	return;
}

void gestionSIP(){//Connection_SIP *sipT, Connection_SIP *sipR){
	//sipT = new Connection_SIP(mainIni.sip_dest_ip,mainIni.sip_dest_port,SIP_TRANSMITTER,true);
	sip = new Connection_SIP(mainIni.sip_local_ip,mainIni.sip_local_port,SIP_TRANSCEIVER,true);
 	sem_init(&mutex_sip, 0, 1);

	while( sip && sip->connect ){
		std::thread thread_listend(gestionSIP_listend);
	        std::thread thread_send(gestionSIP_send);
	        thread_listend.join();
	        thread_send.join();
	}

	delete sip;
	sip = NULL;

	sem_destroy(&mutex_sip);
	return;
}

/**
*	GESTION DES CONVERSIONS 
*		- SIP 2 SMPP
*		- SMPP 2 SIP
*/
#include <sstream>
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

#include "parseSip.h"
#include "createMessageSip.h"

bool program_start;

void conversion_SIP2SMPP(){

        while(program_start){
		if(sms_listend_sip.size()>0){
			string str = sms_listend_sip.front();
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
			Connection_SMPP::displaySMS(*sms);
			sms_send_smpp.push(sms);
			sms_listend_sip.pop();
		}
		usleep(100);
	}
	return;
}

void conversion_SMPP2SIP(){	
	while(program_start){
		if(sms_listend_smpp.size()>0){
			SMS *sms = sms_listend_smpp.front();
			string str = createTrameSipSMS(mainIni.sip_dest_ip,mainIni.sip_dest_port,
				mainIni.sip_local_ip,mainIni.sip_local_port,sms->dst,sms->src,sms->msg);
			sms_send_sip.push(str);
			sms_listend_smpp.pop();
			delete sms;
			
		}
		usleep(100);
	}
	
	return;
}

void conversion(){
	std::thread thread_convert_SMPP2SIP(conversion_SMPP2SIP);
        std::thread thread_convert_SIP2SMPP(conversion_SIP2SMPP);
        thread_convert_SMPP2SIP.join();
        thread_convert_SIP2SMPP.join();
	return;
}

/**
*	PROGRAMME PRINCIPAL
*/

int main(int argc,char **argv){
    int c, nofork=0, checkout=0;
    char *conffile = NULL;

	program_start = true;

    if(debug){
        printf("%sStart\n%s",RED,NONE);
    }

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
                    pid_file = optarg;
                    break;
            case 'f':
                    debug  = 1;
                    nofork = 1;
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
        fprintf(stderr,"The INI file isn't found!\n");
        handler(-1);
    }

    if(!loadFileIni(conffile)){
        fprintf(stderr,"There are errors in the INI file!\n");
        handler(-1);
    }

    /*if(daemonize(nofork) != 0){
        fprintf(stderr,"Daemoniize failed: %s\n", strerror(errno));
        exit(-1);
	}*/

    if(nofork || debug) {
        fprintf(stdout,"Version          : [%s]\n", VERSION);

        fprintf(stdout,"SIP dest IP      : [%s]\n", mainIni.sip_dest_ip);
        fprintf(stdout,"SIP dest Port    : [%s]\n", mainIni.sip_dest_port);
        fprintf(stdout,"SIP Local IP     : [%s]\n", mainIni.sip_local_ip);
        fprintf(stdout,"SIP Local Port   : [%s]\n", mainIni.sip_local_port);

        fprintf(stdout,"SMPP Server IP   : [%s]\n", mainIni.smpp_server_ip);
        fprintf(stdout,"SMPP Server Port : [%s]\n", mainIni.smpp_server_port);
        fprintf(stdout,"Device           : [%s]\n", mainIni.capt_device);
        fprintf(stdout,"Pid file         : [%s]\n", pid_file);
        fprintf(stdout,"INI file         : [%s]\n", conffile);
    }

    ///prog///
/*	//SMPP
	Connection_SMPP *smpp = new Connection_SMPP(mainIni.smpp_server_ip,mainIni.smpp_server_port,
                        mainIni.user_smpp,mainIni.pass_smpp,BIND_TRANSCEIVER,true);
	//SIP
	Connection_SIP *sipT = new Connection_SIP(mainIni.sip_dest_ip,mainIni.sip_dest_port,SIP_TRANSMITTER,true);
        Connection_SIP *sipR = new Connection_SIP(mainIni.sip_local_ip,mainIni.sip_local_port,SIP_RECEIVER,true);
*/

	std::thread transceiverSMPP(gestionSMPP);//,smpp);
	std::thread transceiverSIP(gestionSIP);//,sipR,sipT);
	std::thread thread_conversion(conversion);//,sipR,sipT);
	

	//a finir .... command line system
	string str = "";
	while(str != "shutdown"){
		cin >> str;
		if(str == "help"){
			cout << "List of commands :"                                    << endl;
			cout << "  help        : display the commands"                  << endl;
			cout << "  smpp        : push a sms in smpp FIFO"               << endl;
			cout << "  sip         : push a sms in sip FIFO"                << endl;
			cout << "  fifo        : display the sizes of FIFOs"            << endl;
			cout << "  reload sip  : NOT IMPLEMENTED : reload sip connection"  << endl;
			cout << "  reload smpp : NOT IMPLEMENTED : reload smpp connection" << endl;
			cout << "  reload conf : NOT IMPLEMENTED : reload the INI file"    << endl;
			cout << "  loglvl      : NOT IMPLEMENTED : choice the log level"   << endl;
			cout << "  shutdown    : exit the program"                      << endl;
		}
		if(str == "smpp"){
			SMS *sms = (SMS*)malloc(sizeof(SMS));
			sms->src = (char*)malloc(sizeof(char)*21);
			memset(sms->src,0,(sizeof(char))*21);
			sms->dst = (char*)malloc(sizeof(char)*21);
			memset(sms->dst,0,(sizeof(char))*21);
			sms->msg = (char*)malloc(sizeof(char)*161);
			memset(sms->msg,0,(sizeof(char))*161);
			cout << "From : "; cin >> sms->src;
			//sms->src = (char*)"0630566333";
			cout << "To : "; cin >> sms->dst;
			//sms->dst = (char*)"060000003";
			sms->msg = (char*)"Message de test";
			sms_send_smpp.push(sms);
			//smpp->sendSMS((char*)"0630566333",(char*)"060000003",(char*)"Message de test");
		}
		if(str == "sip"){
			Sip_msg *sip_msg = (Sip_msg*)malloc(sizeof(Sip_msg));
			
		}
		if(str == "fifo"){
			cout << "sms_send_smpp    : " << sms_send_smpp.size()    << endl;
			cout << "sms_send_sip     : " << sms_send_sip.size()     << endl;
			cout << "sms_listend_smpp : " << sms_listend_smpp.size() << endl;
			cout << "sms_listend_sip  : " << sms_listend_sip.size()  << endl;
		}
		if(str == "shutdown"){
			cout << "Bey" << endl;
		}
	}
	
	delete smpp;
	delete sip;
	//delete sipT;
	//delete sipR;
	
	thread_conversion.join();
	transceiverSMPP.join();
	transceiverSIP.join();

    handler(0);
    return 0;



}
