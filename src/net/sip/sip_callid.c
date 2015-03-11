
#include "sip_callid.h"

#define SIZE_RAND_NUM   20  //16
#define SIZE_CALLID_S   65  // '-' + pid + ppid + ipv4/6(128bits)
#define SIZE_CALLID_P   (sizeof(unsigned long) * 2)

static unsigned long rand_number;
static char buffer_callid[SIZE_RAND_NUM + SIZE_CALLID_S];

int init_callid(char *ip_host){
    int rand_bits = sizeof(int)*8;
    int i = sizeof(long)*8 / rand_bits;

    //Init buffer
    memset(buffer_callid, 0, sizeof(buffer_callid));

    //Init rand
    srand(time(NULL));

    if(INT_MAX != RAND_MAX){
        return (int) -1;
    }

    //Generate a long rand
    rand_number = rand();
    while(i--){
        rand_number <<= rand_bits;
        rand_number |= rand();
    }

    //Save in buffer_callid
    if(snprintf(buffer_callid, SIZE_CALLID_P + 1, "%0*lx", SIZE_CALLID_P, rand_number) == -1){
        return (int) -1;
    }

    if(ip_host){
        return (snprintf(buffer_callid+SIZE_CALLID_P, SIZE_CALLID_S, "%c%d%d@%s", '-', getppid(), getpid(), ip_host) == -1) ? -1 : 0;
    }

    return (snprintf(buffer_callid+SIZE_CALLID_P, SIZE_CALLID_S, "%c%d%d", '-', getppid(), getpid()) == -1) ? -1 : 0;
}


static inline int inc_hexchar(char* c){
  if (*c == '9') {
    *c = 'a';
    return 0;
  }

  if (*c == 'f') {
    *c = '0';
    return 1;
  }

  (*c)++;
  return 0;
}

int generate_callid(char **str_out){
    if(str_out){
        short i = SIZE_CALLID_P;
        while(i && !inc_hexchar(buffer_callid + i-- - 1));
        i = strlen((char*)buffer_callid);
        *str_out = (char*)calloc(i, sizeof(char));
        strncpy(*str_out, buffer_callid, i);
        return (int) 0;
    }
    return (int) -1;
}

/**
 * SAMPLE
int main(){
    char *str1 = NULL;
    short i = 30;
    init_callid("192.168.10.1"); 
    printf("init_callid = %s\n", buffer_callid);

    while(i--){
        generate_callid(&str1);
        printf("%s\n",str1);
        //printf("init_callid = %s\n", buffer_callid);
        free(str1);
    } 
    return 0;
}
 */

