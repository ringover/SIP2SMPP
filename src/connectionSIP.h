/**
*	Connection SIP
*/
#ifndef CONNECTION_SIP_H
#define CONNECTION_SIP_H


#include <iostream>
#include "sip/sipStruct.h"
#include "sip/udp_methode.h"

using namespace std;

#define SIP_RECEIVER    1
#define SIP_TRANSMITTER 2
#define SIP_TRANSCEIVER 3

class Connection_SIP{
	private:
		bool verbos;
		int bindT;
		int sock_udp;

	public:
		bool connect;
		
		Connection_SIP(string server_ip, string server_port, int bind, bool verbose);
		~Connection_SIP();
		
		void sendSIP(string msg, char* addrIP, char* port);
		char* receiveSIP();
};

#endif
