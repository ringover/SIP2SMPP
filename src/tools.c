
#include "tools.h"

int16_t unhex(uint8_t c){
    if(c >= '0' && c <= '9'){
        return (int16_t)(c - '0');
    }
    if(c >= 'a' && c <= 'f'){
        return (int16_t)(c - 'a' + 10);
    }
    if(c >= 'A' && c <= 'F'){
        return (int16_t)(c - 'A' + 10);
    }
    return (uint16_t) -1;
}

uint32_t hex2bin(uint8_t *hex, uint8_t *bin){
    uint32_t i = 0;
    uint32_t j = 0;

    // Trim string if comments present
    if(strchr(hex, '#') != NULL)
      *strchr(hex, '#') = 0;
    if(strchr(hex, '*') != NULL)
      *strchr(hex, '*') = 0;
    if(strchr(hex, '\'') != NULL)
      *strchr(hex, '\'') = 0;

    for(i = 0; i < strlen(hex); i++) {
      if(hex[i] >= '0' && unhex(hex[i]) < 0){
        ERROR(LOG_SCREEN | LOG_FILE, "Bad hex digit encountered.\n")
        return (uint32_t) -1 ;
      }
    }

    for(i = 0; i < strlen(hex); i++) {
      if(hex[i] < '0'){
        continue;
      }
      if(hex[i] >= '0' && hex[i+1] >= '0'){
        bin[j++] = unhex(hex[i])*16+unhex(hex[i+1]);
        i++;
        continue;
      }
      if(hex[i] >= '0'){
        bin[j++] = unhex(hex[i]);
      }
    }
    return (uint32_t) j;
}

