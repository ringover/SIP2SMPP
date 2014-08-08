/**
*  \file ConnectionSMPP.cpp
*  
*  \brief This file contain all function to the Connection_SMPP object.
*  
*/
#include "connectionSMPP.h"

/**
*  \brief This function is a constructor to the Connection_SMPP object, it allow to initialize a SMPP connection
*
*  \param server_ip   This parameter is the IP of the BTS server 
*  \param server_port This parameter is the port of the BTS server
*  \param user_smpp   This parameter is the login of the BTS server
*  \param pass_smpp   This parameter is the password of the BTS server
*  \param bind        This parameter is mode of connection : BIND_RECEIVER | BIND_TRANSMITTER | BIND_TRANSCEIVER
*  \param verbos      It use for enable or disable the verbosity
*
*/
Connection_SMPP::Connection_SMPP(char *server_ip, char *server_port, string user_smpp, string pass_smpp, int bind, bool verbos){
	cout << "\033[0;33;40mLoading SMPP \033[0m";
	this->verbos = verbos;
	this->bind = bind;
	this->connect = false;
	cout << "\033[0;33;40m..\033[0m";
	
	if(do_tcp_connect(&sock_tcp, (char*)server_ip, atoi(server_port)) != 0){
        	cerr << "\033[0;31;40mError : tcp connect.\033[0m" << endl;
		exit(-1);
	}
	cout << "\033[0;33;40m.\n\033[0m";
	
	SmppConnect conn_smpp;
	sprintf(conn_smpp.systemId,"%s",user_smpp.c_str());
	sprintf(conn_smpp.password,"%s",pass_smpp.c_str());
	sprintf(conn_smpp.systemType,"%s","WWW");
	this->connect = true;
	switch(bind){
	   case BIND_RECEIVER    :
		if(do_smpp_connectReceiver(&conn_smpp, sock_tcp)){
                	cerr << "\033[0;31;40mError : smpp receiver connect.\033[0m" << endl;
			this->connect = false;
                //	exit(-1);
        	}
		break;
	   case BIND_TRANSMITTER :
		if(do_smpp_connectTransmitter(&conn_smpp, sock_tcp)){
                	cerr << "\033[0;31;40mError : smpp transmitter connect.\033[0m" << endl;
			this->connect = false;
                //	exit(-1);
        	}
		break;
	   case BIND_TRANSCEIVER :
		if(do_smpp_connect(&conn_smpp, sock_tcp)){
                	cerr << "\033[0;31;40mError : smpp transceiver connect.\033[0m" << endl;
			this->connect = false;
                //	exit(-1);
       		}

		break;
	}
	cout << "\033[0;33;40m...\033[0m";
	cout << "\033[0;32;40m ok\033[0m" << endl;
	return;
}

/**
*  \brief This function is a destructor to the Connection_SMPP object
*/
Connection_SMPP::~Connection_SMPP(void){
        cout << "\033[0;31;40mtransceiver ..\033[0m" << sock_tcp << endl;
	this->connect=false;
	if(do_smpp_close(sock_tcp)){
	 	cerr << "\033[0;31;40mError : socket close.\033[0m" << endl;
	}else{
                cout << "\033[0;31;40m.. is closed\033[0m" << endl;
	}
	return;
}

/**
*  \brief This function allow to send a SMS with the SMPP protocol
*
*  \param[in] fromMsisdn  This parameter is a MSISDN of sender
*  \param[in] toMsisdn    This parameter is a MSISDN of receiver
*  \param[in] message     This parameter is a message to send
*
*/
bool Connection_SMPP::sendSMS(char* fromMsisdn, char* toMsisdn, char *message){//, int id, int err, int stat, int doneDate){
	SMS sms;
	
	sms.src = fromMsisdn;
	sms.dst = toMsisdn;
	sms.msg = message;	
	//sms.id   = id;
	//sms.err  = err;
	//sms.stat = stat;
	//sms.doneDate = doneDate;
	return (bool)sendSMS(sms); //see : Connection_SMPP::sendSMS(SMS &sms)
}

