
#ifndef STR_H_
#define STR_H_

#include <stdlib.h>
#include <ctype.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include "log/log.h"

inline int count_occurence_in_string(char *str_in, char *occurence);

inline char** explode(char *str_in, char *cutter);
inline int  implode(char **p_p_str_in, char *glue, char **str_out);

inline void del_occurrence(char* str_in, int cpt, ...);

inline int strlistcmp(char *buffer, size_t buffer_len, size_t max, char **str_propose);

inline void strupper(char **str_out, const char *str_in);
inline void strlower(char **str_out, const char *str_in);

inline int isnumber(const char *str_in);

inline int strtoi(int *out, const char *str_in);
inline int strtob(bool *out, const char *buffer);

#endif /*STR_H_*/

