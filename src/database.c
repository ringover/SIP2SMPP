
#include "database.h"

//////////////////////////////
// Statement DB
///////////
static char *create_stmts = {
        "CREATE TABLE IF NOT EXISTS sm ("
                "id INTEGER PRIMARY KEY, "
                "call_id text, "
                "sequence_number INTEGER, "
                "interface VARCHAR(64), "
                "ip_origin VARCHAR(16), "
                "port_origin INTEGER, "
                "src VARCHAR(32), "
                "dst VARCHAR(32), "
                "msg TEXT,"
                "creation DATETIME"
                ")"
        };

//////////////////////////////
// Queries
///////////

static char *query_get_sm_by_id = {
        "SELECT * FROM sm WHERE sm.id = %lld"
};

static char *query_get_all_sm = {
        "SELECT * FROM sm"
};

static char *query_get_first_sm = {
        "SELECT * FROM sm LIMIT 1"
};

static char *query_delete_sm_by_id = {
        "DELETE FROM sm WHERE sm.id = %lld"
};

static char *query_insert_sm = {
        "INSERT INTO sm (call_id,sequence_number,interface,ip_origin,port_origin,src,dst,msg,creation) "
        "VALUES('%s',%d,'%s','%s',%d,'%s','%s','%s',date('now'))"
};

static char *query_count_sm = {
        "SELECT COUNT(*) AS count_sm FROM sm"
};

/**
 * Function DB
 */

#define DEF_URL "sqlite://%s?synchronous=%s&heap_limit=%d&foreign_keys=%s&encoding=%s"

static ConnectionPool_T pool = NULL;
static URL_T url             = NULL;
static char *str_url         = NULL;


int db_init(void){
    int ret = 0;

    if(!cfg_sqlite){
        return (int) -1;
    }

    if(!cfg_sqlite->path && !cfg_sqlite->synchronous &&
       !cfg_sqlite->heap_limit && !cfg_sqlite->foreign_keys &&
       !cfg_sqlite->encoding){
        ERROR(LOG_SCREEN | LOG_FILE,"Failed to create connection.");
        return (int) -1;	
    }

    str_url = (char*)calloc(strlen((char*)DEF_URL) + strlen((char*)cfg_sqlite->path) + 128,sizeof(char));
    sprintf(str_url, DEF_URL, cfg_sqlite->path, cfg_sqlite->synchronous, cfg_sqlite->heap_limit, cfg_sqlite->foreign_keys, cfg_sqlite->encoding);

    url  = URL_new(str_url);
    pool = ConnectionPool_new(url);
    ConnectionPool_start(pool);
    ret = db_prepare();
    
    INFO(LOG_SCREEN,"Connection to SQLite3 DB is established.");
    return (int) ret;
}

int db_close(void){
    ConnectionPool_free(&pool);
    URL_free(&url);
    free(str_url);
    return 0;
}

int db_prepare(void){
    int ret = 0;
    Connection_T con = ConnectionPool_getConnection(pool);
    TRY
        Connection_execute(con, create_stmts);
    CATCH(SQLException)
        ERROR(LOG_SCREEN | LOG_FILE,"The statement of the table is failed");
        ret = -1;
    END_TRY;
    Connection_close(con);
    return (int) ret;
}

/**
 * Function Query
 */

long long int db_insert_sm(unsigned char *call_id, unsigned int sequence_number, unsigned char *interface, unsigned char *ip_origin, unsigned int port_origin, unsigned char *msisdn_src, unsigned char *msisdn_dst, unsigned char *msg){
    int ret = 0;
    Connection_T con = ConnectionPool_getConnection(pool);
    TRY
        //"INSERT INTO sm (interface,ip_origin,port_origin,ttl,status,src,dst,msg) "
        Connection_execute(con, query_insert_sm, call_id, sequence_number, interface, ip_origin, port_origin, msisdn_src, msisdn_dst, msg);
        ret = Connection_lastRowId(con);
    CATCH(SQLException)
        ERROR(LOG_SCREEN | LOG_FILE,"The insert database is failed");
        ret = -1;
    END_TRY;
    Connection_close(con);
    return (long long int) ret;
}

int db_delete_sm_by_id(long long int id){
    int ret = 0;
    Connection_T con = ConnectionPool_getConnection(pool);
    TRY
        Connection_execute(con, query_delete_sm_by_id, id);
    CATCH(SQLException)
        ERROR(LOG_SCREEN | LOG_FILE,"The delete database is failed");
        ret = -1;
    END_TRY;
    Connection_close(con);
    return (int) ret;
}

int db_select_sm_count(int *count_out){
    int ret = -1;
    if(count_out){
        Connection_T con   = ConnectionPool_getConnection(pool);
        ResultSet_T result = NULL;
        TRY
            result = Connection_executeQuery(con, query_count_sm);
            if(ResultSet_next(result)){
                *count_out = ResultSet_getIntByName(result, "count_sm");
            }else{
                WARNING(LOG_SCREEN | LOG_FILE, "The return select database is empty");
                *count_out = 0;
            }
            ret = 0;
        CATCH(SQLException)
            ERROR(LOG_SCREEN | LOG_FILE, "The select database is failed");
            ret = -1;
        END_TRY;
        Connection_close(con);
    }
    return (int) ret;
}

