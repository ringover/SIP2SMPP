/**
*	Transceiver SIP
*/
#include "connectionSIP.h"

/**
*  \brief This function is a constructor to the Connection_SIP object, it allow to initialize a SIP connection
*
*  \param server_ip   This parameter is the IP of the local SIP interface
*  \param server_port This parameter is the Port of the local SIP interface
*  \param bindT       This parameter is mode of connection : SIP_TRANSMITTER | SIP_RECEIVER | SIP_TRANSCEIVER
*  \param verbos      It use for enable or disable the verbosity
*
*/
Connection_SIP::Connection_SIP(string server_ip, string server_port, int bindT, bool verbos){
	cout << "\033[0;33;40mLoading SIP ... \033[0m";
	this->connect = false;
	this->verbos  = verbos;
	this->bindT     = bindT;
	if(bindT == SIP_TRANSMITTER){
		cout << "\033[0;33;40mTRANSMITTER mode ... \033[0m";
	}
        else if(bindT == SIP_RECEIVER){
                cout << "\033[0;33;40mRECEIVER mode ... \033[0m";
        }
	else if(bindT == SIP_TRANSCEIVER){
		cout << "\033[0;33;40mTRANSCEIVER mode ... \033[0m";
	}
	if(do_udp_connect(&sock_udp,(char*)server_ip.c_str(),(int)atoi(server_port.c_str())) == -1){
               exit(-1);
        }
	this->connect = true;
	cout << "\033[0;32;40mok\033[0m" << endl;
}

/**
*  \brief This function is a constructor to the Connection_SIP object
*/
Connection_SIP::~Connection_SIP(){
	this->connect = false;
	do_udp_close(sock_udp);
}

/**
*  \brief This function allow to send a SIP trame
*  
*  \param msg    This parameter is the message of SMS
*  \param addrIP This parameter is the IP of the SIP gateway
*  \param port   This parameter is the Port of the SIP gateway
*
*/
void Connection_SIP::sendSIP(string msg, char* addrIP, char* port){
	char buffer[2048] = "";
	strcpy(buffer,msg.c_str());
	if(this->bindT == SIP_TRANSMITTER || this->bindT == SIP_TRANSCEIVER){
		udp_send(sock_udp, (char*)buffer, sizeof(buffer), addrIP, atoi(port), verbos);
	}else{
		cerr << "\033[0;31;40mError this connection is not TRANSMITTER or TRANSCEIVER\033[0m" << endl;
	}
}

/**
*  \brief This function allow to receive a SIP trame, only if the mode is RECEIVE or TRANSCEIVER.
*/
char* Connection_SIP::receiveSIP(){
	char *buffer = NULL;
	if(this->bindT == SIP_RECEIVER || this->bindT == SIP_TRANSCEIVER){
		buffer =(char*)malloc(sizeof(char)*2049);
		memset(buffer,0,2049);		
		if(udp_receive(sock_udp, &buffer, sizeof(char)*2048, verbos) < 0){
			free(buffer);
			return (char*)NULL;
		}
	}else{
		cerr << "\033[0;31;40mError this connection is not RECEIVER or TRANSCEIVER\033[0m" << endl;
	}
	return (char*)buffer;
}

/*
//sample using
int main(){
	string  SIPMSG = "MESSAGE sip:toto@127.0.0.1:5061 SIP/2.0\r\n";
        SIPMSG += "Via: SIP/2.0/UDP 127.0.0.1 \r\n";
        SIPMSG += "To: sip:toto@127.0.0.1:5061\r\n";
        SIPMSG += "From: sip:toto@127.0.0.1:5061\r\n";
        SIPMSG += "CSeq: 1 MESSAGE\r\n";
        SIPMSG += "Call-ID: toto-567854\r\n";
        SIPMSG += "Content-Length: 4\r\n";
        SIPMSG += "User-Agent: SMSC\r\n";
        SIPMSG += "\r\n";
        SIPMSG += "data";

	cout << "sendSIP" << endl;
	Connection_SIP *sipT = new Connection_SIP("127.0.0.1","5061",SIP_TRANSMITTER,true);
	sipT->sendSIP(SIPMSG);

	cout << "receiveSIP" << endl;
        Connection_SIP *sipR = new Connection_SIP("192.168.10.1","5090",SIP_RECEIVER,true);
        sipR->receiveSIP();

	return 0;
}
*/

