sip2smpp
========

SIP to SMPP Gateway

#INSTALL

  First, you need to install the "libsmpp34" library :

> go to the folder "libsmpp34-1.10"

    ./configure
    make && make install
    
  Now, build the project :
      
    make
    make install
    
  Without error, you should have :
* a configuration file "config.ini"
* a binary file "sip2smpp" (default : "/etc/sip2smpp")
    
#CONFIGURATION

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

#START

    sip2smpp -c /etc/sip2smpp/config.ini
  
  If you need help :

    sip2smpp -h




