#include "sigtran_io.h"

inline void destroy_config_sigtran(config_sigtran_t *sigtran){
    return;
}

inline void display_config_sigtran(config_sigtran_t *sigtran){
    if(sigtran){
        //TODO
    }
    return;
}

inline void free_config_sigtran(void **sigtran){
    //TODO
    //destroy_config_sigtran(*sigtran);
    free(*sigtran);
    *sigtran = NULL;
    return;
}

inline int compare_config_sigtran(const void *sigtran1, void *sigtran2){//TODO
    //config_sigtran_t *sig1 = (config_sigtran_t*)sigtran1;
    //config_sigtran_t *sig2 = (config_sigtran_t*)sigtran2;
    return (int) 0;
}

