
#include "database.h"

//////////////////////////////
// Statement DB
///////////
static char *create_stmts = {
        "CREATE TABLE IF NOT EXISTS sms ("
                "id INTEGER PRIMARY KEY, "
                "interface VARCHAR(64), "
                "ip_origin VARCHAR(20), "
                "port_origin INTEGER, "
                "ttl TINYINT, "             //time to live : 3 by default 
                "status TINYINT, "          //PENDING(0) || ONGOING(1)
                "src VARCHAR(64), "
                "dst VARCHAR(64), "
                "msg VARCHAR(1024)"
                ")"
        };

//////////////////////////////
// Queries
///////////

static char *query_get_sms_by_status = {
        "SELECT * FROM sms WHERE status = %d LIMIT 1"
};

static char *query_update_ttl_sms_by_id = {
        "UPDATE sms SET ttl=%d WHERE sms.id %lld"
};

static char *query_update_status_sms_by_id = {
        "UPDATE sms SET status=%d WHERE sms.id = %lld"
};

static char *query_delete_sms_by_id = {
        "DELETE FROM sms WHERE sms.id = %lld"
};

static char *query_insert_sms = {
        "INSERT INTO sms (interface,ip_origin,port_origin,ttl,status,src,dst,msg) "
        "VALUES('%s','%s',%d,%d,%d,'%s','%s','%s')"
};
static char *query_count_sms = {
        "SELECT COUNT(*) AS count_sms FROM sms "
        "WHERE status=%d"
};

/**
 * Function DB
 */

#define DEF_URL "sqlite://%s?synchronous=normal&heap_limit=8096000&foreign_keys=on"
//#define DEF_URL "sqlite://%s?synchronous=%s&heap_limit=%d&foreign_keys=%s&encoding=%s"
static ConnectionPool_T pool = NULL;
static URL_T url             = NULL;
static char *str_url         = NULL;


