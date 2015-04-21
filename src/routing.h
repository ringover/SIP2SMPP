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
#include "sm_struct.h"
//INI FILE
//#include "ini/iniFile.h"
#include "config/config.h"

int default_start_routing(void **functions, void **cfgs);

int default_routing(const unsigned char *interface_name, const unsigned char *origin_ip, const unsigned int *origin_port, sm_data_t *p_sm);

int default_close_routing();

#endif /*ROUTING_H*/