int db_select_sm_get_by_id(long long int id, unsigned char **call_id, unsigned int *sequence_number, unsigned char **interface, unsigned char **ip_origin, unsigned int *port_origin, unsigned char **msisdn_src, unsigned char **msisdn_dst, unsigned char **msg){
    int ret = 0;
    Connection_T con   = ConnectionPool_getConnection(pool);
    ResultSet_T result = NULL;
    TRY
        //SELECT src, dst, msg, interface, ip_origin, port_origin
        result = Connection_executeQuery(con, query_get_sm_by_id, id);
        if(ResultSet_next(result)){
            const char *const_tmp = NULL;

            if(call_id){
                const_tmp   = ResultSet_getStringByName(result, "call_id");
                *call_id  = (unsigned char*)calloc(strlen((char*)const_tmp), sizeof(char));
                strcpy((char*)*call_id, (char*)const_tmp);
            }

            if(sequence_number){
                *sequence_number  = ResultSet_getIntByName(result, "sequence_number"); 
            }

            if(interface){
                const_tmp   = ResultSet_getStringByName(result, "interface");
                *interface  = (unsigned char*)calloc(strlen((char*)const_tmp), sizeof(char));
                strcpy((char*)*interface, (char*)const_tmp);
            }

            if(ip_origin){
                const_tmp   = ResultSet_getStringByName(result, "ip_origin");
                *ip_origin  = (unsigned char*)calloc(strlen((char*)const_tmp), sizeof(char));
                strcpy((char*)*ip_origin, (char*)const_tmp);
            }

            if(port_origin){
                *port_origin = ResultSet_getIntByName(result, "port_origin");
            }

            if(msisdn_src){
                const_tmp   = ResultSet_getStringByName(result, "src");
                *msisdn_src = (unsigned char*)calloc(strlen((char*)const_tmp), sizeof(char));
                strcpy((char*)*msisdn_src, (char*)const_tmp);
            }

            if(msisdn_dst){
                const_tmp   = ResultSet_getStringByName(result, "dst");
                *msisdn_dst = (unsigned char*)calloc(strlen((char*)const_tmp), sizeof(char));
                strcpy((char*)*msisdn_dst, (char*)const_tmp);
            }

            if(msg){
                const_tmp   = ResultSet_getStringByName(result, "msg");
                *msg        = (unsigned char*)calloc(strlen((char*)const_tmp), sizeof(char));
                strcpy((char*)*msg, (char*)const_tmp);
            }
        }else{
            WARNING(LOG_SCREEN | LOG_FILE, "The return select database is empty");
        }
    CATCH(SQLException)
        ERROR(LOG_SCREEN | LOG_FILE, "The select database is failed");
        ret = -1;
    END_TRY;
    Connection_close(con);
    return (int) ret;
}

int db_select_sm_get_first(long long int *id, unsigned char *call_id, unsigned int *sequence_number, unsigned char **interface, unsigned char **ip_origin, unsigned int *port_origin, unsigned char **msisdn_src, unsigned char **msisdn_dst, unsigned char **msg){
    int ret = -1;
    Connection_T con   = ConnectionPool_getConnection(pool);
    ResultSet_T result = NULL;
    TRY
        //SELECT src, dst, msg, interface, ip_origin, port_origin
        result = Connection_executeQuery(con, query_get_first_sm);
        if(ResultSet_next(result)){
            const char *const_tmp = NULL;
            if(id){
                *id         = ResultSet_getLLongByName(result, "id");
            }

            if(call_id){
                const_tmp   = ResultSet_getStringByName(result, "call_id");
                *call_id  = (unsigned char*)calloc(strlen((char*)const_tmp), sizeof(char));
                strcpy((char*)*call_id, (char*)const_tmp);
            }

            if(sequence_number){
                *sequence_number  = ResultSet_getIntByName(result, "sequence_number"); 
            }

            if(interface){
                const_tmp   = ResultSet_getStringByName(result, "interface");
                *interface  = (unsigned char*)calloc(strlen((char*)const_tmp), sizeof(char));
                strcpy((char*)*interface, (char*)const_tmp);
            }

            if(ip_origin){
                const_tmp   = ResultSet_getStringByName(result, "ip_origin");
                *ip_origin  = (unsigned char*)calloc(strlen((char*)const_tmp), sizeof(char));
                strcpy((char*)*ip_origin, (char*)const_tmp);
            }

            if(port_origin){
                *port_origin = ResultSet_getIntByName(result, "port_origin");
            }

            if(msisdn_src){
                const_tmp   = ResultSet_getStringByName(result, "src");
                *msisdn_src = (unsigned char*)calloc(strlen((char*)const_tmp), sizeof(char));
                strcpy((char*)*msisdn_src, (char*)const_tmp);
            }

            if(msisdn_dst){
                const_tmp   = ResultSet_getStringByName(result, "dst");
                *msisdn_dst = (unsigned char*)calloc(strlen((char*)const_tmp), sizeof(char));
                strcpy((char*)*msisdn_dst, (char*)const_tmp);
            }

            if(msg){
                const_tmp   = ResultSet_getStringByName(result, "msg");
                *msg        = (unsigned char*)calloc(strlen((char*)const_tmp), sizeof(char));
                strcpy((char*)*msg, (char*)const_tmp);
            }
        }else{
            WARNING(LOG_SCREEN | LOG_FILE, "The return select database is empty");
        }
    CATCH(SQLException)
        ERROR(LOG_SCREEN | LOG_FILE, "The select database is failed");
        ret = -1;
    END_TRY;
    Connection_close(con);
    return (int) ret;
}

