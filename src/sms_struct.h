#ifdef __cplusplus
extern "C"{
#endif 

#ifndef SMS_STRUCT_INCLUDE_H
#define SMS_STRUCT_INCLUDE_H

#include <stdlib.h>
#include <stdint.h>

typedef struct _SMS{
	uint8_t *src;
	uint8_t *dst;
	uint8_t *msg;
	int      id;
	int      ttl;
	
	/*
	int err;
	int stat;
	int doneDate;
	*/
} SMS;

void free_sms(SMS **sms);

#endif /* SMS_STRUCT_INCLUDE_H */

#ifdef __cplusplus
}
#endif 
