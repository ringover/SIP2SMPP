#include "smpp.h"

extern int  smpp34_errno;
extern char smpp34_strerror[2048];

uint8_t print_buffer[2048];
uint8_t local_buffer[2048];
int  local_buffer_len_smpp = 0;
int  ret = 0;
uint32_t tempo = 0;
uint32_t cmd_id = 0;

int unhex(char c) {
  if (c >= '0' && c <= '9')
    return (int)(c - '0');
  if (c >= 'a' && c <= 'f')
    return (int)(c - 'a' + 10);
  if (c >= 'A' && c <= 'F')
    return (int)(c - 'A' + 10);
  return (-1);
}

int hex2bin(char *hex, unsigned char *bin) {
  unsigned int i = 0;
  unsigned int j = 0;

  /* Trim string if comments present */
  if (strchr(hex, '#') != NULL)
    *strchr(hex, '#') = 0;
  if (strchr(hex, '*') != NULL)
    *strchr(hex, '*') = 0;
  if (strchr(hex, '\'') != NULL)
    *strchr(hex, '\'') = 0;

  for (i = 0; i < strlen(hex); i++) {
    if (hex[i] >= '0' && unhex(hex[i]) < 0){
      printf("Bad hex digit encountered.\n"); 
      return( -1 );
    }
  }

  for (i = 0; i < strlen(hex); i++) {
    if (hex[i] < '0')
      continue;
    if (hex[i] >= '0' && hex[i+1] >= '0') {
      bin[j++] = unhex(hex[i])*16+unhex(hex[i+1]);
      i++;
      continue;
    }
    if (hex[i] >= '0') {
      bin[j++] = unhex(hex[i]);
    }
  }
  return (j);
}

int do_smpp_connect(SmppConnect *p, int sock_tcp){
    bind_transceiver_t      req;
    bind_transceiver_resp_t res;
    memset(&req, 0, sizeof(bind_transceiver_t));
    memset(&res, 0, sizeof(bind_transceiver_resp_t));

    /* Init PDU ***********************************************************/
    req.command_length   = 0;
    req.command_id       = BIND_TRANSCEIVER;   
    req.command_status   = ESME_ROK;           
    req.sequence_number  = 1;
    sprintf((char*)req.system_id,"%s",(char*)p->systemId);
    sprintf((char*)req.password,"%s",(char*)p->password);
    sprintf((char*)req.system_type,"%s",(char*)p->systemType);
    req.interface_version = SMPP_VERSION;
#if 0 /* if you need add */
    req.addr_ton          = 2;
    req.addr_npi          = 1;
    snprintf( b.address_range, sizeof(b.address_range), "%s", "");
#endif

    /* FIRST STEP: PACK AND SEND */
#include "pack_and_send.inc"
    /* Set a timer (PENDIENTE) ********************************************/
    /* SECOND STEP: READ AND UNPACK Response */
#include "recv_and_unpack.inc"
    destroy_tlv( res.tlv ); /* Por las dudas */

    if( res.command_id != BIND_TRANSCEIVER_RESP ||
        res.command_status != ESME_ROK ){
        printf("Error in BIND(BIND_TRANSCEIVER)[%d:%d]\n", 
                                       res.command_id, res.command_status);
        return( -1 );
    };
    return( 0 );
}


int do_smpp_connectReceiver(SmppConnect *p, int sock_tcp){
    bind_receiver_t      req;
    bind_receiver_resp_t res;
    memset(&req, 0, sizeof(bind_receiver_t));
    memset(&res, 0, sizeof(bind_receiver_resp_t));

    /* Init PDU ***********************************************************/
    req.command_length   = 0;
    req.command_id       = BIND_RECEIVER;
    req.command_status   = ESME_ROK;
    req.sequence_number  = 1;
    sprintf((char*)req.system_id,"%s",(char*)p->systemId);
    sprintf((char*)req.password,"%s",(char*)p->password);
    sprintf((char*)req.system_type,"%s",(char*)p->systemType);
    req.interface_version = SMPP_VERSION;
#if 0 /* if you need add */
    req.addr_ton          = 2;
    req.addr_npi          = 1;
    snprintf( b.address_range, sizeof(b.address_range), "%s", "");
#endif

    /* FIRST STEP: PACK AND SEND */
#include "pack_and_send.inc"
    /* Set a timer (PENDIENTE) ********************************************/
    /* SECOND STEP: READ AND UNPACK Response */
#include "recv_and_unpack.inc"
    destroy_tlv( res.tlv ); /* Por las dudas */

    if( res.command_id != BIND_RECEIVER_RESP ||
        res.command_status != ESME_ROK ){
        printf("Error in BIND(BIND_RECEIVER)[%d:%d]\n",
                                       res.command_id, res.command_status);
        //return( -1 );
    };
    return( 0 );
}


