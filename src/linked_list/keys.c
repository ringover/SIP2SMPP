
#include "keys.h"

void free_uli(void **data){
    if(data && *data){
        free(*data);
        *data = NULL;
    }
    return;
}

void* copy_uli(const void *data){
    void *uint_out = NULL;
    if(data){
        uint_out  = calloc(1, sizeof(unsigned long int));
        memcpy(uint_out,data,sizeof(unsigned long int));
    }
    return (void*) uint_out;
}

long int compare_uli(const void *data1, const void *data2){
    unsigned long int i1, i2;
    memcpy(&i1,data1,sizeof(unsigned long int));
    memcpy(&i2,data2,sizeof(unsigned long int));
    return (long int) (i1 - i2);
}

///////////////////////
// Key uint32 (unsigned int)
/////

void free_uint16(void **data){
    if(data && *data){
        free(*data);
        *data = NULL;
    }
    return;
}

void* copy_uint16(const void *data){
    void *uint_out = NULL;
    if(data){
        uint_out  = calloc(1, sizeof(uint16_t));
        memcpy(uint_out,data,sizeof(uint16_t));
    }
    return (void*) uint_out;
}

int compare_uint16(const void *data1, const void *data2){
    unsigned int i1 = 0;
    unsigned int i2 = 0;
    memcpy(&i1,data1,sizeof(uint16_t));
    memcpy(&i2,data2,sizeof(uint16_t));
//printf("%d-%d = %d\n", i1, i2, i1-i2);
    return (int) (((int)i1)-((int)i2));
}

///////////////////////
// Key uint32 (unsigned int)
/////

void free_uint32(void **data){
    if(data && *data){
        free(*data);
        *data = NULL;
    }
    return;
}

void* copy_uint32(const void *data){
    void *uint_out = NULL;
    if(data){
        uint_out  = calloc(1, sizeof(unsigned int));
        memcpy(uint_out,data,sizeof(unsigned int));
    }
    return (void*) uint_out;
}

int compare_uint32(const void *data1, const void *data2){
    uint32_t i1, i2;
    memcpy(&i1,data1,sizeof(uint32_t));
    memcpy(&i2,data2,sizeof(uint32_t));
//printf("%d-%d = %d\n", i1, i2, i1-i2);
    return (int) ((int)i1-(int)i2);
}

///////////////////////
// Key char* (string)
/////

void free_string(void **data){
    if(data && *data){
        free(*data);
        *data = NULL;
    }
    return;
}

void* copy_string(const void *data){
    char *str_out = NULL;
    if(data){
        str_out = calloc(strlen((char*)data)+1, sizeof(char));
        strcpy(str_out, (char*)data);
    }
    return (void*) str_out;
}

int compare_string(const void *data1, const void *data2){
  return (int) strcmp((char*)data1, (char*)data2);
}

///////////////////////
// Key char** (tab string)
/////

void free_tab_string(void **data){
    if(data && *data){
        char **tmp = (char**)*data;
        int i = 0;
	while(tmp[i]){
            free(tmp[i++]);
        }
        free(*data);
        *data = NULL;
    }
    return;
}

