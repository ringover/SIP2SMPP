
#include "smpp.h"

extern int  smpp34_errno;
extern char smpp34_strerror[2048];

uint8_t  print_buffer[2048];
uint8_t  local_buffer[2048];
int32_t  local_buffer_len_smpp = 0;
int32_t  ret = 0;
uint32_t tempo = 0;

int32_t pack_and_send(int sock_tcp, void *req);
int32_t recv_and_unpack(int sock_tcp, void *res);

int32_t pack_and_send(int sock_tcp, void *req){
    /* Linealize PDU to buffer ********************************************/
    memset(local_buffer, 0, sizeof(local_buffer));
    ret = smpp34_pack2( local_buffer, sizeof(local_buffer),
                                            &local_buffer_len_smpp, (void*)req);
    if(ret != 0){
        ERROR(LOG_FILE | LOG_SCREEN, "Error in smpp34_pack():%d:\n%s\n", smpp34_errno, smpp34_strerror)
        return(int32_t) -1;
    }
    if(log_get_display() >= LOG_INFO){
        /* Print PDU **********************************************************/
        memset(print_buffer, 0, sizeof(print_buffer));
        ret = smpp34_dumpPdu2( print_buffer, sizeof(print_buffer), (void*)req);
        if(ret != 0){
            ERROR(LOG_FILE | LOG_SCREEN, "Error in smpp34_dumpPdu():%d:\n%s\n", smpp34_errno, smpp34_strerror)
            return(int32_t) -1;
        }
        INFO(LOG_SCREEN, "-----------------------------------------------------------\n"
                         "SENDING PDU \n%s\n", print_buffer)
        /* Print Buffer *******************************************************/
        memset(print_buffer, 0, sizeof(print_buffer));
        ret = smpp34_dumpBuf(print_buffer, sizeof(print_buffer),
                                                local_buffer, local_buffer_len_smpp);
        if(ret != 0){
            ERROR(LOG_FILE | LOG_SCREEN, "Error in smpp34_dumpBuf():%d:\n%s\n", smpp34_errno, smpp34_strerror )
            return(int32_t) -1;
        }
        INFO(LOG_SCREEN, "SENDING BUFFER \n%s\n"
                         "-----------------------------------------------------------\n",print_buffer);
    }
    /* Write to socket ****************************************************/
    ret = send(sock_tcp, local_buffer, local_buffer_len_smpp, 0);
    if(ret != local_buffer_len_smpp){
        ERROR(LOG_FILE | LOG_SCREEN, "Error in send()\n")
        return(int32_t) -1;
    }
}

int32_t recv_and_unpack(int sock_tcp, void *res){
    memset(local_buffer, 0, sizeof(local_buffer));
    /* Read from socket ****************************************************/
    ret = recv(sock_tcp, local_buffer, 4, MSG_PEEK);

    if(ret != 4){
        ERROR(LOG_FILE | LOG_SCREEN, "Error in recv(PEEK)\n")
	//...
        return(int32_t) -1;
    }
    memcpy(&tempo, local_buffer, sizeof(uint32_t)); /* get lenght PDU */
    local_buffer_len_smpp = ntohl( tempo );
    ret = recv(sock_tcp, local_buffer, local_buffer_len_smpp, 0);
    if(ret != local_buffer_len_smpp){
        ERROR(LOG_FILE | LOG_SCREEN, "Error in recv(%d bytes)\n", local_buffer_len_smpp)
        return(int32_t) -1;
    }
    if(log_get_display() >= LOG_INFO){
        /* Print Buffer *******************************************************/
        memset(print_buffer, 0, sizeof(print_buffer));
        ret = smpp34_dumpBuf(print_buffer, sizeof(print_buffer),
                                                local_buffer, local_buffer_len_smpp);
        if(ret != 0){
            ERROR(LOG_FILE | LOG_SCREEN, "Error in smpp34_dumpBuf():%d:\n%s\n", smpp34_errno, smpp34_strerror )
            return(int32_t) -1;
        }
        INFO(LOG_SCREEN, "-----------------------------------------------------------\n"
                         "RECEIVE BUFFER \n%s\n", print_buffer);
    }
    /* unpack PDU *********************************************************/
    ret = smpp34_unpack2((void*)res, local_buffer, local_buffer_len_smpp);
    if(ret != 0){
        ERROR(LOG_FILE | LOG_SCREEN, "Error in smpp34_unpack():%d:%s\n", smpp34_errno, smpp34_strerror)
        return(int32_t) -1;
    }
    if(log_get_display() >= LOG_INFO){
        /* Print PDU **********************************************************/
        memset(print_buffer, 0, sizeof(print_buffer));
        ret = smpp34_dumpPdu2(print_buffer, sizeof(print_buffer), (void*)res);
        if(ret != 0){
            ERROR(LOG_FILE | LOG_SCREEN, "Error in smpp34_dumpPdu():%d:\n%s\n", smpp34_errno, smpp34_strerror);
            return(int32_t) -1;
        }
        INFO(LOG_SCREEN, "RECEIVE PDU \n%s\n"
                         "-----------------------------------------------------------\n", print_buffer)
    }
}

