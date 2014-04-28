#include "structSMPP.h"

void free_sms(SMS **sms){
	if((*sms)->msg) free((*sms)->msg);
	if((*sms)->src) free((*sms)->src);
	if((*sms)->dst) free((*sms)->dst);
	if(*sms) free(*sms);
	*sms = NULL;
}
