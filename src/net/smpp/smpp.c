
#include "smpp.h"

/////////////////////////////////////////
////                                /////
////   Short Message Peer-to-Peer   /////
////           declaration          /////
////                                /////
/////////////////////////////////////////
// Variables & Unions
//////

typedef union bind_t{
    bind_transmitter_t transmitter;
    bind_receiver_t    receiver;
    bind_transceiver_t transceiver;
} bind_t;

typedef union bind_resp_t{
    bind_transmitter_resp_t transmitter;
    bind_receiver_resp_t    receiver;
    bind_transceiver_resp_t transceiver;
} bind_resp_t;

extern int  smpp34_errno;
extern char smpp34_strerror[2048];

////////////////////
// Static Functions
//////

static inline int _smpp_send_sm(socket_t *sock, char *from, char *to, char *msg, int command_id, unsigned int *sequence_number, int src_ton, int src_npi, int dst_ton, int dst_npi);
static inline void _init_pdu_header_(void *data, unsigned int command_id, unsigned int command_status, unsigned int sequence_number);
static inline int _dump_pdu_and_buf(char *buffer, int len, void* data, char *communication_mode);
static inline int _smpp_pack_and_send(socket_t *sock, void *data);

/////////////////////////////////////////
////                                /////
////   Short Message Peer-to-Peer   /////
////              TOOLS             /////
////                                /////
/////////////////////////////////////////

inline const char* bind_to_str(int i){
    if(i == BIND_TRANSCEIVER){
        return (const char*) STR_BIND_TRANSCEIVER;
    }
    if(i == BIND_RECEIVER){
        return (const char*) STR_BIND_RECEIVER;
    }
    if(i == BIND_TRANSMITTER){
        return (const char*) STR_BIND_TRANSMITTER;
    }
    return (const char*) NULL;
}

inline int str_to_bind(const char *str){
    if(str){
        char *tmp = NULL;
        strupper(&tmp, str);
        if(strcmp(tmp, STR_BIND_TRANSCEIVER) == 0){
            return (int) BIND_TRANSCEIVER;
        }
        if(strcmp(tmp, STR_BIND_RECEIVER) == 0){
            return (int) BIND_RECEIVER;
        }
        if(strcmp(tmp, STR_BIND_TRANSMITTER) == 0){
            return (int) BIND_TRANSMITTER;
        }
    }
    return (int) -1;
}

inline const char* npi_to_str(int i){
    if(i == NPI_Unknown){
        return (const char*) STR_NPI_Unknown;
    }
    if(i == NPI_ISDN_E163_E164){
        return (const char*) STR_NPI_ISDN_E163_E164;
    }
    if(i == NPI_Data_X121){
        return (const char*) STR_NPI_Data_X121;
    }
    if(i == NPI_Telex_F69){
        return (const char*) STR_NPI_Telex_F69;
    }
    if(i == NPI_Land_Mobile_E212){
        return (const char*) STR_NPI_Land_Mobile_E212;
    }
    if(i == NPI_National){
        return (const char*) STR_NPI_National;
    }
    if(i == NPI_Private){
        return (const char*) STR_NPI_Private;
    }
    if(i == NPI_ERMES){
        return (const char*) STR_NPI_ERMES;
    }
    if(i == NPI_Internet_IP){
        return (const char*) STR_NPI_Internet_IP;
    }
    if(i == NPI_WAP_Client_Id){
        return (const char*) STR_NPI_WAP_Client_Id;
    }
    return (const char*) NULL;
}