int32_t do_smpp_connect_transceiver(int sock_tcp, uint8_t *user, uint8_t *passwd, uint8_t *system_type, uint8_t ton_src, uint8_t npi_src){
    bind_transceiver_t      req;
    bind_transceiver_resp_t res;
    memset(&req, 0, sizeof(bind_transceiver_t));
    memset(&res, 0, sizeof(bind_transceiver_resp_t));

    // Init PDU
    req.command_length   = 0;
    req.command_id       = BIND_TRANSCEIVER;
    req.command_status   = ESME_ROK;
    req.sequence_number  = 1;
    memcpy((void*)req.system_id,(void*)user,sizeof(req.system_id));
    memcpy((void*)req.password,(void*)passwd,sizeof(req.password));
    memcpy((void*)req.system_type,(void*)system_type,sizeof(req.system_type));
    req.interface_version = SMPP_VERSION;
    req.addr_ton          = ton_src;
    req.addr_npi          = npi_src;
    //memcpy((void*)req.address_range(void*)address_range,sizeof(req.address_range));

    if(pack_and_send(sock_tcp, (void*)&req) == -1){
        return (int32_t) -1;
    }
    if(recv_and_unpack(sock_tcp, (void*)&res) == -1){
        return (int32_t) -1;
    }
    destroy_tlv(res.tlv);

    if(res.command_id != BIND_TRANSCEIVER_RESP || res.command_status != ESME_ROK){
        ERROR(LOG_SCREEN | LOG_FILE, "Error in BIND(BIND_TRANSCEIVER)[%d:%d]\n", res.command_id, res.command_status)
        return(int32_t) -1;
    }
    return(int32_t) 0;
}


int32_t do_smpp_connect_receiver(int sock_tcp, uint8_t *user, uint8_t *passwd, uint8_t *system_type, uint8_t ton_src, uint8_t npi_src){
    bind_receiver_t      req;
    bind_receiver_resp_t res;
    memset(&req, 0, sizeof(bind_receiver_t));
    memset(&res, 0, sizeof(bind_receiver_resp_t));

    // Init PDU
    req.command_length   = 0;
    req.command_id       = BIND_RECEIVER;
    req.command_status   = ESME_ROK;
    req.sequence_number  = 1;
    memcpy((void*)req.system_id,(void*)user,sizeof(req.system_id));
    memcpy((void*)req.password,(void*)passwd,sizeof(req.password));
    memcpy((void*)req.system_type,(void*)system_type,sizeof(req.system_type));
    req.interface_version = SMPP_VERSION;
    req.addr_ton          = ton_src;
    req.addr_npi          = npi_src;
    //memcpy((void*)req.address_range(void*)address_range,sizeof(req.address_range));

    if(pack_and_send(sock_tcp, (void*)&req) == -1){
        return (int32_t) -1;
    }

    if(recv_and_unpack(sock_tcp, (void*)&res) == -1){
        return (int32_t) -1;
    }
    destroy_tlv(res.tlv);

    if(res.command_id != BIND_RECEIVER_RESP || res.command_status != ESME_ROK){
        ERROR(LOG_SCREEN | LOG_FILE, "Error in BIND(BIND_RECEIVER)[%d:%d]\n", res.command_id, res.command_status)
        return(int32_t) -1;
    }
    return(int32_t) 0;
}


