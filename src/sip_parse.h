/**
*	parseSip.cpp permet de parser des trames SIP
*
*	@author : Mickael ROUSSE
*/
#ifndef PARSE_SIP_H
#define PARSE_SIP_H

#include <iostream>
#include <sstream>

using namespace std;

class Sip_FromTo{
	protected:
		string user_name;
		string addr_ip;
		string addr_port;
	protected:
		Sip_FromTo(string user_name,string addr_ip,string addr_port);
		Sip_FromTo();
		string returnSegString(string str,string find1, string find2);
		string returnSegStringFromBegin(string str,string find);
		string returnSegStringToEnd(string str,string find);
		void set_user_name(string);
		void set_addr_ip(string);
		void set_addr_port(string);
		void set_addr(string);
	public:
		string get_user_name();
                string get_addr_ip();
                string get_addr_port();

};

class Sip_From : public Sip_FromTo{
//exemple de from : "From: sip:toto@127.0.0.1:5061\r\n";
	private:
		string  from;
	public:
		Sip_From(string);
};

class Sip_To : public Sip_FromTo{
//exemple de to : "To: <sip:toto@127.0.0.1:5061>;tag=1B0QUUmSDeB7B\r\n"
	protected:
		string  to;
	public:
		Sip_To(string to);
};

#endif