inline int str_to_npi(const char *str){
    if(str){
        char *tmp = NULL;
        strupper(&tmp, str);
        if(strcmp(tmp, STR_NPI_Unknown) == 0){
            return (int) NPI_Unknown;
        }
        if(strcmp(tmp, STR_NPI_ISDN_E163_E164) == 0){
            return (int) NPI_ISDN_E163_E164;
        }
        if(strcmp(tmp, STR_NPI_Data_X121) == 0){
            return (int) NPI_Data_X121;
        }
        if(strcmp(tmp, STR_NPI_Telex_F69) == 0){
            return (int) NPI_Telex_F69;
        }
        if(strcmp(tmp, STR_NPI_Land_Mobile_E212) == 0){
            return (int) NPI_Land_Mobile_E212;
        }
        if(strcmp(tmp, STR_NPI_National) == 0){
            return (int) NPI_National;
        }
        if(strcmp(tmp, STR_NPI_Private) == 0){
            return (int) NPI_Private;
        }
        if(strcmp(tmp, STR_NPI_ERMES) == 0){
            return (int) NPI_ERMES;
        }
        if(strcmp(tmp, STR_NPI_Internet_IP) == 0){
            return (int) NPI_Internet_IP;
        }
        if(strcmp(tmp, STR_NPI_WAP_Client_Id) == 0){
            return (int) NPI_WAP_Client_Id;
        }
    }
    return (int) -1;
}

inline const char* ton_to_str(int i){
    if(i == TON_Unknown){
        return (const char*) STR_TON_Unknown;
    }
    if(i == TON_International){
        return (const char*) STR_TON_International;
    }
    if(i == TON_National){
        return (const char*) STR_TON_National;
    }
    if(i == TON_Network_Specific){
        return (const char*) STR_TON_Network_Specific;
    }
    if(i == TON_Subscriber_Number){
        return (const char*) STR_TON_Subscriber_Number;
    }
    if(i == TON_Alphanumeric){
        return (const char*) STR_TON_Alphanumeric;
    }
    if(i == TON_Abbreviated){
        return (const char*) STR_TON_Abbreviated;
    }
    return (const char*) NULL;
}

inline int str_to_ton(const char *str){
    if(str){
        char *tmp = NULL;
        strupper(&tmp, str);
        if(strcmp(tmp, STR_TON_Unknown) == 0){
            return (int) TON_Unknown;
        }
        if(strcmp(tmp, STR_TON_International) == 0){
            return (int) TON_International;
        }
        if(strcmp(tmp, STR_TON_National) == 0){
            return (int) TON_National;
        }
        if(strcmp(tmp, STR_TON_Network_Specific) == 0){
            return (int) TON_Network_Specific;
        }
        if(strcmp(tmp, STR_TON_Subscriber_Number) == 0){
            return (int) TON_Subscriber_Number;
        }
        if(strcmp(tmp, STR_TON_Alphanumeric) == 0){
            return (int) TON_Alphanumeric;
        }
        if(strcmp(tmp, STR_TON_Abbreviated) == 0){
            return (int) TON_Abbreviated;
        }
    }
    return (int) -1;
}

//Display trame SMPP
static inline int _dump_pdu_and_buf(char *buffer, int len, void* data, char *communication_mode){
    if(log_get_display() >= LOG_INFO && buffer && len > 0 && communication_mode && data){
        int ret = 0;
        unsigned char print_buffer[8192] = { 0 };
        //Print Buffer
        memset(print_buffer, 0, sizeof(print_buffer));
        if((ret = smpp34_dumpBuf(print_buffer, sizeof(print_buffer), buffer, len)) != 0){
            ERROR(LOG_FILE | LOG_SCREEN, "Error in smpp34_dumpBuf():%d:\n%s\n", smpp34_errno, smpp34_strerror )
            return (int) -1;
        }
        INFO(LOG_SCREEN, "-----------------------------------------------------------\n"
                         "%s BUFFER \n%s\n", communication_mode, print_buffer)

        //Print PDU
        memset(print_buffer, 0, sizeof(print_buffer));
        if((ret = smpp34_dumpPdu2(print_buffer, sizeof(print_buffer), (void*)data)) != 0){
            ERROR(LOG_FILE | LOG_SCREEN, "Error in smpp34_dumpPdu():%d:\n%s\n", smpp34_errno, smpp34_strerror);
            return (int) -1;
        }
        INFO(LOG_SCREEN, "%s PDU \n%s\n"
                         "-----------------------------------------------------------\n", communication_mode, print_buffer)
        return (int) 1;
    }
    return (int) 1;
}