/**
*  \brief This function allow to send a SMS with the SMPP protocol
*
*  \param sms  SMS to send
*/
bool Connection_SMPP::sendSMS(SMS &sms){
        if(do_smpp_send_message(&sms,this->sock_tcp)){
                cerr << "\033[0;31;40mError on the sending SMS.\033[0m" << endl;
                return (bool)false;
        }
        return (bool)true;
}

/**
*  \brief This function allow to receive a SMS
*         A timeout is configured on 250 usec
*
*  \param verbos  It use for enable or disable the verbosity
*/
SMS* Connection_SMPP::receiverSMS(bool verbos){
	return listend_smpp(sock_tcp,verbos);
}

/**
*  \brief This function allow to display a SMS
*
*  \param sms  SMS to display 
*/
void Connection_SMPP::displaySMS(SMS &sms){
	cout << "------------------------------------------------------" << endl;
	cout << "  _______  " << endl;
	cout << " |\\_____/| source addr: " << sms.src << endl;
        cout << " |_______| dest addr: " << sms.dst << "\n" << endl;
        cout << "Short Message :\n" << sms.msg << endl;
/*
        if ((sms.esm_class & smpp::ESM_DELIVER_SMSC_RECEIPT) != 0) {
                DeliveryReport dlr(sms);
                cout << "id: " << dlr.id << endl;
                cout << "err: " << dlr.err << endl;
                cout << "stat: " << dlr.stat << endl;
                cout << "done date: " << dlr.doneDate << endl;
                cout << "submit date: " << dlr.submitDate << endl;
        }
*/
	cout << "------------------------------------------------------" << endl;
	return;
}

/*
void testThreadSend(Connection_SMPP *smpp){
	while(1){
		string str;
		cin >> str;
		if(str.c_str()[0]=='s'){
			smpp->sendSMS((char*)"0630566333",(char*)"060000003",(char*)"Message de test");
		}
	}
}

int main(){
	cout << "\033[0;33;40mSMPP Transceiver Start\033[0m" << endl;
	Connection_SMPP smpp("127.0.0.1","2775","login","password",BIND_TRANSCEIVER,true);

	SMS* sms = NULL;

	std::thread receiver(testThreadSend,&smpp); 

	while(true){
		sms = smpp.receiverSMS(false);
		if(sms){//1 thiread scheduler avec l'envoie des SMS sip reçus
			smsFIFO.push(sms);
		}
		if(smsFIFO.size()>0){//x thread par sms present
			sms = smsFIFO.front();
			smsFIFO.pop();
			Connection_SMPP::displaySMS(*sms);
			//conversion et renvoie SIP
		}
	}

	receiver.join();
	
	return 1;
}
*/
/*
	//smpp.sendSMS("0630566333","060000003","Envoyer message new test ... je ne sais pas quoi ecrire !!");


	//cout << "\033[0;35;40mgetSMS\033[0m" << endl;
	//while((sms = listend_smpp(smpp.getSocket(),1)) == NULL);
	//Connection_SMPP::displaySMS(*sms);
	//std::thread *receiver = smppR.receiverSMS();

	cout << "\033[0;35;40mSend exemple\033[0m" << endl;
	smpp.sendSMS("0630566333","060000003","Message de test");
	cout << "\033[0;35;40mgetSMS\033[0m" << endl;
	while((sms = listend_smpp(smpp.getSocket(),1)) == NULL);
	Connection_SMPP::displaySMS(*sms);
	sleep(2);
	
	cout << "\033[0;35;40mSend exemple\033[0m" << endl;
	smpp.sendSMS("0630566333","060000003","Message de test");
	cout << "\033[0;35;40mgetSMS\033[0m" << endl;
	while((sms = listend_smpp(smpp.getSocket(),1)) == NULL);
	Connection_SMPP::displaySMS(*sms);
	sleep(2);
	//receiver->join();
*/








