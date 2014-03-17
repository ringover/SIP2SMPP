SIP to SMPP Gateway
========

This script is designed to convert SIP MESSAGE to SMPP, and SMPP to SIP Messages. You can use it to create SMPP gateway to receive or send SMS via/to Sip proxy. 

Version 1.0


##INSTALL

  First, you need to install the "libsmpp34" library :

Go to the folder "libsmpp34-1.10"

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

    [main]
    #BTS
    smpp_server_ip = 192.168.10.1
    smpp_server_port = 2775
    
    #Login/password
    system_id_smpp = login
    pass_smpp = password
    
    #SIP proxy
    sip_dest_ip     = 127.0.0.1
    sip_dest_port   = 5061
    sip_local_ip    = 127.0.0.1
    sip_local_port  = 5065

##START

This program works well in screen.

    sip2smpp -c /etc/sip2smpp/config.ini
  
  If you need help :

    sip2smpp -h

##ROADMAP

v2.0 :
* database implementation replacing the fifo smpp et sip message
* add a reload SIP, SMPP, CONF module
* change log level
* add a daemon mode
 






