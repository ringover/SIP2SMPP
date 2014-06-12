#include "database.h"

/**
 * Query
 */

static char *query_select_sms_send = {
        "SELECT * FROM sms_send "
        "WHERE type=%d AND pending=%d limit 1"
};

static char *query_count_sms_send = {
        "SELECT COUNT(*) FROM sms_send "
        "WHERE type=%d"
};

static char *query_select_sms_send_id = {
        "SELECT * FROM sms_send "
        "WHERE id=%d"
};

static char *query_insert_sms_send = {
        "INSERT INTO sms_send (type,ttl,pending,src,dst,msg) "
        "VALUES(%d,%d,%d,'%s','%s','%s')"
};

static char *query_update_sms_send = {
        "UPDATE sms_send SET pending=%d, ttl=%d "
        "WHERE id=%d"
};

static char *query_delete_sms_send = {
        "DELETE FROM sms_send "
        "WHERE id=%d"
};

/**
 * Function DB
 */

void db_error_func(dbi_conn conn, void *data){
	const char *msg;
	dbi_conn_error(conn, &msg);
	printf("DBI: %s\n", msg);
	return;
}

int db_init(void){
	dbi_initialize(NULL);
	
	if(!dbmsIni.dbms_name){
		ERROR(LOG_SCREEN | LOG_FILE,"Failed to create connection.");
                return -1;	
	}
	if(strcmp(dbmsIni.dbms_name,"sqlite3")!=0){
		ERROR(LOG_SCREEN | LOG_FILE,"Only SQLite3 is supported.");
		return -1;
	}

	conn = dbi_conn_new(dbmsIni.dbms_name);
	
	if(conn == NULL) {
		ERROR(LOG_SCREEN | LOG_FILE,"Failed to create connection with %s.",dbmsIni.dbms_name);
		INFO( LOG_SCREEN | LOG_FILE,"db_dirname  = %s",dbmsIni.db_dirname);
		INFO( LOG_SCREEN | LOG_FILE,"db_basename = %s",dbmsIni.db_basename);
		return -1;
	}
	
	dbi_conn_error_handler(conn, db_error_func, NULL);

	dbi_conn_set_option(conn, "sqlite3_dbdir" , dirname( dbmsIni.db_dirname ));
	dbi_conn_set_option(conn, "dbname"        , basename(dbmsIni.db_basename));

	if(dbi_conn_connect(conn) < 0){
		return -1;
	}
	
	INFO(LOG_SCREEN,"Connection to %s server is established.",dbmsIni.dbms_name);
	return db_prepare();
}

int db_close(void){
	dbi_conn_close(conn);
	dbi_shutdown();
	return 0;
}

int db_prepare(void){
	dbi_result result;
	
	result = dbi_conn_query(conn, create_stmts);
	if(!result){
	   DEBUG(LOG_SCREEN | LOG_FILE,"Failed to create this table OR already created : %s", create_stmts);
	}
	dbi_result_free(result);

	return 0;
}

/**
 * Function Query
 */

int db_select_sms_send(db_type type, db_pending pending, SMS *sms){
	dbi_result result = NULL;

	if(!sms){
		DEBUG(LOG_SCREEN | LOG_FILE,"SMS failed ... The query does not apply\n");
		return -1;
	}
	
	result = dbi_conn_queryf(conn, query_select_sms_send, type, pending);
	if(!result){
		ERROR(LOG_SCREEN | LOG_FILE,"Query failed ...%s %s %s",query_select_sms_send, type, pending);
		return -1;
	}
	
	if(dbi_result_next_row(result)){
		sms->src = dbi_result_get_string_copy(result,"src");
		sms->dst = dbi_result_get_string_copy(result,"dst");
		sms->msg = dbi_result_get_string_copy(result,"msg");
		sms->ttl = dbi_result_get_int(result,"ttl");
		sms->id  = dbi_result_get_int(result,"id");
		DEBUG(LOG_SCREEN | LOG_FILE,"id: %d | pending: %d |%s -> %s : %s\n",sms->id,pending,sms->src,sms->dst,sms->msg);
	}
	
	dbi_result_free(result);
	return 0;
}

SMS* db_select_sms_send_id(int id){
	dbi_result result = NULL;
	SMS *sms = (SMS*)malloc(sizeof(SMS)*1);
	memset(sms,0,sizeof(SMS)*1);
	
	if(id<=0){
		DEBUG(LOG_SCREEN | LOG_FILE,"ID wrong ... The query does not apply");
		free(sms);
		return NULL;
	}

	result = dbi_conn_queryf(conn, query_select_sms_send_id, id);
	if(!result){
		ERROR(LOG_SCREEN | LOG_FILE,"Query failed ...\n");
		free(sms);
		return NULL;
	}
	
	if(dbi_result_next_row(result)){
		sms->src = dbi_result_get_string_copy(result,"src");
		sms->dst = dbi_result_get_string_copy(result,"dst");
		sms->msg = dbi_result_get_string_copy(result,"msg");
		sms->ttl = dbi_result_get_int(result,"ttl");
		sms->id  = dbi_result_get_int(result,"id");

		int pending = dbi_result_get_int(result,"pending");
		DEBUG(LOG_SCREEN | LOG_FILE,"id: %d | pending: %d |%s -> %s : %s",sms->id,pending,sms->src,sms->dst,sms->msg);
	}
	
	dbi_result_free(result);
	return sms;
}

