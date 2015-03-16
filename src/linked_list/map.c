
#include "map.h"

map *map_str_smpp = NULL;

static iterator_map* _new_iterator_map(void *key, void* value){
	struct _iterator_map *new_it = (struct _iterator_map*)malloc(sizeof(struct _iterator_map));
	memset(new_it, 0, sizeof(struct _iterator_map));
	new_it->key      = key;
	new_it->value    = value;
	new_it->previous = NULL;
	new_it->next     = NULL;
	return (iterator_map*)new_it;
}

static void _free_iterator_map(iterator_map **it, free_key free_k, free_value free_v){
	if(it && *it){
		free_k(&((*it)->key));
		free_v(&((*it)->value));
		(*it)->previous = NULL;
		(*it)->next     = NULL;
		free(*it);
		*it = NULL;
	}
	return;
}

map* new_map(free_key fk, copy_key ck, compare_key cmpk, free_value fv, copy_value cv, compare_value cmpv){
	map *new_map = (map*)malloc(sizeof(map));
	memset(new_map,0,sizeof(map));

	new_map->_key_free    = fk;
	new_map->_key_copy    = ck;
	new_map->_key_compare = cmpk;

	new_map->_value_free    = fv;
	new_map->_value_copy    = cv;
	new_map->_value_compare = cmpv;

	new_map->_size = 0;
	new_map->begin = NULL;

	return (map*)new_map;
}

map_error map_set(map *This, void *key, void *value){
	if(This->_size < SIZE_MAX){
		iterator_map *p_it = This->begin;
		if(p_it == NULL){//map is empty
			This->begin = _new_iterator_map(key, value);
			(This->_size)++;
			return (map_error)MAP_OK;
		}
		// used the compare key function for search the iterator with higher 
		// key than the new key or the last iterator if it is the end
		while(p_it && p_it->next != NULL){
			if(This->_key_compare(key,p_it->key) < 0){
				p_it = p_it->next;
			}else if(This->_key_compare(key,p_it->key) == 0){
				return (map_error)MAP_KEY_EXIST;
			}else{
				break;
			}
		}

		if(p_it != NULL){//insert at this place
			//used compare key function for insert the new iterator before ou after p_it
			struct _iterator_map *new_it = _new_iterator_map(key,value);
			if(This->_key_compare(key,p_it->key) < 0){
				//insert before
				if(p_it->previous == NULL){
					//first iterator of this map
					This->begin = new_it;
				}
				new_it->previous = p_it->previous;
				new_it->next     = p_it;
				p_it->previous   = new_it;
			}else{
				//insert after
				new_it->next     = p_it->next;
				new_it->previous = p_it;
				p_it->next       = new_it;
			}
			(This->_size)++;
			return (map_error)MAP_OK;
		}
	}
	return (map_error)MAP_FULL;
}

map_error map_setByCopy(map *This, void *key, void *value){
  if(This->_key_copy && This->_value_copy){
    map_error code;
    void *k = This->_key_copy(key);
    void *v = This->_value_copy(value);

    if((code = map_set(This,k,v)) != MAP_OK){
      This->_value_free(&v);
      This->_key_free(&k);
      return code;
    }

    return (map_error)MAP_OK;
  }
  return (map_error)MAP_COPY_FUNC_NULL;
}

iterator_map* map_find(map *This, const void *key){
	iterator_map *p_it = This->begin;
	while(p_it != NULL && This->_key_compare(key, p_it->key) != 0){
		p_it = p_it->next;
	}
	return (iterator_map*)p_it;
}

void* map_get(map *This, const void *key){
	iterator_map *p_it = map_find(This,key);
	return (void*)(p_it != NULL ? p_it->value : NULL);
}

bool map_exist(map *This, const void *key){
	return (bool)(map_find(This,key)!=NULL);
}

map_error map_erase(map *This, void *key){
	iterator_map *p_it = This->begin;
	while(p_it != NULL && This->_key_compare(key,p_it->key) != 0){
		p_it = p_it->next;
	}
	if(p_it != NULL){
		if(p_it->previous == NULL){
			//it is a first iterator of this map
			This->begin = p_it->next;
			if(p_it->next){
				p_it->next->previous = NULL;
			}
		}else{
			p_it->previous->next = p_it->next;
			if(p_it->next != NULL){
				p_it->next->previous = p_it->previous;
			}//else :It is the lastiterator of this map
		}
		_free_iterator_map(&p_it,This->_key_free,This->_value_free);
		(This->_size)--;
		return MAP_OK;
	}
	return (map_error)MAP_KEY_NOT_EXIST;
}

size_t map_size(map *This){
	return This->_size;
}

map_error map_destroy(map **This){
	iterator_map *p_it = (*This)->begin;
	while(p_it != NULL){
		iterator_map *next = p_it->next;
		_free_iterator_map(&p_it,(*This)->_key_free,(*This)->_value_free);
		p_it = next;
	}
	free(*This);
	*This = NULL;
	return (map_error)MAP_DESTROY;
}

struct _iterator_map* map_begin(map *This){
	return (struct _iterator_map*) This->begin;
}

struct _iterator_map* map_end(map *This){
	iterator_map *p_it = This->begin;
	while(p_it && p_it->next){
		p_it = p_it->next;
	}
	return (struct _iterator_map*)p_it;
}

