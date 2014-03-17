/**
*	.cppteSipMessage permet creer des trames SIP
*
*	@author : Mickael ROUSSE
*/

#include "createMessageSip.h"

string createTrameSipAccepted(string ip_dst, string port_dst, string ip_src, string port_src, string userFrom, string userTo){
	//mettre en paramettre le FROM et le TO de la trame recu
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
string createTrameSipAccepted(Sip_msg &sip_msg){	
	return createTrameSipAccepted(sip_msg.ip_dst, sip_msg.port_dst, sip_msg.ip_src,
					 sip_msg.port_src, sip_msg.userFrom, sip_msg.userTo);
}

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

/*
int main(){
	string tmp;

	tmp = createTrameSipAccepted((char*)"192.168.10.1",(char*)"5061",(char*)"192.168.0.53",(char*)"5060",(char*)"from",(char*)"to");
	cout << tmp << endl;
	
	tmp = createTrameSipSMS((char*)"ToToToToTo",(char*)"fromFromFrom",(char*)"192.168.0.53",(char*)"5060",(char*)"Kikoo Jakie :)");
	cout << tmp  << endl;

}
*/
