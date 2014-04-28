#ifdef __cplusplus
extern "C"{
#endif 

#ifndef SMPP_H
#define SMPP_H

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>

#ifdef __linux__
#include <stdint.h>
#endif

#include <smpp34.h>
#include <smpp34_structs.h>
#include <smpp34_params.h>

#include "structSMPP.h"
/*
extern int  smpp34_errno;
extern char smpp34_strerror[2048];

uint8_t print_buffer[2048];
uint8_t local_buffer[1024];
int  local_buffer_len_smpp = 0;
int  ret = 0;
uint32_t tempo = 0;
uint32_t cmd_id = 0;
*/
int unhex(char c);

int hex2bin(char *hex, unsigned char *bin);

int do_smpp_connect(SmppConnect *p, int sock_tcp);
int do_smpp_connectTransmitter(SmppConnect *p, int sock_tcp);
int do_smpp_connectReceiver(SmppConnect *p, int sock_tcp);

int do_smpp_send_message(SMS *p, int sock_tcp);

SMS* listend_smpp(int sock_tcp, int verbose);

int do_smpp_close(int sock_tcp);

#endif

#ifdef __cplusplus
}
#endif
