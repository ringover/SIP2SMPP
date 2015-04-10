#ifndef CONFIG_STRUCT_H
#define CONFIG_STRUCT_H

#include "../linked_list/map.h"
#include "../net/smpp/libsmpp34/smpp34.h"
#include "../str.h"

#define STR_LIST_DELIMITER          ","

#define STR_BIND_TRANSCEIVER        "TRANSCEIVER"
#define STR_BIND_RECEIVER           "RECEIVER"
#define STR_BIND_TRANSMITTER        "TRANSMITTER"

#define STR_TON_Unknown             "UNKNOWN"
#define STR_TON_International       "INTERNATIONAL"
#define STR_TON_National            "NATIONAL"
#define STR_TON_Network_Specific    "NETWORK_SPECIFIC"
#define STR_TON_Subscriber_Number   "SUBSCRIBER_NUMBER"
#define STR_TON_Alphanumeric        "ALPHANUMERIC"
#define STR_TON_Abbreviated         "ABBREVIATED"

#define STR_NPI_Unknown             "UNKNOWN"
#define STR_NPI_ISDN_E163_E164      "ISDN_E163_E164"
#define STR_NPI_Data_X121           "DATA_X121"
#define STR_NPI_Telex_F69           "TELEX_F69"
#define STR_NPI_Land_Mobile_E212    "LAND_MOBILE_E212"
#define STR_NPI_National            "NATIONAL"
#define STR_NPI_Private             "PRIVATE"
#define STR_NPI_ERMES               "ERMES"
#define STR_NPI_Internet_IP         "INTERNET_IP"
#define STR_NPI_WAP_Client_Id       "WAP_CLIENT_ID"

typedef enum _enum_config_load{
    CONFIG_MAIN     = 0x01,
    CONFIG_SQLITE   = 0x02,
    CONFIG_SIP      = 0x04,
    CONFIG_SMPP     = 0x08, 
    CONFIG_SIGTRAN  = 0x10, //TODO
    CONFIG_ALL      = 0xFF
} enum_config_load_t;

typedef enum _sock_model{
    MODEL_CLIENT = 0,
    MODEL_SERVER = 1
} sock_model_t;

#define STR_MODEL_CLIENT  "CLIENT"
#define STR_MODEL_SERVER  "SERVER"

inline int str_to_sock_model(const char *str);
extern char* str_model[];

inline const char* bind_to_str(int i);
inline int str_to_bind(const char *str);

inline const char* npi_to_str(int i);
inline int str_to_npi(const char *str);

inline const char* ton_to_str(int i);
inline int str_to_ton(const char *str);

typedef struct _config_main{
    char    log_level;
    char    *launch_msg;
    bool    fork;
    char    *routing_module;
} config_main_t;
#define new_config_main()   (config_main_t*)calloc(1, sizeof(config_main_t))

typedef struct _config_sip{
    char          *name;          //Interface name
    char          *ip;            //IP host
    unsigned int   port;          //Port Host
    char         *routing_to;     //Routing to
} config_sip_t;
#define new_config_sip()   (config_sip_t*)calloc(1, sizeof(config_sip_t))

typedef struct _config_client_smpp{
    char         *name;           //Interface name
    char         *ip;             //Remote is client/Host if server
    unsigned int  port;           //Remote is client/Host if server
    char         *system_id;      //User ID
    char         *password;       //Passwd
//    char          npi;            //numeric_plan_indicator
//    char          ton;            //type_of_number
    char         *routing_to;     //Routing to
} config_client_smpp_t;
#define new_config_client_smpp()   (config_client_smpp_t*)calloc(1, sizeof(config_client_smpp_t))

typedef struct _config_smpp{
    char         *name;           //Interface name
    sock_model_t  model;          //client | server
    char         *ip;             //Remote is client/Host if server
    unsigned int  port;           //Remote is client/Host if server
    char         *system_id;      //User ID
    char         *password;       //Passwd
//    char          npi_src;        //numeric_plan_indicator
//    char          ton_src;        //type_of_number
//    char          npi_dst;        //numeric_plan_indicator
//    char          ton_dst;        //type_of_number
    char          npi;            //numeric_plan_indicator
    char          ton;            //type_of_number
    char         *system_type;    //WWW | EMAIL | VMS | OTA ...
    //TODO: The service_type parameter can be used to indicate the SMS Application service associated with the message
    char         *service_type;   
    int           command_id;     //bind : transceiver | receiver | transmitter
    char         *routing_to;     //Routing to - Only if client model

    //Client(s) for server model
    map          *list_c_smpp;    // <str, config_client_smpp_t>
} config_smpp_t;
#define new_config_smpp()   (config_smpp_t*)calloc(1, sizeof(config_smpp_t))

typedef struct _config_sqlite{
    char         *path;	          //SQLite3
    char         *encoding;	      //default : UTF-8
    char         *synchronous;     //default : normal 
    unsigned long heap_limit;      //default : 8290304 Ko 
    char         *foreign_keys;    //default : on 
} config_sqlite_t;
#define new_config_sqlite()   (config_sqlite_t*)calloc(1, sizeof(config_sqlite_t))

inline void destroy_config_main(config_main_t *main);
inline void destroy_config_sqlite(config_sqlite_t *sqlite);
//inline void destroy_config_sigtran(config_sigtran_t *sigtran);//TODO
inline void destroy_config_client_smpp(config_client_smpp_t *c_smpp);
inline void destroy_config_smpp(config_smpp_t *smpp);
inline void destroy_config_sip(config_sip_t *sip);

inline void free_config_main(void **main);
inline void free_config_sqlite(void **sqlite);
//inline void free_config_sigtran(void **sigtran);//TODO
inline void free_config_client_smpp(void **c_smpp);
inline void free_config_smpp(void **smpp);
inline void free_config_sip(void **sip);

void free_config_file(enum_config_load_t flags, const char *name);

//inline int compare_config_sigtran(const void *sigtran1, void *sigtran2);//TODO
int compare_config_client_smpp(const void *c_smpp1, const void *c_smpp2);
int compare_config_smpp(const void *smpp1, const void *smpp2);
int compare_config_sip(const void *sip1, const void *sip2);

#endif /* CONFIG_STRUCT_H */

