/**
*	Connection SMPP
*/
#ifndef CONNECTION_SMPP_H
#define CONNECTION_SMPP_H

#include <iostream>
//#include <thread>
//#include <queue>
//#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#include "net/tcp/tcp.h"
#include "net/smpp/smpp.h"
#include "net/smpp/struct_smpp.h"
//#include "net/smpp/lib_smpp34/smpp34.h"
//#include "net/smpp/lib_smpp34/smpp34_structs.h"
//#include "net/smpp/lib_smpp34/smpp34_params.h"
#include <smpp34.h>
#include <smpp34_structs.h>
#include <smpp34_params.h>

using namespace std;

class Connection_SMPP{
	/**
	*	Attribut
	*/
	protected:
		bool verbos;
		int sock_tcp;
		int bind;
	/**
	*	Methode
	*/
	public:
		bool connect;
		/**
		*	Contructor/Destructor
		*/
		Connection_SMPP(char *server_ip, char *server_port, string user_smpp, string pass_smpp, int bind, bool verbos);
		~Connection_SMPP(void);
	
		int getSocket(){
			return this->sock_tcp;
		}
		int getBind(){
			return this->bind;
		}		
		/**
		*	Function of SMS receive/send
		*/
		bool sendSMS(char* fromMsisdn, char* toMsisdn, char* msg);
		bool sendSMS(SMS &sms);
		//std::thread* receiverSMS();
		SMS* receiverSMS(bool verbos);
		/**
		*	Affichage
		*/
		static void displaySMS(SMS &sms);
};

//std::queue<SMS*> smsFIFO;

//static void receiverSMPP(void* p_data);
#endif
