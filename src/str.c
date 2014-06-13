#include "str.h"

void strcpyP(char** str1, char* str2){
        *str1=(char*)malloc(sizeof(char)*(strlen(str2)+1));
        memset(*str1,0,sizeof(char)*(strlen(str2)+1));
        strcpy(*str1,str2);
        return;
}