/////////////////////////////////////////
////                                /////
////   Short Message Peer-to-Peer   /////
////            RECV SMPP           /////
////                                /////
/////////////////////////////////////////

//Recv_end_unpack
/*
int smpp_scan_sock(socket_t *sock, void *data){
    unsigned char buffer[2048] = { 0 };
    size_t buffer_len = sizeof(buffer);
    int len = 0;
    int ret = 0;
    
    if((len = do_tcp_recv(sock, buffer, buffer_len, 0)) == -1){
        return (int) -1;
    }

    //Unpack PDU
    if((ret = smpp34_unpack2((void*)data, buffer, len)) != 0){
        ERROR(LOG_FILE | LOG_SCREEN, "Error in smpp34_unpack():%d:%s\n", smpp34_errno, smpp34_strerror)
        return (int)-1;
    }

    return (int) _dump_pdu_and_buf((char*)buffer, len, data, "RECEIVE");
}
*/
inline int smpp_receive(socket_t *sock, char *buffer, size_t buffer_len){
    return do_tcp_recv(sock, buffer, buffer_len, 0);
}

inline int smpp_parser(char *buffer, int len, void **data){
    //Unpack PDU
    if(buffer && len > 8 && data && *data == NULL){
        int command_id = 0;
        int tt = 0;
        memcpy(&tt, (buffer+4), 4);//get command_id
        command_id = ntohl(tt);
        switch(command_id){
            case BIND_TRANSMITTER :
                *data = (void*)calloc(1, sizeof(bind_transmitter_t));
                break;
            case BIND_TRANSMITTER_RESP :
                *data = (void*)calloc(1, sizeof(bind_transmitter_resp_t));
                break;
            case BIND_RECEIVER :
                *data = (void*)calloc(1, sizeof(bind_receiver_t));
                break;
            case BIND_RECEIVER_RESP :
                *data = (void*)calloc(1, sizeof(bind_receiver_resp_t));
                break;
            case BIND_TRANSCEIVER :
                *data = (void*)calloc(1, sizeof(bind_transceiver_t));
                break;
            case BIND_TRANSCEIVER_RESP :
                *data = (void*)calloc(1, sizeof(bind_transceiver_resp_t));
                break;
            case OUTBIND :
                *data = (void*)calloc(1, sizeof(outbind_t));
                break;
            case UNBIND :
                *data = (void*)calloc(1, sizeof(unbind_t));
                break;
            case UNBIND_RESP :
                *data = (void*)calloc(1, sizeof(unbind_resp_t));
                break;
            case GENERIC_NACK :
                *data = (void*)calloc(1, sizeof(generic_nack_t));
                break;
            case SUBMIT_SM :
                *data = (void*)calloc(1, sizeof(submit_sm_t));
                break;
            case SUBMIT_SM_RESP :
                *data = (void*)calloc(1, sizeof(submit_sm_resp_t));
                break;
            case SUBMIT_MULTI :
                *data = (void*)calloc(1, sizeof(submit_multi_t));
                break;
            case SUBMIT_MULTI_RESP :
                *data = (void*)calloc(1, sizeof(submit_multi_resp_t));
                break;
            case DELIVER_SM :
                *data = (void*)calloc(1, sizeof(deliver_sm_t));
                break;
            case DELIVER_SM_RESP :
                *data = (void*)calloc(1, sizeof(deliver_sm_resp_t));
                break;
            case DATA_SM :
                *data = (void*)calloc(1, sizeof(data_sm_t));
                break;
            case DATA_SM_RESP :
                *data = (void*)calloc(1, sizeof(data_sm_resp_t));
                break;
            case QUERY_SM :
                *data = (void*)calloc(1, sizeof(query_sm_t));
                break;
            case QUERY_SM_RESP :
                *data = (void*)calloc(1, sizeof(query_sm_resp_t));
                break;
            case CANCEL_SM :
                *data = (void*)calloc(1, sizeof(cancel_sm_t));
                break;
            case CANCEL_SM_RESP :
                *data = (void*)calloc(1, sizeof(cancel_sm_resp_t));
                break;
            case REPLACE_SM :
                *data = (void*)calloc(1, sizeof(replace_sm_t));
                break;
            case REPLACE_SM_RESP :
                *data = (void*)calloc(1, sizeof(replace_sm_resp_t));
                break;
            case ENQUIRE_LINK :
                *data = (void*)calloc(1, sizeof(enquire_link_t));
                break;
            case ENQUIRE_LINK_RESP :
                *data = (void*)calloc(1, sizeof(enquire_link_resp_t));
                break;
            case ALERT_NOTIFICATION :
                *data = (void*)calloc(1, sizeof(alert_notification_t));
                break;
            default:
                ERROR(LOG_SCREEN, "Invalid SMPP PDU [%08X]", command_id);
                break;
        };

        if(*data && smpp34_unpack2((void*)*data, buffer, len) != 0){
            ERROR(LOG_FILE | LOG_SCREEN, "Error in smpp34_unpack():%d:%s\n", smpp34_errno, smpp34_strerror)
            return (int) -1;
        }

        return (int) _dump_pdu_and_buf((char*)buffer, len, *data, "RECEIVE");
    }
    return (int) -1;
}

