SIP to SMPP Gateway
========

This script is designed to convert SIP MESSAGE to SMPP, and SMPP to SIP Messages. You can use it to create SMPP gateway to receive or send SMS via/to Sip proxy. 

Version 1.0


##INSTALL

  First, you need to install all dependencies :

  apt-get install libdbi1 libdbi-dev libxml2 libxml2-dev sqlite3 libsqlite3-dev libsqlite3-0 libdbd-sqlite3
  
  After you have to install "libsmpp34" library :

> https://github.com/bjtpartners/libsmpp34 

    ./configure
    make && make install

  Now, build the project :
      
    make
    make install
    
  Without error, you should have :
* a configuration file "config.ini"
* a binary file "sip2smpp" (default : "/etc/sip2smpp")
    
##CONFIGURATION

  Edit the configuration file according to your settings, for example :

	[MAIN]
	#loglevel => [0-8]
	loglevel = 8
	#fork => 0/1
	fork = 0

	[SMPP_CONNECT]
	#BTS
	smpp_server_ip = 127.0.0.1
	smpp_server_port = 1234
	#Login/password BTS
	system_id_smpp = test
	pass_smpp = test
	
	[SIP_DEST]
	sip_dest_ip     = 127.0.0.1
	sip_dest_port   = 6000
	
	[SIP_LOCAL]
	sip_local_ip    = 127.0.0.1
	sip_local_port  = 5065
	
	[DBMS]
	#SQLite 3 ONLY
	dbms_name    = sqlite3
	db_path      = /etc/sip2smpp.db
	

##START

This program works well in screen.

    sip2smpp -c /etc/sip2smpp/config.ini
  
  If you need help :

    sip2smpp -h

##ROADMAP

v2.0 :
* V : database implementation replacing the fifo smpp et sip message
* - : add a reload SIP, SMPP, CONF module
* X : thread pooling
* V : change log level
* - : add a daemon mode
 
