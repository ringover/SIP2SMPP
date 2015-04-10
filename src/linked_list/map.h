#ifndef MAP_H_INCLUDED
#define MAP_H_INCLUDED

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>


#ifndef _VALUE_
#define _VALUE_
	typedef void(*free_value)(void**);
	typedef void*(*copy_value)(const void*);
	typedef int(*compare_value)(const void*, const void*);
#endif

#ifndef _KEY_
#define _KEY_
	typedef void(*free_key)(void**);
	typedef void*(*copy_key)(const void*);
	typedef int(*compare_key)(const void*, const void*);
#endif

#ifndef SIZE_MAX
#define SIZE_MAX ((size_t) - 1)
#endif
/**
 * This enum is used for establish the status of Map
 */
typedef enum _map_error{
	MAP_OK = 0,
	MAP_DESTROY,
	MAP_FULL,
	MAP_KEY_EXIST,
	MAP_KEY_NOT_EXIST,
	MAP_VALUE_EXIST,
	MAP_VALUE_NOT_EXIST,
  MAP_COPY_FUNC_NULL
} map_error;

typedef struct _iterator_map{
    void *key;
    void *value;//mapped_type

    struct _iterator_map *previous;
    struct _iterator_map *next;
}iterator_map;

typedef struct _map{
	iterator_map *begin;

    free_key    _key_free;
    copy_key    _key_copy;
	compare_key _key_compare;

    free_value    _value_free;
    copy_value    _value_copy;
	compare_value _value_compare;

	size_t _size;
} map;

map* new_map(free_key,copy_key,compare_key,free_value,copy_value,compare_value);

map_error map_set(map *This, void *key, void *value);
map_error map_setByCopy(map *This, void *key, void *value);

void* map_get(map *This, const void *key);
iterator_map* map_find(map *This, const void *key);
bool map_exist(map *This, const void *key);

map_error map_erase(map *This, void *key);
map_error map_destroy(map **This);

size_t map_size(map *This);

//struct _iterator_map* map_begin(map *This);
//struct _iterator_map* map_end(map *This);


#endif // MAP_H_INCLUDED
