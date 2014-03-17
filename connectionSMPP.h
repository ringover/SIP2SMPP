/**
*	Connection SMPP
*/
#ifndef CONNECTION_SMPP_H
#define CONNECTION_SMPP_H

#include <iostream>
#include <thread>
#include <queue>
//#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#include "smpp/tcp.h"
#include "smpp/smpp.h"
#include "smpp/structSMPP.h"
#include <smpp34.h>
#include <smpp34_structs.h>
#include <smpp34_params.h>

using namespace std;

class Connection_SMPP{
	/**
	*	Attribut
	*/
	protected:
		bool verbose;
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
		Connection_SMPP(string server_ip, string server_port, string user_smpp, string pass_smpp, int bind, bool verbose);
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
		bool sendSMS(char* fromPhoneNb, char* toPhoneNb, char* msg);
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