int db_count_sms_send(db_type type){
	dbi_result result = NULL;
	int count = 0;

	if(!(type == DB_TYPE_SIP || type == DB_TYPE_SMPP)){
		DEBUG(LOG_SCREEN | LOG_FILE,"The query does not apply");
		return -1;
	}
	
	result = dbi_conn_queryf(conn, query_count_sms_send, type);
	if(!result){
		ERROR(LOG_SCREEN | LOG_FILE,"Query failed ...");
		return -1;
	}
	
	if(dbi_result_next_row(result)){
		count = dbi_result_get_longlong(result,"COUNT(*)");
	}
	
	dbi_result_free(result);
	return count;
}

int db_insert_sms_send(db_pending pending, db_type type, const char *src, const char* dst, const char *msg){
	dbi_result result;

	if(!src || !dst || !msg){
		DEBUG(LOG_SCREEN | LOG_FILE,"The query does not apply");
		return -1;
	}
	
	result = dbi_conn_queryf(conn,query_insert_sms_send,type,dbmsIni.db_ttl_sms,pending,src,dst,msg);
	
	if(!result){
		ERROR(LOG_SCREEN | LOG_FILE,"Query failed...");
	}

	dbi_result_free(result);
	return 0;
}

int db_update_sms_send(db_pending pending, SMS *sms){
	dbi_result result;
	
	DEBUG(LOG_SCREEN | LOG_FILE,"db_ttl_sms = %s",dbmsIni.db_ttl_sms);

	if(strcmp(dbmsIni.db_ttl_sms,"0") != 0){
		if(--(sms->ttl) <= 0){
			printf("SMS_TTL kill\n");
			return db_delete_sms_send(sms);
		}
	}
	if(!sms || (sms->id)==0){
		DEBUG(LOG_SCREEN | LOG_FILE,"The query does not apply");
		return -1;
	}
	
	//sleep(5000*sms->ttl) ???

	result = dbi_conn_queryf(conn,query_update_sms_send,pending,sms->ttl,sms->id);

	if(!result){
                ERROR(LOG_SCREEN | LOG_FILE,"Query failed...");
		return -1;
        }

	dbi_result_free(result);
	return 0;
}

int db_delete_sms_send(const SMS *sms){
	dbi_result result;
	
	if(!sms || (sms->id)==0){
		DEBUG(LOG_SCREEN | LOG_FILE,"The query does not apply");
		return -1;
	}

	result = dbi_conn_queryf(conn,query_delete_sms_send,sms->id);

	if(!result){
                ERROR(LOG_SCREEN | LOG_FILE,"Query failed...");
		return -1;
        }

	dbi_result_free(result);
	return 0;
}

/**
 * function used
 */

/**
 * \brief This function allow to insert one SMS in the DB
 *
 * \param type	This parameter is the type of receive SMS (DB_TYPE_SMPP/DB_TYPE_SIP)
 * \param src	This parameter is the MSISDN source
 * \param src	This parameter is the MSISDN destination
 * \param src	This parameter is the SMS message
 */
int sms_set(db_type type, const char *src, const char *dst, const char *msg){
	if(db_insert_sms_send(DB_PENDING_FALSE, type, src, dst, msg)==0){
		return 0;
	}
	return-1;
}

/**
 * \brief This function allow to count the number of SMS in the DB
 *
 * \param type	This parameter is the type of receive SMS (DB_TYPE_SMPP/DB_TYPE_SIP)
 */
int sms_count(db_type type){
	return db_count_sms_send(type);
}

/**
 * \brief
 *
 * \param type	This parameter is the type of receive SMS (DB_TYPE_SMPP/DB_TYPE_SIP)
 * \param sms	This parameter is the SMs struct (out)
 */
int sms_get(db_type type, SMS *sms){
	if(db_select_sms_send(type,DB_PENDING_FALSE,sms)==0){
		if(db_update_sms_send(DB_PENDING_TRUE,sms)==0){
			return 0;
		}
	}
	return -1;
}

/**
 * \brief	This function reset the sms in the base if it is not send
 *		(DB_PENDING_TRUE -> DB_PENDING_FALSE) and decrease of the TTL
 *
 * \param sms   This parameter is the SMs struct (in) 
 */
int sms_cls(SMS *sms){
	if(db_update_sms_send(DB_PENDING_FALSE,sms)==0){
		return 0;
	}
	return -1;
}

/**
 * \brief       This function delete the sms in the base if it is send
 *		(but the SMS pointer is not free)
 *
 * \param sms   This parameter is the SMs struct (in) 
 */
int sms_rm(const SMS *sms){
	if(db_delete_sms_send(sms)==0){
		return 0;
	}
	return -1;
}

/*
//Sample for how to use it
int main(){
	db_host = (char*) malloc(sizeof(char)*20);
	db_host = "localhost";
	db_basename = (char*) malloc(sizeof(char)*20);
	db_basename = "sip2smpp";
	db_username = (char*) malloc(sizeof(char)*20);
	db_username = "root";
	db_password = (char*) malloc(sizeof(char)*20);
	db_password = "5rdpqkq6";
	db_encoding = (char*) malloc(sizeof(char)*20);
	db_encoding = "UTF-8";
	
	printf("The DB have %lu table(s)\n",sizearray(create_stmts));
//	printf("%u\n", unique_id());

	db_init();

	db_insert_sms_send(DB_PENDING_FALSE,DB_TYPE_SMPP,"0624901020","0663335588","blabla");
	
	SMS sms;
	sms.id = 0;

	db_select_sms_send(DB_TYPE_SMPP,DB_PENDING_FALSE,&sms);
	
	db_update_sms_send(DB_PENDING_TRUE,&sms);

	SMS *sms2 = db_select_sms_send_id(sms.id);
	
	db_delete_sms_send(sms2);

	free(sms2);
	free(db_host);
	free(db_basename);
	free(db_username);
	free(db_password);
	free(db_encoding);
	db_close();

	return 0;
}
*/