int do_smpp_connectTransmitter(SmppConnect *p, int sock_tcp){
    bind_transmitter_t      req;
    bind_transmitter_resp_t res;
    memset(&req, 0, sizeof(bind_transmitter_t));
    memset(&res, 0, sizeof(bind_transmitter_resp_t));

    /* Init PDU ***********************************************************/
    req.command_length   = 0;
    req.command_id       = BIND_TRANSMITTER;
    req.command_status   = ESME_ROK;
    req.sequence_number  = 1;
    sprintf((char*)req.system_id,"%s",(char*)p->systemId);
    sprintf((char*)req.password,"%s",(char*)p->password);
    sprintf((char*)req.system_type,"%s",(char*)p->systemType);
    req.interface_version = SMPP_VERSION;
#if 0 /* if you need add */
    req.addr_ton          = 2;
    req.addr_npi          = 1;
    snprintf( b.address_range, sizeof(b.address_range), "%s", "");
#endif

    /* FIRST STEP: PACK AND SEND */
#include "pack_and_send.inc"
    /* Set a timer (PENDIENTE) ********************************************/
    /* SECOND STEP: READ AND UNPACK Response */
#include "recv_and_unpack.inc"
    destroy_tlv( res.tlv ); /* Por las dudas */

    if( res.command_id != BIND_TRANSMITTER_RESP ||
        res.command_status != ESME_ROK ){
        printf("Error in BIND(BIND_TRANSMITTER)[%d:%d]\n",
                                       res.command_id, res.command_status);
        return( -1 );
    };
    return( 0 );
}


int do_smpp_send_message(SMS *p, int sock_tcp){
    char message[256];
    tlv_t tlv;
    submit_sm_t      req;
    submit_sm_resp_t res;
    memset(&req, 0, sizeof(submit_sm_t));
    memset(&res, 0, sizeof(submit_sm_resp_t));

    /* Init PDU ***********************************************************/
    req.command_length   = 0;
    req.command_id       = SUBMIT_SM;
    req.command_status   = ESME_ROK;
    req.sequence_number  = 2;
    
    req.source_addr_ton  = TON_International;
    req.source_addr_npi  = NPI_ISDN_E163_E164;
    sprintf((char*)req.source_addr,"%s",(char*)p->src);
///printf("................%s",req.source_addr); 
    req.dest_addr_ton    = TON_International;
    req.dest_addr_npi    = NPI_ISDN_E163_E164;
    sprintf((char*)req.destination_addr,"%s",(char*)p->dst);
/*#if 0 // if you need this
    b.esm_class        = 0;
    b.protocol_id      = 0;
    b.priority_flag    = 0;
    snprintf( b.schedule_delivery_time, TIME_LENGTH, "%s", "");
    snprintf( b.validity_period, TIME_LENGTH, "%s", "");
    b.registered_delivery = 0;
    b.replace_if_present_flag =0;
    b.data_coding         = 0;
    b.sm_default_msg_id   = 0;
#endif*/

    sprintf((char*)message,"%s",(char*)p->msg);
/*#if 0 // Message in short_message scope
    b.sm_length           = strlen(TEXTO);
    memcpy(b.short_message, TEXTO, b.sm_length);
//#else // Message in Payload*/
    tlv.tag = TLVID_message_payload;
    tlv.length = strlen(message);
    memcpy(tlv.value.octet, message, tlv.length);
    build_tlv( &(req.tlv), &tlv );
//#endif

    /* Add other optional param */
    tlv.tag = TLVID_user_message_reference;
    tlv.length = sizeof(uint16_t);
    tlv.value.val16 = 0x0024;
    build_tlv( &(req.tlv), &tlv );

    /* FIRST STEP: PACK AND SEND */
#include "pack_and_send.inc"
    destroy_tlv( req.tlv ); /* Por las dudas */
    /* Set a timer (PENDIENTE) ********************************************/
    /* SECOND STEP: READ AND UNPACK Response */
#include "recv_and_unpack.inc"

    if( res.command_id != SUBMIT_SM_RESP ||
        res.command_status != ESME_ROK ){
        printf("Error in send(SUBMIT_SM)[%08X:%08X]\n", 
                                       res.command_id, res.command_status);
        return( -1 );
    };
    return( 0 );
}


