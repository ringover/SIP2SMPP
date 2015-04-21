/////////////////////////////
//  SCRIPT ROUTING SAMPLE  //
/////////////////////////////

#include "mod_routing.h"

//interface name (cf INI file)
#define I_CONNECTION_SMPP "smpp_c01"
#define I_LISTEN_SIP      "sip_02"

#define URL_MySQL "mysql://sbc:sbcsbc@127.0.0.1/sbc"

/**
 * Global Variables
 */

typedef int (*p_send_sms_to_smpp)(unsigned char* interface_name, sm_data_t *p_sm);
typedef int (*p_send_sms_to_sip)(unsigned char *interface_name, sm_data_t *p_sm, unsigned char *ip_remote, unsigned int port_remote);

static p_send_sms_to_smpp r_send_sms_to_smpp;
static p_send_sms_to_sip r_send_sms_to_sip;
static map* r_cfg_sip; 
static map* r_cfg_smpp;
//map* cfg_sigtran; //TODO

/**
 * Local Variables
 */

static ConnectionPool_T mysql_pool;

//static char *smpp_ip = NULL;

int start_routing(void **functions, void **cfgs){
    if(!functions){
        ERROR(LOG_SCREEN, "functions parametter is empty");
        return (int) -1;
    }
    if(!cfgs){
        ERROR(LOG_SCREEN, "cfgs parametter is empty");
        return (int) -1;
    }
    r_send_sms_to_smpp = (p_send_sms_to_smpp)functions[0]; //send_sms_to_client_smpp
    r_send_sms_to_sip = (p_send_sms_to_sip)functions[1]; //send_sms_to_sip
    r_cfg_smpp = (map*)cfgs[1]; //cfg_smpp
    r_cfg_sip = (map*)cfgs[2]; //cfg_sip
    //cfg_sigtran = cfg[3]; //cfg_sigtran
    
    URL_T url = URL_new(URL_MySQL);
    mysql_pool = ConnectionPool_new(url);
    ConnectionPool_start(mysql_pool);
    return 0;
}

int routing(const unsigned char *interface_name, const unsigned char *origin_ip, const unsigned int *origin_port, sm_data_t *p_sm){
    Connection_T con = NULL;
    int res = -1;
    DEBUG(LOG_SCREEN, "from = %s - to = %s - msg = %s", p_sm->src, p_sm->dst, p_sm->msg)
    if(strcmp(interface_name, I_CONNECTION_SMPP) == 0){
        //send to SIP interface
        char *r_user = NULL;
        char *r_ip   = NULL;
        unsigned int   r_port = NULL;
//        res = send_sms_to_sip(I_LISTEN_SIP, p_sm, "192.168.1.51", 5075);
//        return (int) 0;
        //Get routing
        TRY
            con = ConnectionPool_getConnection(mysql_pool);
            ResultSet_T result = Connection_executeQuery(con, "SELECT dest FROM routes WHERE number = '%s' LIMIT 1", p_sm->dst);
            DEBUG(LOG_SCREEN, "SELECT dest FROM routes WHERE number = '%s' LIMIT 1", p_sm->dst)
    	    if(ResultSet_next(result)){
                const char *const_tmp = ResultSet_getStringByName(result, "dest");
                char *pos_a = NULL;
                char *pos_b = NULL;
                if(const_tmp == NULL){
                    ERROR(LOG_SCREEN | LOG_FILE, "Routing error because SELECT empty")
                }else{
                    DEBUG(LOG_SCREEN, "Dest = %s", const_tmp)
                }
                //sample : sip:10176502000@213.144.188.141:5080
                //get user
                pos_a = strstr(const_tmp,"@");
                int size = 0;
                if(strncmp(const_tmp, "sip:", 4) == 0){
                    size = pos_a - (const_tmp+4);
                }else{
                    size = pos_a - const_tmp;
                }
                r_user = (char*)calloc(size+1, sizeof(char));
                strncpy(r_user,const_tmp+4,size);
//                DEBUG(LOG_SCREEN, "r_user = %s", r_user)
                pos_a++;
                //get ip
                pos_b = strstr(pos_a, ":");
                size = pos_b - pos_a;
                r_ip = (char*)calloc(size+1, sizeof(char));
                strncpy(r_ip, pos_a, size);
//                DEBUG(LOG_SCREEN, "r_ip = %s", r_ip)
                pos_b++;
                //get port
                r_port = atoi(pos_b);
//                DEBUG(LOG_SCREEN, "r_port = %d", r_port)
            }
        CATCH(SQLException)
            ERROR(LOG_SCREEN | LOG_FILE,"SELECT MySQL Routing failed");
        END_TRY;
        Connection_close(con);
        //Send sip message

        DEBUG(LOG_SCREEN, "%d Routing to %s@%s:%d", p_sm->dst, r_user, r_ip, r_port)
        
        free(p_sm->dst); p_sm->dst = r_user;
        //res = send_sms_to_sip(I_LISTEN_SIP, p_sm->src, r_user, p_sm->message, r_ip, r_port);
        res = r_send_sms_to_sip(I_LISTEN_SIP, p_sm, r_ip, r_port);
        free(r_ip);
        //Set LOG
        TRY
            con = ConnectionPool_getConnection(mysql_pool);
            Connection_execute(con, "INSERT INTO logs_sms (user_from, user_to, server_from, server_to) VALUES ('%s', '%s', '%s', '%s')", p_sm->src, r_user, origin_ip, r_ip);
        CATCH(SQLException)
            ERROR(LOG_SCREEN | LOG_FILE, "LOG SMS Failed");
        END_TRY;
        Connection_close(con);
    }else if(strcmp(interface_name, I_LISTEN_SIP) == 0){
        //send to SMPP interface
        res = r_send_sms_to_smpp(I_CONNECTION_SMPP, p_sm);
        //Set LOG
        TRY
            con = ConnectionPool_getConnection(mysql_pool);
            Connection_execute(con, "INSERT INTO logs_sms (user_from, user_to, server_from, server_to) VALUES ('%s', '%s', '%s', '%s')", p_sm->src, p_sm->dst, origin_ip, "81.201.82.10:2777" );
        CATCH(SQLException)
            ERROR(LOG_SCREEN | LOG_FILE, "LOG SMS Failed");
        END_TRY;
        Connection_close(con);
    }
    return (int) res;
}

int close_routing(){
    ConnectionPool_free(&mysql_pool);
    return 0;
}

