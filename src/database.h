
#ifndef DATABASE_H
#define DATABASE_H

#include <string.h>
#include <stdio.h>
#include <libgen.h>
#include <stdlib.h>
#include <stdint.h>
#include <sys/time.h>
#include <math.h>


#include "zdb/URL.h"
#include "zdb/ResultSet.h"
#include "zdb/PreparedStatement.h"
#include "zdb/Connection.h"
#include "zdb/ConnectionPool.h"
#include "zdb/SQLException.h"

//#include <zdb/Exception.h>
//#include <zdb/SQLException.h>
//#include <zdb/zdb.h>

#include "log/log.h"
#include "type_projet.h"
#include "ini/iniFile.h"

/**
 * Function DB
 */
int db_init(void);
int db_close(void);
int db_prepare(void);

/**
 * Function Query
 */
long long int db_insert_sm(unsigned char *call_id, unsigned int sequence_number, unsigned char *interface, unsigned char *ip_origin, unsigned int port_origin, unsigned char *msisdn_src, unsigned char *msisdn_dst, unsigned char *msg);
int db_delete_sm_by_id(long long int id);
int db_select_sm_count(int *count_out);
int db_select_sm_get_by_id(long long int id, unsigned char **call_id, unsigned int *sequence_number, unsigned char **interface, unsigned char **ip_origin, unsigned int *port_origin, unsigned char **msisdn_src, unsigned char **msisdn_dst, unsigned char **msg);
int db_select_sm_get_first(long long int *id, unsigned char *call_id, unsigned int *sequence_number, unsigned char **interface, unsigned char **ip_origin, unsigned int *port_origin, unsigned char **msisdn_src, unsigned char **msisdn_dst, unsigned char **msg);



#endif

