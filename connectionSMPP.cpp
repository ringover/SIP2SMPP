/**
*	Transceiver SMPP
*/
#include "connectionSMPP.h"

Connection_SMPP::Connection_SMPP(string server_ip, string server_port, string user_smpp, string pass_smpp, int bind, bool verbose){
	cout << "\033[0;33;40mLoading SMPP \033[0m";
	this->verbose = verbose;
	this->bind = bind;
	this->connect = false;
	cout << "\033[0;33;40m..\033[0m";
	
	TcpConnect conn_tcp;
	sprintf(conn_tcp.host,"%s",server_ip.c_str());
	conn_tcp.port = atoi((char*)server_port.c_str());
	if(do_tcp_connect(conn_tcp, &sock_tcp)){
        	cerr << "\033[0;31;40mError in tcp connect.\033[0m" << endl;
		exit(-1);
	}
	cout << "\033[0;33;40m.\n\033[0m";
	
	SmppConnect conn_smpp;
	sprintf(conn_smpp.systemId,"%s",user_smpp.c_str());
	sprintf(conn_smpp.password,"%s",pass_smpp.c_str());
	sprintf(conn_smpp.systemType,"%s","WWW");
	switch(bind){
	   case BIND_RECEIVER    :
		if(do_smpp_connectReceiver(&conn_smpp, sock_tcp)){
                	cerr << "\033[0;31;40mError in smpp receiver connect.\033[0m" << endl;
                	exit(-1);
        	}
		break;
	   case BIND_TRANSMITTER :
		if(do_smpp_connectTransmitter(&conn_smpp, sock_tcp)){
                	cerr << "\033[0;31;40mError in smpp transmitter connect.\033[0m" << endl;
                	exit(-1);
        	}
		break;
	   case BIND_TRANSCEIVER :
		if(do_smpp_connect(&conn_smpp, sock_tcp)){
                	cerr << "\033[0;31;40mError in smpp transceiver connect.\033[0m" << endl;
                	exit(-1);
       		}

		break;
	}
	cout << "\033[0;33;40m...\033[0m";
	this->connect = true;
	cout << "\033[0;32;40m ok\033[0m" << endl;
	return;
}

Connection_SMPP::~Connection_SMPP(void){
        cout << "\033[0;31;40mtransceiver ..\033[0m" << sock_tcp << endl;
	this->connect=false;
	if(do_smpp_close(sock_tcp)){
	 	cerr << "\033[0;31;40mError on the closing of the socket.\033[0m" << endl;
	}else{
                cout << "\033[0;31;40m.. is closed\033[0m" << endl;
	}
	return;
}

bool Connection_SMPP::sendSMS(char* fromPhoneNb, char* toPhoneNb, char *message){//, int id, int err, int stat, int doneDate){
	SMS sms;
	
	sms.src = fromPhoneNb;
	sms.dst = toPhoneNb;
	sms.msg = message;	
	//sms.id  = id;
	//sms.err =  err;
	//sms.stat= stat;
	//sms.doneDate = doneDate;
	return (bool)sendSMS(sms); 
}

bool Connection_SMPP::sendSMS(SMS &sms){
        if(do_smpp_send_message(&sms,this->sock_tcp)){
                cerr << "\033[0;31;40mError on the sending SMS.\033[0m" << endl;
                return (bool)false;
        }
        return (bool)true;
}
/*
//utili si utilisation de thread
void receiverSMPP(int *sock_tcp){
	listend_smpp(*sock_tcp,1);//1 pour la verbo
	return;
}
*/
SMS* Connection_SMPP::receiverSMS(bool verbos){
	return listend_smpp(sock_tcp,verbos);//1 pour la verbo
	//int *sock = &sock_tcp;
	//std::thread *receiver = new std::thread(receiverSMPP,sock);
        //return (std::thread*) receiver;
}

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








