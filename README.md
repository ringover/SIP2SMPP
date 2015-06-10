SIP to SMPP Gateway
========

This script is designed to convert SIP MESSAGE to SMPP, and SMPP to SIP Messages. You can use it to create SMPP gateway to receive or send SMS via/to Sip proxy. 

Version 0.2.0


##INSTALL

  First, you need to install all dependencies :

  apt-get install sqlite3 libsqlite3-dev libsqlite3-0 libdbd-sqlite3
  
  After you have to install the libraries libsmpp34, libiconv and libzdb v3.0 :

> https://github.com/bjtpartners/libsmpp34

    ./configure
    make
    make install
    ldconfig

Download and install Libiconv : http://www.gnu.org/software/libiconv/#downloading

    ./configure --prefix=/usr/local
    make
    make install
    ldconfig

Download and install LibZDB : http://www.tildeslash.com/libzdb/#

    ./configure
    make
    make install
    ldconfig

Now, build the project :

    ldconfig
    ./configure
    make
    make install
    
  Without error, you should have :
* a configuration file "config.ini"
* a binary file "sip2smpp" (default : "/etc/sip2smpp")
    
##CONFIGURATION

  Edit the configuration file according to your settings, for example :

  Look at this example : config_tmp.ini

  /!\ This version allows : SMPP interfaces (with client mode ONLY), SIP interfaces
	
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
* V : thread pooling
* V : change log level
* V : add a daemon mode
* ~ : DB sqlite3 for the data persistence

v0.3.0 (X : not implemented | V : implemented | ~ : in progress) : 
* ~ : SMPP server mode
* ~ : add system_type (smpp)
* ~ : bind receiver & transmitter with SMPP
* V : Routing mode
* V : Possibility of using many interfaces (SMPP and SIP)
* V : Receive sar_msg and reconstituting all segments
* V : Converting another data_coding to UTF-8
* X : Add INI parameters for to manage this parameter : data_coding
* X : Add INI parameters for to have a choice of payload_message or sar_msg

v0.4.0 (X : not implemented | V : implemented | ~ : in progress) : 
* X : reload configuration file (SIP or SMPP interfaces, routing module ...)
* X : Use libev

v0.5.0 (X : not implemented | V : implemented | ~ : in progress) : 
* ~ : SIGTRAN module
