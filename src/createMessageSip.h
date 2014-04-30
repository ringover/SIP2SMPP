/**
*	.cppteSipMessage permet creer des trames SIP
*
*	@author : Mickael ROUSSE
*/
#ifndef CREATE_MESSAGE_SIP_H
#define CREATE_MESSAGE_SIP_H

#include <iostream>
#include <sstream>
#include "net/sip/sipStruct.h"
using namespace std;

	string createTrameSipAccepted(string ip_dst, string port_dst, string ip_src,
						string port_src, string userFrom, string userTo);
	
	string createTrameSipSMS(string ip_dst, string port_dst, string ip_src,
						string port_src, string userFrom, string userTo, string msg);


#endif