int smpp_scan_sock(socket_t *sock, void **data){
    unsigned char buffer[2048] = { 0 };
    size_t buffer_len = sizeof(buffer);
    int ret = 0;
    if((ret = smpp_receive(sock, buffer, buffer_len)) == -1 || ret == 0){
        return (int) ret;
    }
    if((ret = smpp_parser(buffer, ret, data)) != 0){
        return (int) ret;
    }
    return (int) ret;
}

/////////////////////////////////////////
////                                /////
////   Short Message Peer-to-Peer   /////
////            SEND SMPP           /////
////                                /////
/////////////////////////////////////////

////////////////////
// Static Function used for SMPP SEND
/////

static inline int _smpp_pack_and_send(socket_t *sock, void *data){
    char buffer[2048] = { 0 };
    int buffer_len = sizeof(buffer);
    int local_len  = 0;
    int ret = 0;

    //Linealize PDU to buffer
    if((ret = smpp34_pack2((uint8_t*)buffer, buffer_len, (int*)&local_len, (void*)data)) != 0){
        ERROR(LOG_FILE | LOG_SCREEN, "Error in smpp34_pack():%d:\n%s\n", smpp34_errno, smpp34_strerror)
        return (int)-1;
    }

    //Display trame SMPP
    _dump_pdu_and_buf(buffer, local_len, data, "SEND");

    //Write to socket
    return (int) do_tcp_send(sock, (uint8_t*)buffer, local_len, 0);
}

unsigned int get_sequence_number(){
    static unsigned int sequence_number = 0;
    if(++sequence_number >= INT32_MAX){
        sequence_number = 1;
    }
    return (unsigned int) sequence_number;
}

static inline void _init_pdu_header(void *data, unsigned int command_id, unsigned int command_status, unsigned int sequence_number){
    if(data){
        generic_nack_t *gen = (generic_nack_t*)data;
        gen->command_length = 0;
        gen->command_id = command_id;
        gen->command_status = command_status;
        if(sequence_number > 0){
            gen->sequence_number = sequence_number;
        }else{
            gen->sequence_number = get_sequence_number();
        }
    }
    return;
}

inline int smpp_send_generic(socket_t *sock, unsigned int command_id, unsigned int command_status, unsigned int *sequence_number){
    int ret = -1;
    //UNBIND || ENQUIRE_LINK 
    //UNBIND_RESP || ENQUIRE_LINK_RESP || CANCEL_RESP || REPLACE_SM_RESP 
    if(sock && sequence_number){
        generic_nack_t gen = { 0 };
        _init_pdu_header((void*)&gen, command_id, command_status, *sequence_number);
        if(sequence_number && *sequence_number == 0){//Response or Request
            *sequence_number = gen.sequence_number;
        }
        ret = _smpp_pack_and_send(sock, (void*)&gen);
    }
    return (int) ret;
}

