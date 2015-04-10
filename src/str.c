
#include "str.h"

#ifndef _strcpy
#define _strcpy(dst, src) \
    dst = (char*)calloc(strlen((char*)src)+1, sizeof(char)); \
    strcpy(dst, src)
#endif /*_strcpy*/

#ifndef _strncpy
#define _strncpy(dst, src, size) \
    dst = (char*)calloc(size+1, sizeof(char)); \
    strncpy(dst, src, size)
#endif /*_strncpy*/


/**
 * Count the number of occurences to 'occurence' in input string 'str_in'
 */
inline int count_occurence_in_string(char *str_in, char *occurence){
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
inline char** explode(char *str_in, char *cutter){
    int nb_occur = count_occurence_in_string(str_in, cutter);
    
    if(nb_occur>0){
        char  **exp = (char**)calloc(nb_occur+2, sizeof(char*));
        char  *end  = str_in+strlen((char*)str_in);
        char  *cursor1 = str_in;
        char  *cursor2 = str_in;
        int   cpt = 0;

        while(cursor1 < end){
                cursor2 = (char*)strstr((char*)cursor1,(char*)cutter);
                if(cursor2){
                    char *temp = (char*)calloc(cursor2-cursor1+1, sizeof(char));
                    strncpy((char*)temp,(char*)cursor1,cursor2-cursor1);
                    *(exp+cpt) = temp;
                    cursor1 = cursor2+strlen((char*)cutter);
                }else if(end-cursor1 > 0){
                    char *temp = (char*)calloc(end-cursor1+1, sizeof(char));
                    strncpy((char*)temp,(char*)cursor1,end-cursor1);
                    *(exp+cpt) = temp;
                    cursor1 = end;
                    break;
                }
                cpt++;
        }
        return (char**)exp;
    }
    return (char**)NULL;
}

inline int  implode(char **p_p_str_in, char *glue, char **str_out){
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

inline void del_occurrence(char* str_in, int cpt, ...){
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

/**
 * This function is used for strupper and strlower
 */
static inline int _strxer(const char *str_in, char **str_out, int (*f_char)(int c)){
    if(str_out && str_in){
        int i = 0;
        if(!(*str_out)){
            *str_out = (char*)calloc(strlen(str_in)+1, sizeof(char));
        }//TODO: else len(str_out) < strlen(str_in) => return -1
        while(str_in[i]){
            (*str_out)[i] = (char)f_char((int)str_in[i]);
            i++;
        }
        return (int) 0;
    }
    return (int) -1;
}

inline void strupper(char **str_out, const char *str_in){
    _strxer(str_in, str_out, toupper);
    return;
}

inline void strlower(char **str_out, const char *str_in){
    _strxer(str_in, str_out, tolower);
    return;
}

inline int strlistcmp(char *buffer, size_t buffer_len, size_t max, char **str_propose){
    if(buffer &&
        (!max || (max <= buffer_len && strlen(buffer) <= max)))
    {
        if(str_propose){
            int i = 0;
            while(str_propose + i){
                if(strcmp(*(str_propose + i), buffer) == 0){
                   return (int) i;
                }
                i++;
            }
        }
    }
    return (int) -1;
}

inline int isnumber(const char *str_in){
    if(str_in){
        int flags = 0; // 0x01 => is number positif | 0x02 => is double positif | 0x04 => is negatif
        unsigned int len = strlen(str_in);
        unsigned int i = 0;
        if(*(str_in + 0) == '-'){
            flags |= 0x04;
            i++;
        }
        while(i < len){
            if(isdigit(*(str_in+i))){
                flags |= 0x01;
                i++;
            }else if((*(str_in + i) == '.' || *(str_in + i) == ',') && (flags & 0x02) == 0){
                flags |= 0x02;
                i++;
            }else{
                break;
            }
        }
        if(flags & 0x01){
            return (int) flags;
        }
    }
    return (int) -1;
}

inline int strtoi(int *out, const char *str_in){
    if(str_in && out && isnumber(str_in)){
        *out = atoi(str_in);
        return (int) 0;
    }
    return (int) -1;
}

#define STR_FALSE    "FALSE"
#define STR_TRUE     "TRUE"
#define STR_OFF      "OFF"
#define STR_ON       "ON"
#define STR_NO       "NO"
#define STR_YES      "YES"
inline int strtob(bool *out, const char *buffer){
    char *str_cmp = NULL;
    if(!out && buffer){
        return (int) -1;
    }
    strupper(&str_cmp, buffer);
    if(strcmp(str_cmp, STR_TRUE) == 0 || strcmp(str_cmp, STR_ON) || strcmp(str_cmp, STR_YES)){
        *out = true;
    }else if(strcmp(str_cmp, STR_FALSE) == 0 || strcmp(str_cmp, STR_OFF) == 0 || strcmp(str_cmp, STR_NO) == 0){
        *out = false;
    }else{
        free(str_cmp);
        return (int) -1;
    }
    free(str_cmp);
    return (int) 0;
}
#undef STR_FALSE
#undef STR_TRUE
#undef STR_OFF
#undef STR_ON
#undef STR_NO
#undef STR_YES



