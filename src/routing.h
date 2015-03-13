#ifndef ROUTING_H
#define ROUTING_H

#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <sys/time.h>

#include "sip_io.h"
#include "smpp_io.h"

#include "log/log.h"
#include "type_projet.h"
#include "ini/iniFile.h"
#include "sm_struct.h"

int start_routing();

int routing(const unsigned char *interface_name, const unsigned char *origin_ip, const unsigned int *origin_port, sm_data_t *p_sm);

int close_routing();

#endif /*ROUTING_H*/

