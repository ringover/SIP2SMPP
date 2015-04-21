#ifndef ROUTING_MODULE_H
#define ROUTING_MODULE_H

#include "zdb/URL.h"
#include "zdb/ResultSet.h"
#include "zdb/PreparedStatement.h"
#include "zdb/Connection.h"
#include "zdb/ConnectionPool.h"
#include "zdb/SQLException.h"

#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <sys/time.h>

#include "../src/log/log.h"
#include "../src/linked_list/map.h"
#include "../src/type_projet.h"
#include "../src/sm_struct.h"

extern map *cfg_sip;
extern map *cfg_smpp;

extern int send_sms_to_client_smpp(unsigned char* interface_name, sm_data_t *p_sm);
extern int send_sms_to_sip(unsigned char *interface_name, sm_data_t *p_sm, unsigned char *ip_remote, unsigned int port_remote);

int start_routing(void **functions, void **cfgs);

int routing(const unsigned char *interface_name, const unsigned char *origin_ip, const unsigned int *origin_port, sm_data_t *p_sm);

int close_routing();

#endif /*ROUTING_MODULE_H*/

