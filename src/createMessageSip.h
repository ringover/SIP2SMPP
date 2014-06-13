/**
*	.cppteSipMessage permet creer des trames SIP
*
*	@author : Mickael ROUSSE
*/
#ifndef CREATE_MESSAGE_SIP_H
#define CREATE_MESSAGE_SIP_H

#include <iostream>
#include <sstream>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "net/sip/sipStruct.h"
using namespace std;

	string createTrameSipAccepted(string ip_dst, string port_dst, string ip_src,
						string port_src, string userFrom, string userTo);
	
	string createTrameSipSMS(string ip_dst, string port_dst, string ip_src,
						string port_src, string userFrom, string userTo, string msg);

	string createSip200(string ip_dst, string port_dst, string ip_src,
				string port_src, string userFrom, string userTo, string callID);

	char* getCallID(char* sip_msg);

#endif

