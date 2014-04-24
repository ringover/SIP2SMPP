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
		printf("Failed to create connection.\n");
                return -1;	
	}
	if(strcmp(dbmsIni.dbms_name,"sqlite3")!=0){
		printf("Only SQLite3 is supported.\n");
		return -1;
	}
	printf("Connection to %s server...\n",dbmsIni.dbms_name);

	conn = dbi_conn_new(dbmsIni.dbms_name);
	
	if(conn == NULL) {
		printf("Failed to create connection.\n");
		return -1;
	}
	
	dbi_conn_error_handler(conn, db_error_func, NULL);
	
/*	if(strcmp(dbmsIni.dbms_name,"mysql")==0){
		dbi_conn_set_option(conn, "host"    , dbmsIni.db_host);
		dbi_conn_set_option(conn, "username", dbmsIni.db_username);
		dbi_conn_set_option(conn, "password", dbmsIni.db_password);
		dbi_conn_set_option(conn, "dbname"  , dbmsIni.db_basename);
		dbi_conn_set_option(conn, "encoding", dbmsIni.db_encoding);
	}else if(strcmp(dbmsIni.dbms_name,"sqlite3")==0){*/
		dbi_conn_set_option(conn, "sqlite3_dbdir" , dirname(dbmsIni.db_dirname));
		dbi_conn_set_option(conn, "dbname"        , basename(dbmsIni.db_basename));
//	}
	
	if(dbi_conn_connect(conn) < 0){
		return -1;
	}
	
	return db_prepare();
}

int db_close(void){
	dbi_conn_close(conn);
	dbi_shutdown();
	return 0;
}

int db_prepare(void){
	dbi_result result;
	int i;
	
	for(i=0;i<ARRAY_SIZE(create_stmts);i++){
		result = dbi_conn_query(conn, create_stmts[i]);
		if(!result){
			printf("Failed to create some table : %s\n", create_stmts[i]);
			return -1;
		}/*else{
			printf("%s\n",create_stmts[i]);
		}*/
		dbi_result_free(result);
	}

	return 0;
}

/*unsigned int unique_id(void){
	struct timeval time;
	gettimeofday(&time,NULL);
	return labs((time.tv_sec)+(time.tv_usec*rand()));
}*/

/**
 * Function Query
 */

int db_select_sms_send(db_type type, db_pending pending, SMS *sms){
	dbi_result result = NULL;

	if(!sms){
		printf("SMS failed ... The query does not apply\n");
		return -1;
	}
	
	result = dbi_conn_queryf(conn, query_select_sms_send, type, pending);
	if(!result){
		printf("Query failed ...\n");
		return -1;
	}
	
	if(dbi_result_next_row(result)){
		sms->src = dbi_result_get_string_copy(result,"src");
		sms->dst = dbi_result_get_string_copy(result,"dst");
		sms->msg = dbi_result_get_string_copy(result,"msg");
		sms->ttl = dbi_result_get_int(result,"ttl");
		sms->id  = dbi_result_get_int(result,"id");
		printf("id: %d | pending: %d |%s -> %s : %s\n",sms->id,pending,sms->src,sms->dst,sms->msg);
	}
	
	dbi_result_free(result);
	return 0;
}

SMS* db_select_sms_send_id(int id){
	dbi_result result = NULL;
	SMS *sms = (SMS*)malloc(sizeof(SMS)*1);
	memset(sms,0,sizeof(SMS)*1);
	
	if(id<=0){
		printf("ID wrong ... The query does not apply\n");
		free(sms);
		return NULL;
	}

	result = dbi_conn_queryf(conn, query_select_sms_send_id, id);
	if(!result){
		printf("Query failed ...\n");
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
		printf("id: %d | pending: %d |%s -> %s : %s\n",sms->id,pending,sms->src,sms->dst,sms->msg);
	}
	
	dbi_result_free(result);
	return sms;
}

int db_count_sms_send(db_type type){
	dbi_result result = NULL;
	int count = 0;

	if(!(type == DB_TYPE_SIP || type == DB_TYPE_SMPP)){
		printf("SMS failed ... The query does not apply\n");
		return -1;
	}
	
	result = dbi_conn_queryf(conn, query_count_sms_send, type);
	if(!result){
		printf("Query failed ...\n");
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
		printf("SMS failed ... The query does not apply\n");
		return -1;
	}
	
	result = dbi_conn_queryf(conn,query_insert_sms_send,type,dbmsIni.db_ttl_sms,pending,src,dst,msg);
	
	if(!result){
		printf("Query failed...\n");
	}

	dbi_result_free(result);
	return 0;
}

int db_update_sms_send(db_pending pending, SMS *sms){
	dbi_result result;
	
	printf("db_ttl_sms = %s\n",dbmsIni.db_ttl_sms);

	if(strcmp(dbmsIni.db_ttl_sms,"0") != 0){
		if(--(sms->ttl) <= 0){
			printf("SMS_TTL kill\n");
			return db_delete_sms_send(sms);
		}
	}
	if(!sms || (sms->id)==0){
		printf("SMS failed ... The query does not apply\n");
		return -1;
	}
	
	//sleep(5000*sms->ttl) ???

	result = dbi_conn_queryf(conn,query_update_sms_send,pending,sms->ttl,sms->id);

	if(!result){
                printf("Query failed...\n");
		return -1;
        }

	dbi_result_free(result);
	return 0;
}

int db_delete_sms_send(const SMS *sms){
	dbi_result result;
	
	if(!sms || (sms->id)==0){
		printf("SMS failed ... The query does not apply\n");
		return -1;
	}

	result = dbi_conn_queryf(conn,query_delete_sms_send,sms->id);

	if(!result){
                printf("Query failed...\n");
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
	
	printf("The DB have %lu table(s)\n",ARRAY_SIZE(create_stmts));
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

