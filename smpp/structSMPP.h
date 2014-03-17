#ifdef __cplusplus
extern "C"{
#endif 

#ifndef STRUCTSMPP_H
#define STRUCTSMPP_H

typedef struct TcpConnect{
        char host[256];
        int port;
}TcpConnect;

typedef struct SmppConnect{
	char systemId[16];
	char password[9];
	char systemType[13];
}SmppConnect;

typedef struct SMS{
	char* src;
	char* dst;
	char* msg;
	/*
	int id;
	int err;
	int stat;
	int doneDate;
	*/
}SMS;


#endif /*STRUCTSMPP_H*/

#ifdef __cplusplus
}
#endif 
