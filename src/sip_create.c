/**
*	\file createMessageSip.cpp
*/

#include "sip_create.h"

/**
*  \brief This function allow to create 'Accepted' trame SIP
*/
static uint8_t *str_trame_sip_405_method_not_allowed = {
          "SIP/2.0 405 Method Not Allowed\r\n"
          "From: sip:%s@%s:%d\r\n"  // user_from - ip_src - port_src
          "To: sip:%s@%s:%d;tag=1B0QUUmSDeB7B\r\n" // user_to - ip_dst - port_dst
          "Call-ID: %s\r\n"  //call_id - ip_dst
          "CSeq: 1 MESSAGE\r\n"
        };

int create_trame_sip_405_method_not_allowed(uint8_t **str_trame, uint8_t *ip_host, uint32_t port_host, uint8_t *ip_remote, uint32_t port_remote, uint8_t *user_from, uint8_t *user_to, uint8_t *call_id){
    if(str_trame && ip_host && port_host > 0 && ip_remote && port_remote > 0 && user_from && user_to){
        if(*str_trame == NULL){
            *str_trame = (uint8_t*)calloc(2048,sizeof(uint8_t));
        }
        sprintf(*str_trame, str_trame_sip_405_method_not_allowed,
                user_from, ip_host, port_host,
                user_to, ip_remote, port_remote,
                call_id);
        return (int) 0;
    }
    return (int) -1;
}

/**
*  \brief This function allow to create 'Accepted' trame SIP
*/
static uint8_t *str_trame_sip_202_accepted = {
          "SIP/2.0 202 Accepted\r\n"
          "From: sip:%s@%s:%d\r\n"  // user_from - ip_src - port_src
          "To: sip:%s@%s:%d;tag=1B0QUUmSDeB7B\r\n" // user_to - ip_dst - port_dst
          "Call-ID: %s\r\n"  //call_id - ip_dst
          "CSeq: 1 MESSAGE\r\n"
        };

int create_trame_sip_202_accepted(uint8_t **str_trame, uint8_t *ip_host, uint32_t port_host, uint8_t *ip_remote, uint32_t port_remote, uint8_t *user_from, uint8_t *user_to, uint8_t *call_id){
    if(str_trame && ip_host && port_host > 0 && ip_remote && port_remote > 0 && user_from && user_to){
        if(*str_trame == NULL){
            *str_trame = (uint8_t*)calloc(2048,sizeof(uint8_t));
        }
        sprintf(*str_trame, str_trame_sip_202_accepted,
                user_from, ip_host, port_host,
                user_to, ip_remote, port_remote,
                call_id);
        return (int) 0;
    }
    return (int) -1;
}

/**
*  \brief This function allow to create 'Message' trame SIP
*/
static uint8_t *str_trame_sip_message = {
          "MESSAGE sip:%s@%s:%d SIP/2.0\r\n" // user_to - ip_dst - port_dst
          "From: %s <sip:%s@%s:%d>;tag=%s\r\n" // user_from - user_from - ip_src - port_src - tag
          "To: %s <sip:%s@%s:%d>\r\n" // user_to user_to - ip_dst - port_dst
          "CSeq: 1 MESSAGE\r\n"
          "Call-ID: %s\r\n" //uuid
          "Contact: sip:%s@%s:%d\r\n" //user_from - ip_src - port_src
          "Content-Type: text/plain\r\n"
          "Content-Length: %d\r\n" // size_msg(int)
          "User-Agent: SMSC\r\n"
          "\r\n"
          "%s" //msg
      };

int create_trame_sip_message(uint8_t **str_trame, uint8_t *ip_host, uint32_t port_host, uint8_t *ip_remote, uint32_t port_remote, uint8_t *user_from, uint8_t *user_to, uint8_t *msg){
    if(str_trame && ip_host && port_host > 0 && ip_remote && port_remote > 0 && user_from && user_to && msg){
        //uint8_t uuid[] = { "bb27bf00-6329-11e3-b17c-0002a5d5c51b" };
        uint8_t uuid[] = { "bb27bf00-6329-dfdf" };
        uint8_t tag[] = { "tag543" };
        if(*str_trame == NULL){
            *str_trame = (uint8_t*)calloc(1536 + strlen((char*)msg), sizeof(uint8_t));
        }
        sprintf(*str_trame, str_trame_sip_message,
            user_to,ip_remote,port_remote,
            user_from,user_from,ip_host,port_host,tag,
            user_to,user_to,ip_remote,port_remote,
            uuid,
            user_from,ip_host,port_host,
            strlen((char*)msg),
            msg);
        return (int) 0;
    }
    return (int) -1;
}

/**
*  \brief This function allow to create 'Message' trame SIP
*/
static uint8_t *str_trame_sip_200_ok = {
          "SIP/2.0 200 OK\r\n"
          "From: sip:%s@%s:%d\r\n"  // user_from - ip_src - port_src
          "To: sip:%s@%s:%d;tag=1B0QUUmSDeB7B\r\n" // user_to - ip_dst - port_dst
          "Call-ID: %s\r\n"  //call_id - ip_dst
          "CSeq: 1 MESSAGE\r\n"
        };

int create_trame_sip_200_ok(uint8_t **str_trame, uint8_t *ip_host, uint32_t port_host, uint8_t *ip_remote, uint32_t port_remote, uint8_t *user_from, uint8_t *user_to, uint8_t *call_id){
    if(str_trame && ip_host && port_host > 0 && ip_remote && port_remote > 0 && user_from && user_to){
        if(*str_trame == NULL){
            *str_trame = (uint8_t*)calloc(2048,sizeof(uint8_t));
        }
        sprintf(*str_trame, str_trame_sip_200_ok,
                user_from, ip_host, port_host,
                user_to, ip_remote, port_remote,
                call_id);
        return (int) 0;
    }
    return (int) -1;
}

//////////////////////////////////////////////////////
//////////////////////////////////////////////////////
//////////////////////////////////////////////////////
/*
#define DEF_CALL_ID "Call-ID:"

static char* _get_sip_field(char* sip_msg, char* field){
	char* call_id = NULL;
	char* str1 = strstr(sip_msg,field);
	if(str1){
		char* str2 = strstr(str1,"\r\n");
		call_id = (char*)malloc(sizeof(char)*(str2-str1+1));
		memset(call_id,0,sizeof(char)*(str2-str1+1));
		strncpy(call_id,str1,str2-str1);
	}
	return call_id;
}

uint8_t* get_call_id(uint8_t *sip_msg){
	return _get_sip_field(sip_msg,DEF_CALL_ID);
}
*/