SMS* listend_smpp(int sock_tcp, int verbose){
    SMS *sms = (SMS*)malloc(sizeof(SMS));
    int local_buffer_len = 0;
    fd_set input_set;
    struct timeval timeout;
    deliver_sm_t     deliv_sm;
    submit_sm_t      req;
    submit_sm_resp_t res;
    memset(sms, 0, sizeof(SMS));
    memset(&deliv_sm, 0, sizeof(deliver_sm_t));
    memset(&req, 0, sizeof(submit_sm_t));
    memset(&res, 0, sizeof(submit_sm_resp_t));
    timeout.tv_sec  = 0;
    timeout.tv_usec = 250;
    
    FD_ZERO(&input_set);
    FD_SET(sock_tcp,&input_set);
    
    if((ret = select(sock_tcp + 1,&input_set,NULL,NULL,&timeout)) < 0){
	printf("Error in select socket\n");
	return (SMS*)NULL;
    }
    
    if(ret > 0){
    	memset(local_buffer, 0, sizeof(local_buffer));
    	ret = recv(sock_tcp, local_buffer, sizeof(local_buffer),MSG_PEEK);
    	if( ret <= 4 ){
        	printf("Error in recv(PEEK)\n");
    		return (SMS*)NULL;
    	}
    
    	memcpy(&tempo, local_buffer, sizeof(uint32_t)); // get lenght PDU
    	local_buffer_len = ntohl( tempo );
    	ret = recv(sock_tcp, local_buffer, local_buffer_len, 0);
    	if( ret != local_buffer_len ){
        	printf("Error in recv(%d bytes)\n", local_buffer_len);
        	return (SMS*)NULL;
    	}

    	// Print Buffer *******************************************************
    	if(verbose){
        	memset(print_buffer, 0, sizeof(print_buffer));
        	ret = smpp34_dumpBuf(print_buffer, sizeof(print_buffer),
        	                                        local_buffer, local_buffer_len);
        	if( ret != 0 ){ 
			printf("Error in smpp34_dumpBuf():%d:\n%s\n",
        	                       smpp34_errno, smpp34_strerror );
	    		return (SMS*) NULL;
		}
        	printf("-------------------------- RECEIVE --------------------------\n");
        	printf("--- BUFFER \n%s\n", print_buffer);
    	}

    	// unpack PDU *********************************************************
    	ret = smpp34_unpack2((void*)&deliv_sm, local_buffer, local_buffer_len);
	if( ret != 0){
		printf( "Error in smpp34_unpack():%d:%s\n",
        	                    smpp34_errno, smpp34_strerror);
		return (SMS*)NULL;
    	}

    	// Print PDU **********************************************************
    	if(verbose){
       		memset(print_buffer, 0, sizeof(print_buffer));
       		ret = smpp34_dumpPdu2(print_buffer, sizeof(print_buffer), (void*)&deliv_sm);
       		if( ret != 0){
		    printf("Error in smpp34_dumpPdu():%d:\n%s\n",
       		                               smpp34_errno, smpp34_strerror);
		    return (SMS*) NULL;
       		}
        	printf("--- PDU \n%s\n", print_buffer);
        	printf("-----------------------------------------------------------\n\n");
    	 }

    	if( deliv_sm.command_id != DELIVER_SM || deliv_sm.command_status != ESME_ROK ){
        	printf("Error in DELIVER_SM[%d:%d]\n", 
                	                       deliv_sm.command_id, deliv_sm.command_status);
        	return (SMS*)NULL;
    	}

	//copie du SMS
	if(req.source_addr){
		sms->src = (char*)malloc(sizeof(char)*(strlen((char*)deliv_sm.source_addr)+1));
		memset(sms->src,0,(strlen((char*)deliv_sm.source_addr)+1));
		strcpy((char*)sms->src,(char*)deliv_sm.source_addr);
	}
	if(req.destination_addr){
		sms->dst = (char*)malloc(sizeof(char)*(strlen((char*)deliv_sm.destination_addr)+1));
		memset(sms->dst,0,(strlen((char*)deliv_sm.destination_addr)+1));
		strcpy((char*)sms->dst,(char*)deliv_sm.destination_addr);
	}
	if(req.short_message){
		sms->msg = (char*)malloc(sizeof(char)*(strlen((char*)deliv_sm.short_message)+1));
		memset(sms->msg,0,(strlen((char*)deliv_sm.short_message)+1));
		strcpy((char*)sms->msg,(char*)deliv_sm.short_message);
	}
	if(!sms->src || !sms->dst || !sms->msg){
		printf("Error SMS no valid");
		return (SMS*)NULL;
	}
	return (SMS*)sms;
    }
    return (SMS*)NULL;
}

int do_smpp_close(int sock_tcp){
    unbind_t      req;
    unbind_resp_t res;
    memset(&req, 0, sizeof(unbind_t));
    memset(&res, 0, sizeof(unbind_resp_t));

    /* Init PDU ***********************************************************/
    req.command_length   = 0;
    req.command_id       = UNBIND;
    req.command_status   = ESME_ROK;
    req.sequence_number  = 3;

    /* FIRST STEP: PACK AND SEND */
#include "pack_and_send.inc"
    /* Set a timer (PENDIENTE) ********************************************/
    /* SECOND STEP: READ AND UNPACK Response */
#include "recv_and_unpack.inc"

    if( res.command_id != UNBIND_RESP ||
        res.command_status != ESME_ROK ){
        printf("Error in send(UNBIND)[%d:%d]\n", 
                                       res.command_id, res.command_status);
        return( -1 );
    };

    return( 0 );
}