static inline int _smpp_send_sm(socket_t *sock, char *from, char *to, char *msg, int command_id, unsigned int *sequence_number, int src_ton, int src_npi, int dst_ton, int dst_npi){
    int ret = -1;
    if(sock && from && to && msg){
        submit_sm_t req = { 0 };
        //submit_sm_resp_t res = { 0 };
        tlv_t tlv = { 0 };

        //Init PDU
        _init_pdu_header((void*)&req, command_id, ESME_ROK, *sequence_number);
        *sequence_number = req.sequence_number;

        //Init submit or deliver
        req.source_addr_ton  = src_ton;
        req.source_addr_npi  = src_npi;
        sprintf((char*)req.source_addr,"%s",(char*)from);

        req.dest_addr_ton    = dst_ton;
        req.dest_addr_npi    = dst_npi;
        sprintf((char*)req.destination_addr,"%s",(char*)to);
        //if you need this
          //req.esm_class        = 0;
          //req.protocol_id      = 0;
          //req.priority_flag    = 0;
          //snprintf( req.schedule_delivery_time, TIME_LENGTH, "%s", "");
          //snprintf( req.validity_period, TIME_LENGTH, "%s", "");
          //req.registered_delivery = 0;
          //req.replace_if_present_flag =0;
          //req.data_coding         = 0;
          //req.sm_default_msg_id   = 0;

        //if message is <255 octets
          //req.sm_length = strlen((char*)message);
          //memcpy(req.short_message, message, req.sm_length);
/*        if(strlen((char*)msg) < 255){
            req.sm_length = strlen((char*)msg);
            memcpy(req.short_message, msg, req.sm_length);
        }else{*/
            //if message is > 254 octets or > 0 octet
            tlv.tag = TLVID_message_payload;
            tlv.length = strlen((char*)msg);
            memcpy(tlv.value.octet, msg, tlv.length);
            build_tlv( &(req.tlv), &tlv );
        //}
        // Add other optional param - sample
        //tlv.tag = TLVID_user_message_reference;
        //tlv.length = sizeof(uint16_t);
        //tlv.value.val16 = 0x0024;
        //build_tlv( &(req.tlv), &tlv );

        ret = _smpp_pack_and_send(sock, (void*)&req);
        destroy_tlv(req.tlv);
    }
    return (int) ret;
}


////////////////////
// Request
//
// List :
// |+BIND_RECEIVER |+BIND_TRANSMITTER |+BIND_TRANSCEIVER |-QUERY_SM  |
// |+DELIVER_SM    |+SUBMIT_SM        | REPLACE_SM       | CANCEL_SM |
// | OUTBIND       |+ENQUIRE_LINK     | SUBMIT_MULTI     |+UNBIND    |
// | DATA_SM       |                  |                  |           |
// '+' : implemented
// '-' : not implemented
// ' ' : not allowed
//////


int smpp_send_enquire_link(socket_t *sock, unsigned int *sequence_number){
    return smpp_send_generic(sock, ENQUIRE_LINK, ESME_ROK, sequence_number);
}

int smpp_send_deliver_sm(socket_t *sock, char *from, char *to, char *msg, unsigned int *sequence_number, int src_ton, int src_npi, int dst_ton, int dst_npi){
    return _smpp_send_sm(sock, from, to, msg, DELIVER_SM, sequence_number, src_ton, src_npi, dst_ton, dst_npi);
}

int smpp_send_submit_sm(socket_t *sock, char *from, char *to, char *msg, unsigned int *sequence_number, int src_ton, int src_npi, int dst_ton, int dst_npi){
    return _smpp_send_sm(sock, from, to, msg, SUBMIT_SM, sequence_number, src_ton, src_npi, dst_ton, dst_npi);
}

int smpp_send_query_sm(socket_t *sock, unsigned int *sequence_number){
    int ret = -1;
    if(sock && sequence_number){
        query_sm_t query = { 0 };
        _init_pdu_header((void*)&query, QUERY_SM, ESME_ROK, 0);
        *sequence_number = query.sequence_number;
        //TODO
        //query.message_id      = ;
        //query.source_addr_ton = ;
        //query.source_addr_npi = ;
        //query.source_addr     = ;
        WARNING(LOG_SCREEN, "Query_sm is not implemented")
        ret = _smpp_pack_and_send(sock, (void*)&query);
    }
    return (int) ret;
}

