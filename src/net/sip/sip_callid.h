
#ifndef SIP_CALL_ID_H
#define SIP_CALL_ID_H

#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

int init_callid(char *ip_host);

int generate_callid(char **str_out);

#endif /*SIP_CALL_ID_H*/

