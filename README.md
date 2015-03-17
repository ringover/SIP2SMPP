SIP to SMPP Gateway
========

This script is designed to convert SIP MESSAGE to SMPP, and SMPP to SIP Messages. You can use it to create SMPP gateway to receive or send SMS via/to Sip proxy. 

Version 0.2.0


##INSTALL

  First, you need to install all dependencies :

  apt-get install sqlite3 libsqlite3-dev libsqlite3-0 libdbd-sqlite3
  
  After you have to install the libraries "libsmpp34" and "libzdb v3.0" :

> https://github.com/bjtpartners/libsmpp34

    ./configure
    make && make install

Download and install LibZDB : http://www.tildeslash.com/libzdb/#

    ./configure
    make
    make install

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
	fork = 1

	[SMPP_CONNECT]
	#The interface name
	interface_name = SMPP01
	# Specifies the address IPv4 and Port of an ESME
	smpp_ip = xxx.xxx.xxx.xxxx
	smpp_port = 2777
	# The purpose of the SMPP bind operation is to register an instance of an ESME with the SMSC 
	# system and request an SMPP session over this network connection for the submission or 
	# delivery of message. An ESME may bind to the SMSC as either a Transmitter (called ESME Transmitter),
	# a Receiver (called EMSE Receiver) or a Transceiver (called ESME Transceiver). 
	# There are three SMPP bind PDUs to support the various modes of operation namely 
	#    -  bind_receiver    => 1 # /!\ not allowed for the moment
	#    -  bind_transmitter => 2 # /!\ not allowed for the moment
	#    -  bind_transceiver => 3
	# The command_id field setting specifies which PDU is being used.
	# 
	command_id = 3
	# The system_type parameter is used to categorize the type of ESME that
	# is binding to the SMSC. Example : WWW - EMAIL - VMS - OTA - etc
	system_type = WWW
	#The client–server model
	# values :  CLIENT(0) or SERVER(1)
	# The server mode for SMPP is not allowed for this moment  /!\
	model = 0
	
	# The TON (Type Of Number) to be used in the SME address parameters.
	# The folowing TON values are defined :
	#    Unknown            : 0
	#    International      : 1
	#    National           : 2
	#    Network Specific   : 3
	#    Subscriber Nunmber : 4
	#    Alphanumeric       : 5
	#    Abbreviated        : 6
	#
	type_of_number = 1
	# or both :
	#type_of_number_src = 1
	#type_of_number_dst = 0
	
	#
	# The NPI (Numeric Plan Indicaor) to be used in the SME adress parameters.
	# the following NPI values are defines :
	#    Unknow               : 0
	#    ISDN (E163/E164)     : 1
	#    Data (X.121)         : 3
	#    Telex (F.69)         : 4
	#    Land Mobile (E.212)  : 6
	#    National             : 8
	#    Private              : 9
	#    ERMES                : 10
	#    Internet (Ip)        : 14
	#    WAP Client Id        : 18
	numeric_plan_indicator = 1
	# or both :
	#numeric_plan_indicator_src = 1
	#numeric_plan_indicator_dst = 1
	
	# Specifies Login/password of an ESME
	system_id_smpp = user
	pass_smpp = password
	
	[SIP_REMOTE]
	interface_name = SIP_R_01
	sip_ip     = xxx.xxx.xxx.xxx
	sip_port   = 5070
	#The client–server model
	# values :  CLIENT(0) or SERVER(1)
	model      = 0
	
	[SIP_HOST]
	interface_name = SIP_H_01
	sip_ip    = xxx.xxx.xxx.xxx
	sip_port  = 5061
	#The client–server model
	# values :  CLIENT(0) or SERVER(1)
	model     = 1
	
	[DBMS]
	#SQLite 3 ONLY
	dbms_name    = sqlite3
	db_path      = /etc/sip2smpp/sms.db
	#db_ttl_sms  = 0
	##normal / full
	#db_synchronous = normal
	##max size db (in Ko)
	#db_heap_limit = 8096000
	#db_foreign_keys = on

	
##START

This program works well in screen.

    sip2smpp -c /etc/sip2smpp/config.ini
  
  If you need help :

    sip2smpp -h

=======
##ROADMAP

v0.2.0 (X : not implemented | V : implemented | ~ : in progress) : 
* V : SMPP client mode 
* V : SIP mode 
* V : DB sqlite3 for the data persistence

v0.3.0 (X : not implemented | V : implemented | ~ : in progress) : 
* X : reload SIP, SMPP, CONF module
* V : thread pooling
* V : change log level
* V : add a daemon mode
* X : add system_type (smpp)
* ~ : SMPP server mode
* ~ : bind receiver & transmitter with SMPP
* ~ : Routing mode
* ~ : Possibility of using many interface (SMPP and SIP)

v0.4.0 (X : not implemented | V : implemented | ~ : in progress) : 
* X : SIGTRAN module