int32_t do_smpp_connect_transmitter(int sock_tcp, uint8_t *user, uint8_t *passwd, uint8_t *system_type, uint8_t ton_src, uint8_t npi_src){
    bind_transmitter_t      req;
    bind_transmitter_resp_t res;
    memset(&req, 0, sizeof(bind_transmitter_t));
    memset(&res, 0, sizeof(bind_transmitter_resp_t));

    /* Init PDU ***********************************************************/
    req.command_length   = 0;
    req.command_id       = BIND_TRANSMITTER;
    req.command_status   = ESME_ROK;
    req.sequence_number  = 1;

    memcpy((void*)req.system_id,(void*)user,sizeof(req.system_id));
    memcpy((void*)req.password,(void*)passwd,sizeof(req.password));
    memcpy((void*)req.system_type,(void*)system_type,sizeof(req.system_type));
    req.interface_version = SMPP_VERSION;
    req.addr_ton          = ton_src;
    req.addr_npi          = npi_src;
    //memcpy((void*)req.address_range(void*)address_range,sizeof(req.address_range));

    if(pack_and_send(sock_tcp, (void*)&req) == -1){
        return (int32_t) -1;
    }
    if(recv_and_unpack(sock_tcp, (void*)&res) == -1){
        return (int32_t) -1;
    }
    destroy_tlv(res.tlv);

    if(res.command_id != BIND_TRANSMITTER_RESP || res.command_status != ESME_ROK){
        ERROR(LOG_SCREEN | LOG_FILE, "Error in BIND(BIND_TRANSMITTER)[%d:%d]\n", res.command_id, res.command_status)
        return (int32_t) -1;
    }
    return (int32_t) 0;
}


int32_t do_smpp_send_message(int sock_tcp, uint8_t *src_addr, uint8_t *dst_addr, uint8_t *message, uint8_t ton_src, uint8_t npi_src, uint8_t ton_dst, uint8_t npi_dst){
    submit_sm_t      req;
    submit_sm_resp_t res;
    tlv_t tlv;
    memset(&tlv, 0, sizeof(tlv_t));
    memset(&req, 0, sizeof(submit_sm_t));
    memset(&res, 0, sizeof(submit_sm_resp_t));

    /* Init PDU ***********************************************************/
    req.command_length   = 0;
    req.command_id       = SUBMIT_SM;
    req.command_status   = ESME_ROK;
    req.sequence_number  = 2;
    
    req.source_addr_ton  = ton_src;
    req.source_addr_npi  = npi_src;
    sprintf((char*)req.source_addr,"%s",(char*)src_addr);

    req.dest_addr_ton    = ton_dst;
    req.dest_addr_npi    = npi_dst;
    sprintf((char*)req.destination_addr,"%s",(char*)dst_addr);
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
    //if message is > 254 octets or > 0 octet
    tlv.tag = TLVID_message_payload;
    tlv.length = strlen((char*)message);
    memcpy(tlv.value.octet, message, tlv.length);
    build_tlv( &(req.tlv), &tlv );

    // Add other optional param - sample
    //tlv.tag = TLVID_user_message_reference;
    //tlv.length = sizeof(uint16_t);
    //tlv.value.val16 = 0x0024;
    //build_tlv( &(req.tlv), &tlv );

    if(pack_and_send(sock_tcp, (void*)&req) == -1){
        return (int32_t) -1;
    }
    destroy_tlv(req.tlv);

    if(recv_and_unpack(sock_tcp, (void*)&res) == -1){
        return (int32_t) -1;
    }

    if(res.command_id != SUBMIT_SM_RESP || res.command_status != ESME_ROK){
        ERROR(LOG_SCREEN | LOG_FILE ,"Error in send(SUBMIT_SM)[%08X:%08X]\n", res.command_id, res.command_status)
        return(int32_t) -1;
    }
    return(int32_t) 0;
}

