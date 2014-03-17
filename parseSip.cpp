/**
*	parseSip.cpp permet de parser des trames SIP
*
*	@author : Mickael ROUSSE
*/
#include "parseSip.h"
#include <iostream>
#include <sstream>

using namespace std;

/**
*	class Sip_FromTo
*/
Sip_FromTo::Sip_FromTo(){
	this->user_name = "";
	this->addr_ip   = "";
	this->addr_port = "";
}

Sip_FromTo::Sip_FromTo(string user_name,string addr_ip,string addr_port){
	this->user_name = user_name;
	this->addr_ip   = addr_ip;
	this->addr_port = addr_port;
}

string Sip_FromTo::get_user_name(){
	return this->user_name;
}

string Sip_FromTo::get_addr_ip(){
	return this->addr_ip;
}

string Sip_FromTo::get_addr_port(){
	return this->addr_port;
}

void Sip_FromTo::set_user_name(string user_name){
	this->user_name = user_name;
}

void Sip_FromTo::set_addr_ip(string addr_ip){
	this->addr_ip   = addr_ip;
}

void Sip_FromTo::set_addr_port(string addr_port){
	this->addr_port = addr_port;
}

void Sip_FromTo::set_addr(string addr){
        if(addr.find(":")){
                set_addr_ip(returnSegStringFromBegin(addr,":"));
                set_addr_port(returnSegStringToEnd(addr,":"));
        }else{
                set_addr_ip(addr);
		set_addr_port("");
        }
}

string Sip_FromTo::returnSegString(string str,string find1, string find2){
        int pos = str.find(find1)+find1.length();
        string strR;
	strR.assign(str,pos,str.find(find2)-pos);
	return strR;
}

string Sip_FromTo::returnSegStringToEnd(string str,string find){
        int pos = str.find(find)+find.length();
        string strR;
	strR.assign(str,pos,str.length()-pos);
        return strR;
}

string Sip_FromTo::returnSegStringFromBegin(string str,string find){
        int pos = str.find(find)+find.length();
        string strR;
	strR.assign(str,0,pos-find.length());
        return strR;
}

/**
*	class Sip_From
*/
Sip_From::Sip_From(string from){
	this->from = from;
	set_user_name(returnSegString(from,"sip:","@"));
	//cout << from.find("<") << endl;
	if(from.find("<")<from.length()){
		set_addr(returnSegString(from,"@",">"));
	}else{
                set_addr(returnSegString(from,"@","\r\n"));
	}
}

/**
*	class Sip_To
*/
Sip_To::Sip_To(string to){
        this->to = to;
        set_user_name(returnSegString(to,"sip:","@"));
	if(to.find("<")<to.length()){
                set_addr(returnSegString(to,"@",">"));
        }else{
                set_addr(returnSegString(to,"@","\r\n"));
	}
}

/*
int main(){
	string fromTmp="From: sip:totoFrom@127.0.0.1:5061\r\n";
	Sip_From *from = new Sip_From(fromTmp);	
	cout << from->get_user_name() << endl;	
	cout << from->get_addr_ip() << endl;	
	cout << from->get_addr_port() << endl;	

	string toTmp="To: <sip:totoTo@127.0.0.1:5061>;tag=1B0QUUmSDeB7B\r\n";
	Sip_To *to = new Sip_To(toTmp);
	cout << to->get_user_name() << endl;
        cout << to->get_addr_ip() << endl;
        cout << to->get_addr_port() << endl;
}
*/
