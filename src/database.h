/**
*       DataBase
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
#include <sys/time.h>
#include <math.h>
#include <dbi/dbi.h>

#include "smpp/structSMPP.h"
#include "log/log.h"
#include "type_projet.h"
#include "ini/iniFile.h"

typedef enum _db_type{
        DB_TYPE_SMPP = 1,
        DB_TYPE_SIP  = 2
}db_type;

typedef enum _db_pending{
        DB_PENDING_FALSE = 0,
        DB_PENDING_TRUE  = 1
}db_pending;

/**
 * The connection to DBMS
 */
static dbi_conn *conn    = NULL;

/**
 * Statement DB
 */
static char *create_stmts = {
        "CREATE TABLE IF NOT EXISTS sms_send ("
                "id INTEGER PRIMARY KEY, "
                "type TINYINT, "                //type         : 1=SMPP    | 2=SIP
		"ttl TINYINT, "			//time to live : 3 by default 
                "pending TINYINT, "             //pending      : 0=no_send | 1=send_in_progress
                "src VARCHAR(50), "
                "dst VARCHAR(50), "
                "msg VARCHAR(200)"
                ")"
        };


/**
 * Function DB
 */
void db_error_func(dbi_conn conn, void *data);
int db_init(void);
int db_close(void);
int db_prepare(void);

/**
 * Function Query
 */
int db_select_sms_send(db_type type, db_pending pending,SMS *sms);
SMS* db_select_sms_send_id(int id);
int db_insert_sms_send(db_pending pending, db_type type, const char *src, const char* dst, const char *msg);
int db_update_sms_send(db_pending pending, SMS *sms);
int db_delete_sms_send(const SMS *sms);

/**
 * Function used
 */
int sms_set(db_type type, const char *src, const char* dst, const char *msg);
int sms_get(db_type type, SMS *sms);
int sms_count(db_type type);
int sms_cls(SMS *sms);
int sms_rm(const SMS *sms);

#endif

#ifdef __cplusplus
}
#endif
