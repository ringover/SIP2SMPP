
#include "str.h"

/**
 * Count the number of occurences to 'occurence' in input string 'str_in'
 */
int count_occurence_in_string(char *str_in, char *occurence){
    char  *p  = str_in;
    int cpt = 0;

    p = (char*)strstr((char*)p,(char*)occurence);

    while(p && p-str_in < strlen((char*)str_in)){
        cpt++;
        p+=strlen((char*)occurence);
        p = strstr((char*)p, (char*)occurence);
    }

    return (int) cpt;
}

/**
 * Split a string by string
 */
char** explode(char *str_in, char *cutter){
    int nb_occur = count_occurence_in_string(str_in, cutter);
    
    if(nb_occur>0){
        int   cpt = 0;
        char  *cursor1 = str_in;
        char  *cursor2 = str_in;
        char  **exp = (char**)malloc(sizeof(char*)*(nb_occur+1));

        memset(exp,0,sizeof(char*)*(nb_occur+1));
        while(cpt < nb_occur){
                cursor2 = (char*)strstr((char*)cursor1,(char*)cutter);
                if(cursor2){
                    char *temp = (char*)malloc(sizeof(char)*(cursor2-cursor1+1));
                    memset(temp,0,cursor2-cursor1+1);
                    strncpy((char*)temp,(char*)cursor1,cursor2-cursor1);
                    *(exp+cpt) = temp;
                }

                cursor1 = cursor2+strlen((char*)cutter);
                cpt++;
        }
        return (char**)exp;
    }
    return (char**)NULL;
}


int  implode(char **p_p_str_in, char *glue, char **str_out){
  if(p_p_str_in && str_out && *str_out == NULL && glue){
    int size_str_out = 0;
    int i = 0;
    int j = 0;
    while(p_p_str_in[i]){
      size_str_out += strlen((char*)p_p_str_in[i]);
      size_str_out += strlen((char*)glue);
      i++;
    }

    *str_out = (char*)malloc(sizeof(char)*(size_str_out+1));
    memset(*str_out,0,sizeof(char)*(size_str_out+1));
    i=0;

    while(p_p_str_in[i]){
      strcpy((char*)*str_out+j,(char*)p_p_str_in[i]);
      j += strlen((char*)p_p_str_in[i]);
      strcpy((char*)*str_out+j,(char*)glue);
      j += strlen((char*)glue);
      i++;
    }
    return (int) 0;
  }
  return (int) -1;
}

void del_occurrence(char* str_in, int cpt, ...){
    char* parametre = NULL;
    int   i = 0;
    va_list ap;
    va_start(ap, cpt);
    for(; i < cpt; i++){
        char* read      = str_in;
        int   size_param = 0;
        parametre = va_arg(ap, char*);
        size_param = strlen((char*)parametre);
        while((read = strstr((char*)read,(char*)parametre)) && read){
            strcpy((char*)read,(char*)read+size_param);
        }
    }
    va_end(ap);
}

