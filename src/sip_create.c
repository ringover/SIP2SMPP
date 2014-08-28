/**
*	\file createMessageSip.cpp
*/

#include "sip_create.h"


/**
*  \brief This function allow to create 'Accepted' trame SIP
*
*  \param ip_dst     This parameter is the IP of the SIP gateway 
*  \param port_dst   This parameter is the Port of the SIP gateway
*  \param ip_src     This parameter is the IP of the local SIP interface
*  \param port_src   This parameter is the Port of the local SIP interface
*  \param user_from  This parameter is a MSISDN of sender 
*  \param user_to    This parameter is a MSISDN of receiver
* 
*/
static uint8_t *str_trame_sip_202_accepted = {
          "SIP/2.0 202 Accepted\r\n"
          "Via: SIP/2.0/UDP %s\r\n" // ip_src
          "From: sip:%s@%s:%s\r\n"  // user_from - ip_src - port_src
          "To: sip:%s@%s:%s;tag=1B0QUUmSDeB7B\r\n" // user_to - ip_dst - port_dst
          "Call-ID: toto-567854\r\n"
          "CSeq: 1 MESSAGE\r\n"
          "Contact: <sip:%s:%s>\r\n" // ip_src - port_src
          "User-Agent: FreeSWITCH-mod_sofia/1.5.5b+git~20130808T175513Z~4eee5aee8c\r\n"
          "Allow: INVITE, ACK, BYE, CANCEL, OPTIONS, MESSAGE, INFO, UPDATE, REGISTER, REFER, NOTIFY, PUBLISH, SUBSCRIBE\r\n"
          "Supported: timer, precondition, path, replaces\r\n"
          "Content-Length: 0\r\n"
        };

uint8_t* create_trame_sip_202_accepted(uint8_t *ip_dst, uint8_t *port_dst, uint8_t *ip_src, uint8_t *port_src, uint8_t *user_from, uint8_t *user_to){
    uint8_t *str_trame = (uint8_t*)calloc(2048,sizeof(uint8_t));
    sprintf(str_trame,str_trame_sip_202_accepted,ip_src,user_from,ip_src,port_src,user_to,ip_dst,port_dst,ip_src,port_src);
    return (uint8_t*) str_trame;
}

/**
*  \brief This function allow to create 'Message' trame SIP
*
*  \param ip_dst     This parameter is the IP of the SIP gateway
*  \param port_dst   This parameter is the Port of the SIP gateway
*  \param ip_src     This parameter is the IP of the local SIP interface
*  \param port_src   This parameter is the Port of the local SIP interface
*  \param user_from  This parameter is a MSISDN of sender
*  \param user_to    This parameter is a MSISDN of receiver
*  \param msg        This parameter is the messgae of SMS
*
*/
static uint8_t *str_trame_sip_message = {
          "MESSAGE sip:%s@%s:%s SIP/2.0\r\n" // user_to - ip_dst - port_dst
          "Max-Forwards: 70\r\n"
          "Via: SIP/2.0/UDP %s:%s\r\n" // ip_src port_src
          "From: %s <sip:%s@%s:%s>;tag=%s\r\n" // user_from - user_from - ip_src - port_src - tag
          "To: %s <sip:%s@%s:%s>\r\n" // user_to user_to - ip_dst - port_dst
          "CSeq: 1 MESSAGE\r\n"
          "Call-ID: %s\r\n" //uuid
          "Contact: sip:%s@%s:%s\r\n" //user_from - ip_src - port_src
          "Content-Type: text/plain\r\n"
          "Content-Length: %d\r\n" // size_msg(int)
          "User-Agent: SMSC\r\n"
          "\r\n"
          "%s" //msg
      };

uint8_t* create_trame_sip_message(uint8_t *ip_dst, uint8_t *port_dst, uint8_t *ip_src, uint8_t *port_src, uint8_t *user_from, uint8_t *user_to, uint8_t *msg){
    //uint8_t uuid[] = { "bb27bf00-6329-11e3-b17c-0002a5d5c51b" };
    uint8_t uuid[] = { "bb27bf00-6329-dfdf" };
    uint8_t tag[] = { "tag543" };
    uint8_t *str_trame = (uint8_t*)calloc(2048,sizeof(uint8_t));
    sprintf(str_trame,str_trame_sip_message,
		user_to,ip_dst,port_dst,
		ip_src,port_src,
		user_from,user_from,ip_src,port_src,tag,
		user_to,user_to,ip_dst,port_dst,
		uuid,
		user_from,ip_src,port_src,
		strlen((char*)msg),
		msg);
    return (uint8_t*) str_trame;
}

/**
*  \brief This function allow to create 'Message' trame SIP
*
*  \param ip_dst     This parameter is the IP of the SIP gateway
*  \param port_dst   This parameter is the Port of the SIP gateway
*  \param ip_src     This parameter is the IP of the local SIP interface
*  \param port_src   This parameter is the Port of the local SIP interface
*  \param user_from  This parameter is a MSISDN of sender
*  \param user_to    This parameter is a MSISDN of receiver
*  \param call_id    This parameter is call id
*
*/
static uint8_t *str_trame_sip_200_ok = {
          "SIP/2.0 200 OK\r\n"
          "From: sip:%s@%s:%s\r\n" //user_from - ip_src - port_src
          "To: sip:%s@%s:%s\r\n" // user_to - ip_dst - port_dst
          "%s\r\n" //call_id
          "CSeq: 1 MESSAGE\r\n"
          "Via: SIP/2.0/UDP %s:%s;alias;received=%s:%s\r\n" // ip_dst - port_dst - ip_src - port_src
      };

uint8_t* create_trame_sip_200_ok(uint8_t *ip_dst, uint8_t *port_dst, uint8_t *ip_src, uint8_t *port_src, uint8_t *user_from, uint8_t *user_to, uint8_t *call_id){
    uint8_t *str_trame = (uint8_t*)calloc(2048,sizeof(uint8_t));
    sprintf(str_trame,str_trame_sip_200_ok,user_from,ip_src,port_src,user_to,ip_dst,port_dst,call_id,ip_dst,port_dst,ip_src,port_src);
    return (uint8_t*) str_trame;
}

//////////////////////////////////////////////////////
//////////////////////////////////////////////////////
//////////////////////////////////////////////////////

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

