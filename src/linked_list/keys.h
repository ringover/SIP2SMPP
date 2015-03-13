#ifndef KEYS_MAP_H
#define KEYS_MAP_H

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

///////////////////////
// uint32 (unsigned int)
/////

#define new_uint32()    (unsigned int*)calloc(1,sizeof(unsigned int))

void free_uint32(void **data);
void* copy_uint32(const void *data);
int compare_uint32(const void *data1, const void *data2);

///////////////////////
// char* (string)
/////

#define new_string(len)    (unsigned char*)calloc(len+1,sizeof(unsigned char))

void  free_string(void **s);
void* copy_string(const void *s);
int   compare_string(const void *a, const void *b);


#endif /*KEYS_MAP_H*/
