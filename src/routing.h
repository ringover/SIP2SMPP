#ifndef ROUTING_H
#define ROUTING_H

#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <sys/time.h>

#include "sms_struct.h"
#include "log/log.h"
#include "type_projet.h"
#include "ini/iniFile.h"

int start_routing();

int routing(const unsigned char *profile_name, const unsigned char *origin_ip, const unsigned int *origin_port, const unsigned char *msisdn_src, const unsigned char *msisdn_dst, const unsigned char *message);

int close_routing();

#endif /*ROUTING_H*/

