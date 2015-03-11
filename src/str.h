
#ifndef STR_H_
#define STR_H_

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "log/log.h"

void string_copy(char** str1, char* str2);

int count_occurence_in_string(char *str_in, char *occurence);

char** explode(char *str_in, char *cutter);
int  implode(char **p_p_str_in, char *glue, char **str_out);

void del_occurrence(char* str_in, int cpt, ...);

#endif /*STR_H_*/

