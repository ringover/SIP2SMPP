#ifndef LIST_H_INCLUDED
#define LIST_H_INCLUDED

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

#ifndef _KEY_
#define _KEY_
	typedef void(*free_key)(void**);
	typedef void*(*copy_key)(const void*);
	typedef int(*compare_key)(const void*, const void*);
#endif

#ifndef _ERROR_CODE_
#define _ERROR_CODE_
	typedef char error_code;
#endif

#ifndef SIZE_MAX
#define SIZE_MAX ((size_t) - 1)
#endif

/**
 * This enum is used for establish the status of list
 */
typedef enum _list_error{
	LIST_OK = 0,
	LIST_DESTROY,
	LIST_FULL,
	LIST_KEY_EXIST,
	LIST_KEY_NOT_FOUND,
	LIST_KEY_NOT_EXIST,
	LIST_COPY_OK,
	LIST_COPY_NOK
} list_error;

typedef struct _iterator_list{
    void *key;

    struct _iterator_list *previous;
	struct _iterator_list *next;
} iterator_list;

typedef struct _list{
	struct _iterator_list *begin;

    free_key    _key_free;
    copy_key    _key_copy;
	compare_key _key_compare;

	size_t _size;
}list;

list* new_list(free_key,copy_key,compare_key);

list_error list_set(list *This, void *key);
list_error list_setByCopy(list *This, void *key);

void* list_get(list *This, const void *key);
iterator_list* list_get_num(list *This, uint32_t num);
iterator_list* list_find(list *This, const void *key);
bool list_exist(list *This, const void *key);

list_error list_erase(list *This, void *key);
list_error list_clean(list **This);
list_error list_destroy(list **This);

size_t list_size(list *This);

iterator_list* list_begin(list *This);
iterator_list* list_end(list *This);

list_error list_copy(list *copy, list *original);

#endif // LIST_H_INCLUDED