int32_t do_smpp_receive_message(int sock_tcp, uint8_t **src_addr, uint8_t **dst_addr, uint8_t **message){
    if(!(src_addr && *src_addr == NULL && dst_addr && *dst_addr == NULL && message && *message == NULL)){
        return (int32_t) -1;
    }
    int local_buffer_len = 0;
    fd_set input_set;
    struct timeval timeout;
    data_sm_t       req;
    data_sm_resp_t  res;
    memset(&res, 0, sizeof(data_sm_resp_t));
    memset(&req, 0, sizeof(data_sm_t));
    timeout.tv_sec  = 0;
    timeout.tv_usec = 250;
    
    FD_ZERO(&input_set);
    FD_SET(sock_tcp,&input_set);
   
    res.command_length = 0;
    res.command_id = ESME_ROK;
 
    if((ret = select(sock_tcp + 1,&input_set,NULL,NULL,&timeout)) < 0){
        ERROR(LOG_SCREEN | LOG_FILE ,"Error in select socket")
        return (int32_t) -1;
    }
    
    if(ret > 0){
        if(recv_and_unpack(sock_tcp, (void*)&req) == -1){
            return (int32_t) -1;
        }
printf("TODO (DELIV) %d[%d]", req.command_id, req.command_status);

	switch(req.command_id){
        case DELIVER_SM :
        {   //copy SMS
            deliver_sm_t *deliver = (deliver_sm_t*)&res;
            if(strlen((char*)deliver->source_addr)>0 && strlen((char*)deliver->destination_addr)>0 && strlen((char*)deliver->short_message)>0){
                *src_addr = (char*)calloc(strlen((char*)deliver->source_addr)+1,sizeof(char));
                strcpy((char*)*src_addr,(char*)deliver->source_addr);

                *dst_addr = (char*)calloc(strlen((char*)deliver->destination_addr)+1,sizeof(char));
                strcpy((char*)*dst_addr,(char*)deliver->destination_addr);

                *message = (char*)calloc(strlen((char*)deliver->short_message)+1,sizeof(char));
                strcpy((char*)*message,(char*)deliver->short_message);
                return (int) 0;
            }else{
                ERROR(LOG_SCREEN | LOG_FILE ,"Error invalid SMS")
            }
            res.command_id = DELIVER_SM_RESP;
        }
            break;
        case UNBIND  :
            res.command_id = UNBIND_RESP;
            break;
        case ENQUIRE_LINK :
            res.command_id = ENQUIRE_LINK_RESP;
            break;
        default:
            res.command_id = DATA_SM_RESP;
            ERROR(LOG_SCREEN | LOG_FILE ,"Request not implemented [%d:%d]", req.command_id, req.command_status)
            break;
        }

        res.sequence_number = req.sequence_number;

        if(pack_and_send(sock_tcp, (void*)&res) == -1){
            ERROR(LOG_SCREEN | LOG_FILE ,"Error in SM_RESP")
            return (int32_t) -1;
        }else{
            return (int32_t) req.command_id;
        }

//        if(req.command_id != DELIVER_SM || req.command_status != ESME_ROK){
//            ERROR(LOG_SCREEN | LOG_FILE ,"Error in DELIVER_SM[%d:%d]", deliv_sm.command_id, deliv_sm.command_status)
//            return (int) -1;
//        }

    }
    return (int) -1;
}

int32_t do_smpp_close(int sock_tcp){
    unbind_t      req;
    unbind_resp_t res;
    memset(&req, 0, sizeof(unbind_t));
    memset(&res, 0, sizeof(unbind_resp_t));

    // Init PDU
    req.command_length   = 0;
    req.command_id       = UNBIND;
    req.command_status   = ESME_ROK;
    req.sequence_number  = 3;

    if(pack_and_send(sock_tcp, (void*)&req) == -1){
        return (int32_t) -1;
    }

    if(recv_and_unpack(sock_tcp, (void*)&res) == -1){
        return (int32_t) -1;
    }

    if(res.command_id != UNBIND_RESP || res.command_status != ESME_ROK){
        ERROR(LOG_SCREEN | LOG_FILE ,"Error in send(UNBIND)[%d:%d]\n", res.command_id, res.command_status)
        return (int32_t) -1;
    }

    return (int32_t) 0;
}


