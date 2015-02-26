/**
 * 
 */
#ifdef __cplusplus
extern "C"{
#endif

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

#include "sms_struct.h"
#include "log/log.h"
#include "type_projet.h"
#include "ini/iniFile.h"

/**
 * The status of SMS to send
 */
typedef enum _db_status{
        PENDING = 0,
        ONGOING = 1
}db_status;

/**
 * Function DB
 */
int db_init(void);
int db_close(void);
int db_prepare(void);

/**
 * Function Query
 */
long long int db_insert_sms(unsigned char *interface, unsigned char *ip_origin, unsigned int port_origin, unsigned char *msisdn_src, unsigned char *msisdn_dst, unsigned char *msg, db_status status, int ttl);
int db_delete_sms_by_id(long long int id);
int db_update_sms_status_by_id(long long int id, db_status new_status);
int db_update_sms_ttl_by_id(long long int id, int new_ttl);
int db_select_sms_count(db_status status, int *count_out);
long long int db_select_sms_get(unsigned char *interface, unsigned char *ip_origin, unsigned int port_origin, unsigned char *msisdn_src, unsigned char *msisdn_dst, unsigned char *msg, db_status status, int *ttl);

#endif

#ifdef __cplusplus
}
#endif