/**
 * SMPP BIND Server
 */

int smpp_send_bind_server(socket_t *sock, char *ip_host, unsigned int port_host){// TODO: ,int nb_client){
    int res = 0;
    if(sock && ip_host && port_host > 0){
        DEBUG(LOG_SCREEN, "Create Server TCP Socket");
        if((res = tcp_socket_server(sock, ip_host, port_host)) == -1){
            ERROR(LOG_FILE | LOG_SCREEN, "Bind SMPP Server failed")
        }
    }
    return (int) res;
}

//TODO:int smpp_wait_specific_client(socket_t *sock, char *ip_remote, int port_remote, int *bind){ //close socket if ip/port is not correct
int smpp_wait_client(socket_t *sock, char **ip_remote, int *port_remote){
    int csocket = 0;
    if(sock && ip_remote && port_remote && bind){
/*        int csock = 0;
        bind_t       req = { 0 };
        bind_resp_t  res = { 0 };

        res.command_length = 0;
        res.command_id = ESME_ROK;
*/
        if((csocket = do_tcp_wait_client(sock, ip_remote, port_remote)) == -1){
            ERROR(LOG_SCREEN | LOG_FILE, "Client SMPP not connect (failed)")
            return (int) csocket;
        }
/*
        sock->csocket = csock;       
 
        if((ret = smpp_scan_sock(sock, (void*)&req)) == -1){
            return (int) -1;
        }

        switch(req.command_id){
            case BIND_RECEIVER :
                res.command_id = BIND_RECEIVER_RESP;
                DEBUG(LOG_SCREEN, "Bind receiver connection")
            break;
            case BIND_TRANSMITTER :
                res.command_id = BIND_TRANSMITTER_RESP;
                DEBUG(LOG_SCREEN, "Bind transmitter connection")
            break;
            case BIND_TRANSCEIVER :
                res.command_id = BIND_TRANSCEIVER_RESP;
                DEBUG(LOG_SCREEN, "Bind transcceiver connection")
            break;
            default:
                res.command_id = DATA_SM_RESP;
                res.command_status = 4;
                ERROR(LOG_SCREEN | LOG_FILE ,"Request not implemented [%d:%d]", req.command_id, req.command_status)
            break;
        }
        *bind = req.command_id;
        res.sequence_number = req.sequence_number;
        if((ret = _smpp_pack_and_send(sock, (void*)&res)) == -1){
            ERROR(LOG_SCREEN | LOG_FILE ,"Error in SM_RESP")
            return (int) -1;
        }
        DEBUG(LOG_SCREEN, "Connection established")
        return (int)req.command_id;
*/
    }
    return (int) csocket;
}

/**
 * SMPP BIND Client
 */

#define _init_pdu_bind(bind, user, passwd, system_type, ton_src, npi_src, address_range) \
    memcpy((void*)bind.system_id,(void*)user,sizeof(bind.system_id)); \
    memcpy((void*)bind.password,(void*)passwd,sizeof(bind.password)); \
    memcpy((void*)bind.system_type,(void*)system_type,sizeof(bind.system_type)); \
    bind.interface_version = SMPP_VERSION; \
    bind.addr_ton          = ton_src; \
    bind.addr_npi          = npi_src; \
    if(address_range && strlen(address_range) > 0){ \
        memcpy((void*)bind.address_range,(void*)address_range,sizeof(bind.address_range)); \
    }

