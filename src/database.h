
#ifndef DATABASE_H
#define DATABASE_H

#include <string.h>
#include <stdio.h>
#include <libgen.h>
#include <stdlib.h>
#include <stdint.h>
#include <sys/time.h>
#include <math.h>

//Lib ZDB
#include "zdb/URL.h"
#include "zdb/ResultSet.h"
#include "zdb/PreparedStatement.h"
#include "zdb/Connection.h"
#include "zdb/ConnectionPool.h"
#include "zdb/SQLException.h"
//#include <zdb/Exception.h>
//#include <zdb/SQLException.h>
//#include <zdb/zdb.h>

//LOG
#include "log/log.h"

//Linked List - MAP
#include "linked_list/map.h"

//INI FILE
#include "config/config.h"

//Other
#include "type_projet.h"

/**
 * config SQLite3
 */

typedef struct _config_sqlite{
    char         *path;           //SQLite3
    char         *encoding;       //default : UTF-8
    char         *synchronous;     //default : normal 
    unsigned long heap_limit;      //default : 8290304 Ko 
    char         *foreign_keys;    //default : on 
} config_sqlite_t;
#define new_config_sqlite()   (config_sqlite_t*)calloc(1, sizeof(config_sqlite_t))

inline void destroy_config_sqlite(config_sqlite_t *sqlite);
void free_config_sqlite(void **sqlite);
inline void display_config_sqlite(config_sqlite_t *sqlite);

extern config_sqlite_t  *cfg_sqlite;

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

