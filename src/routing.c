/////////////////////////////
//  SCRIPT ROUTING SAMPLE  //
/////////////////////////////
#include "routing.h"

/**
 * GLOBAL VARIABLE
 * 
 * MAP<char*,socket> = interface_name -> p_socket (TODO : Map Interfaces with p_(sip/smpp)_socket)
 * count_sms         = SMS number in the DB
 */

//send_sms_to_sip(I_LISTEN_SIP, msisdn_src, msisdn_dst, message, ip_remote, port_remote);
//send_sms_to_smpp(I_CONNECTION_SMPP, msisdn_src, msisdn_dst, message);

//interface name (cf INI file)
#define I_CONNECTION_SMPP "SMPP01"
#define I_LISTEN_SIP      "SIP_H_01"

/////////////////
// Foreward sample
/////

int start_routing(){
    return 0;
}

int routing(const unsigned char *interface_name, const unsigned char *origin_ip, const unsigned int *origin_port, sm_data_t *p_sm){
    if(strcmp(interface_name, I_CONNECTION_SMPP) == 0){
        //send to SIP interface
        return (int) send_sms_to_sip(I_LISTEN_SIP, p_sm, "192.168.0.1", 5090);
    }else if(strcmp(interface_name, I_LISTEN_SIP) == 0){
        //send to SMPP interface
        return (int) send_sms_to_client_smpp(I_CONNECTION_SMPP, p_sm);
    }
    ERROR(LOG_SCREEN | LOG_FILE, "Routing gailed");
    return (int) -1;
}

int close_routing(){
    return 0;
}

/// END SCRIPT ROUTING SAMPLE
