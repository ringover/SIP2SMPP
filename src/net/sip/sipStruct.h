#ifndef SIPSTRUCT_H
#define SIPSTRUCT_H

typedef struct Sip_msg{
	char* ip_dst;   //not use
	char* port_dst; //not use
	char* ip_src;   //not use
	char* port_src; //not uxe
	char* userFrom;
	char* userTo;
	char* uuid;
	char* msg;
}Sip_msg;


#endif