int smpp_send_bind_client(socket_t *sock, int command_id, char *ip_remote, unsigned int port_remote, unsigned char *user, unsigned char *passwd, unsigned char *system_type, int ton_src, int npi_src, char *address_range, unsigned int *sequence_number){
    int ret = -1;
    bind_t bind;
    memset(&bind, 0, sizeof(bind));

    if((ret = tcp_socket_client(sock, ip_remote, port_remote)) == -1){
        ERROR(LOG_SCREEN | LOG_FILE, "Bind SMPP client failed because the TCP connection failed - Error[%d]", ret)
        return (int) ret;
    }

    //Init Header
    _init_pdu_header((void*)&bind, command_id, ESME_ROK, 0);
    *sequence_number = bind.receiver.sequence_number;
    //Init bidn
    switch(command_id){
        case BIND_TRANSCEIVER :
            _init_pdu_bind(bind.transceiver, user, passwd, system_type, ton_src, npi_src, address_range)
            break;
        case BIND_RECEIVER :
            _init_pdu_bind(bind.receiver, user, passwd, system_type, ton_src, npi_src, address_range)
            break;
        case BIND_TRANSMITTER :
            _init_pdu_bind(bind.transmitter, user, passwd, system_type, ton_src, npi_src, address_range)
            break;
    }

    if((ret = _smpp_pack_and_send(sock, (void*)&bind)) == -1){
        ERROR(LOG_SCREEN | LOG_FILE, "Sent BIND failed")
        tcp_close(sock);
    }

    return (int) ret;
}

/*
#define smpp_send_bind_transceiver_client(sock, ip_remote, port_remote, user, passwd, system_type, ton_src, npi_src, sequence_number) \
    smpp_send_bind_client(sock, BIND_TRANSCEIVER, ip_remote, port_remote, user, passwd, system_type, ton_src, npi_src, sequence_number)

#define smpp_send_bind_receiver_client(sock, ip_remote, port_remote, user, passwd, system_type, ton_src, npi_src, sequence_number) \
    smpp_send_bind_client(sock, BIND_RECEIVER, ip_remote, port_remote, user, passwd, system_type, ton_src, npi_src, sequence_number)

#define smpp_send_bind_transmitter_client(sock, ip_remote, port_remote, user, passwd, system_type, ton_src, npi_src, sequence_number) \
    smpp_send_bind_client(sock, BIND_TRANSMITTER, ip_remote, port_remote, user, passwd, system_type, ton_src, npi_src, sequence_number)
*/

#undef _init_pdu_bind

/**
 * SMPP send unbind 
 */

int smpp_send_unbind(socket_t *sock, unsigned int *sequence_number){
    return smpp_send_generic(sock, UNBIND, ESME_ROK, sequence_number);
}


////////////////////
// Response
// 
// List :
// |+BIND_RECEIVER_RESP    | QUERY_SM_RESP   |+UNBIND_RESP     |+ENQUIRE_LINK_RESP |
// |+BIND_TRANSMITTER_RESP |+SUBMIT_SM_RESP  |+REPLACE_SM_RESP | SUBMIT_MULTI_RESP |
// |+BIND_TRANSCEIVER_RESP |+DELIVER_SM_RESP |+CANCEL_SM_RESP  | DATA_SM_RESP      |
// |+DELIVER_SM_RESP       |                 |                 |                   |
// '+' : implemented
// '-' : not implemented
// ' ' : not allowed
/////

int smpp_send_unbind_resp(socket_t *sock, unsigned int sequence_number, unsigned int command_status){
    return smpp_send_generic(sock, UNBIND_RESP, command_status, &sequence_number);
}

int smpp_send_unquire_link_resp(socket_t *sock, unsigned int sequence_number, unsigned int command_status){
    return smpp_send_generic(sock, ENQUIRE_LINK_RESP, command_status, &sequence_number);
}

int smpp_send_cancel_sm_resp(socket_t *sock, unsigned int sequence_number, unsigned int command_status){
    return smpp_send_generic(sock, CANCEL_SM_RESP, command_status, &sequence_number);
}

int smpp_send_replace_sm_resp(socket_t *sock, unsigned int sequence_number, unsigned int command_status){
    return smpp_send_generic(sock, REPLACE_SM_RESP, command_status, &sequence_number);
}