int db_init(void){
    int ret = 0;
    if(!dbms_ini.dbms_name && !dbms_ini.db_path){
        ERROR(LOG_SCREEN | LOG_FILE,"Failed to create connection.");
        return -1;	
    }

    if(strcmp(dbms_ini.dbms_name,"sqlite3")!=0){
        ERROR(LOG_SCREEN | LOG_FILE,"Only SQLite3 is supported.");
        return -1;
    }

    str_url = (char*)calloc(strlen((char*)DEF_URL)+strlen((char*)dbms_ini.db_path),sizeof(char));
    sprintf(str_url, DEF_URL, dbms_ini.db_path);
    url  = URL_new(str_url);
    pool = ConnectionPool_new(url);
    ConnectionPool_start(pool);
    ret = db_prepare();
    
    INFO(LOG_SCREEN,"Connection to %s server is established.",dbms_ini.dbms_name);
    return ret;
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

long long int db_insert_sms(unsigned char *interface, unsigned char *ip_origin, unsigned int port_origin, unsigned char *msisdn_src, unsigned char *msisdn_dst, unsigned char *msg, db_status status, int ttl){
    int ret = 0;
    Connection_T con = ConnectionPool_getConnection(pool);
    TRY
        //"INSERT INTO sms (interface,ip_origin,port_origin,ttl,status,src,dst,msg) "
        Connection_execute(con, query_insert_sms, interface, ip_origin, port_origin, ttl, (int)status, msisdn_src, msisdn_dst, msg);
        ret = Connection_lastRowId(con);
    CATCH(SQLException)
        ERROR(LOG_SCREEN | LOG_FILE,"The insert database is failed");
        ret = -1;
    END_TRY;
    Connection_close(con);
    return (long long int) ret;
}

int db_delete_sms_by_id(long long int id){
    int ret = 0;
    Connection_T con = ConnectionPool_getConnection(pool);
    TRY
        Connection_execute(con, query_delete_sms_by_id, id);
    CATCH(SQLException)
        ERROR(LOG_SCREEN | LOG_FILE,"The delete database is failed");
        ret = -1;
    END_TRY;
    Connection_close(con);
    return (int) ret;
}

int db_update_sms_status_by_id(long long int id, db_status new_status){
    int ret = 0;
    Connection_T con = ConnectionPool_getConnection(pool);
    TRY
        Connection_execute(con, query_update_status_sms_by_id, (int)new_status, id);
    CATCH(SQLException)
        ERROR(LOG_SCREEN | LOG_FILE,"The update database is failed");
        ret = -1;
    END_TRY;
    Connection_close(con);
    return (int) ret;
}

int db_update_sms_ttl_by_id(long long int id, int new_ttl){
    int ret = 0;
    if(new_ttl > 0){
        Connection_T con = ConnectionPool_getConnection(pool);
        TRY
            Connection_execute(con, query_update_ttl_sms_by_id, new_ttl, id);
        CATCH(SQLException)
            ERROR(LOG_SCREEN | LOG_FILE,"The update database is failed");
            ret = -1;
        END_TRY;
        Connection_close(con);
    }else{
        db_delete_sms_by_id(id);
    }
    return (int) ret;
}

int db_select_sms_count(db_status status, int *count_out){
    int ret = -1;
    if(count_out){
        Connection_T con   = ConnectionPool_getConnection(pool);
        ResultSet_T result = NULL;
        TRY
            result = Connection_executeQuery(con, query_count_sms, (int)status);
            if(ResultSet_next(result)){
                *count_out = ResultSet_getIntByName(result, "count_sms");
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

long long int db_select_sms_get(unsigned char *interface, unsigned char *ip_origin, unsigned int port_origin, unsigned char *msisdn_src, unsigned char *msisdn_dst, unsigned char *msg, db_status status, int *ttl){
    int ret = 0;
    Connection_T con   = ConnectionPool_getConnection(pool);
    ResultSet_T result = NULL;
    TRY
        //SELECT src, dst, msg, interface, ip_origin, port_origin
        result = Connection_executeQuery(con, query_get_sms_by_status, (int)status);
        if(ResultSet_next(result)){
            const char *const_tmp = NULL;
            ret         = ResultSet_getIntByName(result, "id");

            *ttl        = ResultSet_getIntByName(result, "ttl");

            const_tmp   = ResultSet_getStringByName(result, "interface");
            interface   = (unsigned char*)calloc(strlen((char*)const_tmp), sizeof(char));
            strcpy((char*)interface, (char*)const_tmp);

            const_tmp   = ResultSet_getStringByName(result, "ip_origin");
            ip_origin   = (unsigned char*)calloc(strlen((char*)const_tmp), sizeof(char));
            strcpy((char*)ip_origin, (char*)const_tmp);

            port_origin = ResultSet_getIntByName(result, "port_origin");

            const_tmp   = ResultSet_getStringByName(result, "src");
            msisdn_src  = (unsigned char*)calloc(strlen((char*)const_tmp), sizeof(char));
            strcpy((char*)msisdn_src, (char*)const_tmp);

            const_tmp   = ResultSet_getStringByName(result, "dst");
            msisdn_dst  = (unsigned char*)calloc(strlen((char*)const_tmp), sizeof(char));
            strcpy((char*)msisdn_dst, (char*)const_tmp);

            const_tmp   = ResultSet_getStringByName(result, "msg");
            msg         = (unsigned char*)calloc(strlen((char*)const_tmp), sizeof(char));
            strcpy((char*)msg, (char*)const_tmp);
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

/*
int main(){
    db_init();

    long long int id = db_insert_sms("SIP_IN_01", "192.168.10.1", 5090, "0630566333", "0624901020", "Msg de test assez court", PENDING, 5);
    printf("id = %lld", id);
    id = db_insert_sms("SIP_IN_01", "192.168.10.1", 5090, "0630566333", "0624901020", "Msg de test2 assez court", PENDING, 5);
    printf("id = %lld", id);

    db_close();
}
*/
/*
static char *query_get_id_sms = {
        "SELECT id FROM sms WHERE sms.src LIKE \"%s\" "
        "AND sms.dst LIKE \"%s\" AND sms.msg LIKE \"%s\" LIMIT 1"
};

static char *query_get_info_sms = {
        "SELECT * FROM sms WHERE sms.src LIKE \"%s\" "
        "AND sms.dst LIKE \"%s\" AND sms.msg LIKE \"%s\" LIMIT 1"
};

static char *query_get_all_sms = {
        "SELECT * FROM sms"
};

static char *query_get_all_sms_by_status_and_interface = {
        "SELECT * FROM sms "
        "WHERE interface=\"%s\" AND status=%d limit 1"
};

static char *query_get_sms_by_status = {
        "SELECT * FROM sms WHERE status = %d LIMIT 1"
};

static char *query_get_all_sms_by_status = {
        "SELECT * FROM sms WHERE status = %d"
};

static char *query_get_sms_by_id = {
        "SELECT * FROM sms WHERE id = %d LIMIT 1"
};

static char *query_delete_sms = {
        "DELETE FROM sms WHERE sms.src LIKE \"%s\" "
        "AND sms.dst LIKE \"%s\" AND sms.msg LIKE \"%s\" AND sms.status = %d LIMIT 1"
};

static char *query_update_ttl_sms = {
        "UPDATE sms SET ttl=%d WHERE sms.src LIKE \"%s\" "
        "AND sms.dst LIKE \"%s\" AND sms.msg LIKE \"%s\" AND sms.status = %d LIMIT 1"
};
static char *query_update_status_sms = {
        "UPDATE sms SET status=%d WHERE sms.src LIKE \"%s\" "
        "AND sms.dst LIKE \"%s\" AND sms.msg LIKE \"%s\" AND sms.status = %d LIMIT 1"
};
*/

/*
int db_delete_sms(unsigned char *msisdn_src, unsigned char *msisdn_dst, unsigned char *msg, db_status status){
    int ret = 0;
    Connection_T con = ConnectionPool_getConnection(pool);
    TRY
        Connection_execute(con, query_delete_sms, msisdn_src, msisdn_dst, msg, (int)status);
    CATCH(SQLException)
        ERROR(LOG_SCREEN | LOG_FILE,"The delete database is failed");
        ret = -1;
    END_TRY;
    Connection_close(con);
    return (int) ret;
}

int db_update_sms_status(unsigned char *msisdn_src, unsigned char *msisdn_dst, unsigned char *msg, db_status old_status, db_status new_status){
    int ret = 0;
    Connection_T con = ConnectionPool_getConnection(pool);
    TRY
        Connection_execute(con, query_update_status_sms, (int)new_status, msisdn_src, msisdn_dst, msg, (int)old_status);
    CATCH(SQLException)
        ERROR(LOG_SCREEN | LOG_FILE,"The update database is failed");
        ret = -1;
    END_TRY;
    Connection_close(con);
    return (int) ret;
}

int db_update_sms_ttl(unsigned char *msisdn_src, unsigned char *msisdn_dst, unsigned char *msg, db_status status, int new_ttl){
    int ret = 0;
    Connection_T con = ConnectionPool_getConnection(pool);
    TRY
        Connection_execute(con, query_update_ttl_sms, (int)new_ttl, msisdn_src, msisdn_dst, msg, (int)status);
    CATCH(SQLException)
        ERROR(LOG_SCREEN | LOG_FILE,"The update database is failed");
        ret = -1;
    END_TRY;
    Connection_close(con);
    return (int) ret;
}

*/
