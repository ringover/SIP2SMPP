/**
*	.cppteSipMessage permet creer des trames SIP
*
*	@author : Mickael ROUSSE
*/
#ifdef __cplusplus
extern "C"{
#endif

#ifndef CREATE_MESSAGE_SIP_H
#define CREATE_MESSAGE_SIP_H

#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <string.h>

int create_trame_sip_202_accepted(uint8_t **str_trame, uint8_t *ip_host, uint32_t port_host, uint8_t *ip_remote, uint32_t port_remote, uint8_t *user_from, uint8_t *user_to);

int create_trame_sip_message(uint8_t **str_trame, uint8_t *ip_host, uint32_t port_host, uint8_t *ip_remote, uint32_t port_remote, uint8_t *user_from, uint8_t *user_to, uint8_t *msg);

uint8_t* create_trame_sip_200_ok(uint8_t *ip_dst, uint8_t *port_dst, uint8_t *ip_src, 
                    uint8_t *port_src, uint8_t *user_from, uint8_t *user_to, uint8_t *call_id);

uint8_t* get_call_id(uint8_t *sip_msg);

#endif

#ifdef __cplusplus
}
#endif