int smpp_send_submit_sm_resp(socket_t *sock, unsigned char *message_id, unsigned int sequence_number, unsigned int command_status){
    int ret = -1;
    if(sock && sequence_number > 0){
        submit_sm_resp_t resp = { 0 };
        _init_pdu_header((void*)&resp, SUBMIT_SM_RESP, command_status, sequence_number);
        if(message_id){
            int msg_id_len = strlen((char*)message_id);
            if(msg_id_len > 0 && msg_id_len <= sizeof(resp.message_id)){
                memcpy(resp.message_id, message_id, msg_id_len);
            }
        }
        ret = _smpp_pack_and_send(sock, (void*)&resp);
    }
    return (int) ret;
}

int smpp_send_deliver_sm_resp(socket_t *sock, unsigned char *message_id, unsigned int sequence_number, unsigned int command_status){
    int ret = -1;
    if(sock && sequence_number > 0){
        int msg_id_len = strlen((char*)message_id);
        deliver_sm_resp_t resp = { 0 };
        _init_pdu_header((void*)&resp, DELIVER_SM_RESP, command_status, sequence_number);
        if(message_id){
            int msg_id_len = strlen((char*)message_id);
            if(msg_id_len > 0 && msg_id_len <= sizeof(resp.message_id)){
                memcpy(resp.message_id, message_id, msg_id_len);
            }
        }
        ret = _smpp_pack_and_send(sock, (void*)&resp);
    }
    return (int) ret;
}

#define _init_pdu_bind_resp(bind, system_id, set_version) \
    memcpy((void*)bind.system_id, (void*)system_id, sizeof(bind.system_id)); \
    if(set_version == true){ \
        tlv_t tlv = { 0 }; \
        tlv.tag = TLVID_sc_interface_version; \
        tlv.length = 1; \
        tlv.value.val08 = SMPP_VERSION; \
        build_tlv( &(bind.tlv), &tlv ); \
    } \

#define _destroy_tlv(data, set_version) \
    if(set_version == true){ \
        destroy_tlv(data.tlv); \
    } \

int smpp_send_bind_resp(socket_t *sock, unsigned int command_id, unsigned char *system_id, unsigned int sequence_number, unsigned int command_status, bool set_version){
    int ret = -1;
    if(sock && system_id){
        bind_resp_t resp;
        memset((void*)&resp, 0, sizeof(resp));
        //Init header
        _init_pdu_header((void*)&resp, command_id, command_status, sequence_number);
        //Init resp
        switch(command_id){
            case BIND_TRANSCEIVER_RESP :
                _init_pdu_bind_resp(resp.transceiver, system_id, set_version)
                break;
            case BIND_TRANSMITTER_RESP :
                _init_pdu_bind_resp(resp.transmitter, system_id, set_version)
                break;
            case BIND_RECEIVER_RESP :
                _init_pdu_bind_resp(resp.receiver, system_id, set_version)
                break;
        }
        ret = _smpp_pack_and_send(sock, (void*)&resp);
        switch(command_id){
            case BIND_TRANSCEIVER_RESP :
                _destroy_tlv(resp.transceiver, set_version)
                break;
            case BIND_TRANSMITTER_RESP :
                _destroy_tlv(resp.transmitter, set_version)
                break;
            case BIND_RECEIVER_RESP :
                _destroy_tlv(resp.receiver, set_version)
                break;
        }
    }
    return (int) ret; 
}

/*
#define smpp_send_bind_transceiver_resp(sock, system_id, sequence_number, command_status, set_version) \
    smpp_send_bind_resp(sock, BIND_TRANSCEIVER_RESP, system_id, sequence_number, command_status, set_version)

#define smpp_send_bind_transmitter_resp(sock, system_id, sequence_number, command_status, set_version) \
    smpp_send_bind_resp(sock, BIND_TRANSMITTER_RESP, system_id, sequence_number, command_status, set_version)

#define smpp_send_bind_receiver_resp(sock, system_id, sequence_number, command_status, set_version) \
    smpp_send_bind_resp(sock, BIND_RECEIVER_RESP, system_id, sequence_number, command_status, set_version)
*/

#undef _init_pdu_bind_resp
#undef _destroy_tlv



