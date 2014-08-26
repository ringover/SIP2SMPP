
#include "sms_struct.h"

void free_sms(SMS **sms){
   if(*sms){
      if((*sms)->msg) free((*sms)->msg);
      if((*sms)->src) free((*sms)->src);
      if((*sms)->dst) free((*sms)->dst);
      free(*sms);
      *sms = NULL;
   }
}

