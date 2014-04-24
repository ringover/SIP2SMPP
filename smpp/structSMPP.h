#ifdef __cplusplus
extern "C"{
#endif 

#ifndef STRUCTSMPP_H
#define STRUCTSMPP_H

#include <stdlib.h>

typedef struct TcpConnect{
        char host[256];
        int port;
}TcpConnect;

typedef struct SmppConnect{
	char systemId[16];
	char password[9];
	char systemType[13];
}SmppConnect;

typedef struct _SMS{
	char* src;
	char* dst;
	char* msg;
	int   id;
	int   ttl;
	
	/*
	int err;
	int stat;
	int doneDate;
	*/
}SMS;

void free_sms(SMS **sms);

#endif /*STRUCTSMPP_H*/

#ifdef __cplusplus
}
#endif 
