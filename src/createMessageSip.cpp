/**
*	\file createMessageSip.cpp
*/

#include "createMessageSip.h"

/**
*  \brief This function allow to create 'Accepted' trame SIP
*
*  \param ip_dst    This parameter is the IP of the SIP gateway 
*  \param port_dst  This parameter is the Port of the SIP gateway
*  \param ip_src    This parameter is the IP of the local SIP interface
*  \param port_src  This parameter is the Port of the local SIP interface
*  \param userFrom  This parameter is a MSISDN of sender 
*  \param userTo    This parameter is a MSISDN of receiver
* 
*/
string createTrameSipAccepted(string ip_dst, string port_dst, string ip_src, string port_src, string userFrom, string userTo){
	string str ="SIP/2.0 202 Accepted\r\n";
        str +="Via: SIP/2.0/UDP "+ip_src+"\r\n";
        str +="From: sip:" +userFrom+ "@"+ip_src+ ":" +port_src+"\r\n";
        str +="To: <sip:" +userTo+ "@" +ip_dst+ ":" +port_dst+ ">;tag=1B0QUUmSDeB7B\r\n";
        str +="Call-ID: toto-567854\r\n";
        str +="CSeq: 1 MESSAGE\r\n";
        str +="Contact: <sip:" +ip_src+ ":" +port_src+ ">\r\n";
        str +="User-Agent: FreeSWITCH-mod_sofia/1.5.5b+git~20130808T175513Z~4eee5aee8c\r\n";
        str +="Allow: INVITE, ACK, BYE, CANCEL, OPTIONS, MESSAGE, INFO, UPDATE, REGISTER, REFER, NOTIFY, PUBLISH, SUBSCRIBE\r\n";
        str +="Supported: timer, precondition, path, replaces\r\n";
        str +="Content-Length: 0\r\n";
	return str;
}

/**
*  \brief This function allow to create 'Accepted' trame SIP
*
*  \param sip_msg  This parameter contain all information for send the SIP trame.    
*
*/
string createTrameSipAccepted(Sip_msg &sip_msg){	
	return createTrameSipAccepted(sip_msg.ip_dst, sip_msg.port_dst, sip_msg.ip_src,
					 sip_msg.port_src, sip_msg.userFrom, sip_msg.userTo);
}


/**
*  \brief This function allow to create 'Message' trame SIP
*
*  \param ip_dst    This parameter is the IP of the SIP gateway
*  \param port_dst  This parameter is the Port of the SIP gateway
*  \param ip_src    This parameter is the IP of the local SIP interface
*  \param port_src  This parameter is the Port of the local SIP interface
*  \param userFrom  This parameter is a MSISDN of sender
*  \param userTo    This parameter is a MSISDN of receiver
*  \param msg       This parameter is the messgae of SMS
*
*/
string createTrameSipSMS(string ip_dst, string port_dst, string ip_src, string port_src, string userFrom, string userTo,string msg){
	string uuid = "bb27bf00-6329-11e3-b17c-0002a5d5c51b";
	string newMessage = "MESSAGE sip:"+userTo+" SIP/2.0\r\n";
	newMessage += "Via: SIP/2.0/UDP "+ip_src+"\r\n";
	//newMessage += "To: sip:"+userTo+"\r\n";
	//newMessage += "From: sip:"+userFrom+"\r\n";
        newMessage += "To: <sip:" +userTo+ "@" +ip_dst+ ":" +port_dst+ ">\r\n";
        newMessage += "From: <sip:" +userFrom+ "@"+ip_src+ ":" +port_src+">\r\n";
	newMessage += "CSeq: 1 MESSAGE\r\n";
	newMessage += "Call-ID: " + uuid + "@" + ip_dst + "\r\n";
	ostringstream temp;
	temp << msg.size();
	newMessage += "Content-Length: "+temp.str()+"\r\n";
	newMessage += "User-Agent: SMSC\r\n";
	newMessage += "\r\n";
	newMessage += msg;
	return newMessage;
}
/*
string createTrameSipSMS(Sip_msg &sip_msg){
	return createTrameSipSMS(sip_msg.userTo, sip_msg.userFrom, sip_msg.ip_src, sip_msg.uuid, sip_msg.msg);
}
*/

string createSip200(string ip_dst, string port_dst, string ip_src, string port_src, string userFrom, string userTo, string callID){
        string newMessage = "SIP/2.0 200 OK\r\n";
        newMessage += "From: <sip:" +userFrom+ "@"+ip_src+ ":" +port_src+">\r\n";
        newMessage += "To: <sip:" +userTo+ "@" +ip_dst+ ":" +port_dst+ ">\r\n";
	newMessage += callID+"\r\n";
        newMessage += "CSeq: 1 MESSAGE\r\n";
//        newMessage += "Via: SIP/2.0/UDP "+ip_src+":"+port_src+";alias;received="+ip_dst+":"+port_dst+"\r\n";
        newMessage += "Via: SIP/2.0/UDP "+ip_dst+":"+port_dst+";alias;received="+ip_src+":"+port_src+"\r\n";
        //newMessage += "Content-Length: 0\r\n";
        //newMessage += "\r\n";
        return newMessage;
}

static char* _getSipField(char* sip_msg, char* field){
	char* callID = NULL;
	char* str1 = strstr(sip_msg,field);
	if(str1){
		char* str2 = strstr(str1,"\r\n");
		callID = (char*)malloc(sizeof(char)*(str2-str1+1));
		memset(callID,0,sizeof(char)*(str2-str1+1));
		strncpy(callID,str1,str2-str1);
	}
	return callID;
}

char* getCallID(char* sip_msg){
	return _getSipField(sip_msg,"Call-ID:");
}


/*
//example using
int main(){
	string tmp;

	tmp = createTrameSipAccepted((char*)"192.168.10.1",(char*)"5061",(char*)"192.168.0.53",(char*)"5060",(char*)"from",(char*)"to");
	cout << tmp << endl;
	
	tmp = createTrameSipSMS((char*)"ToToToToTo",(char*)"fromFromFrom",(char*)"192.168.0.53",(char*)"5060",(char*)"Kikoo Jakie :)");
	cout << tmp  << endl;

}
*/
