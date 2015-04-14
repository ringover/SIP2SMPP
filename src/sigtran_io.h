#ifndef SIGTRAN_IO_H
#define SIGTRAN_IO_H

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

//TODO: SIGTRAN Interface

typedef struct _config_sigtran{
    char        *ip;
    unsigned int port;
} config_sigtran_t;

inline void destroy_config_sigtran(config_sigtran_t *sigtran);
void free_config_sigtran(void **sigtran);
inline int compare_config_sigtran(const void *sigtran1, void *sigtran2);
inline void display_config_sigtran(config_sigtran_t *sigtran);


#endif /* SIGTRAN_IO_H */

