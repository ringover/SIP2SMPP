#include "net.h"

inline int str_to_sock_model(const char *str){
    if(str){
        char *tmp = NULL;
        strupper(&tmp, str);
        if(strcmp(tmp, STR_MODEL_CLIENT) == 0){
            return (int) MODEL_CLIENT;
        }
        if(strcmp(tmp, STR_MODEL_SERVER) == 0){
            return (int) MODEL_SERVER;
        }
    }
    return (int) -1;
}

char *str_model[] = {
    STR_MODEL_CLIENT,
    STR_MODEL_SERVER
};



