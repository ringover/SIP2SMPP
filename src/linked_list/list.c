
#include "list.h"

static iterator_list* _new_iterator_list(void *key){
	iterator_list *new_it = (struct _iterator_list*)malloc(sizeof(struct _iterator_list));
	memset(new_it,0,sizeof(struct _iterator_list));
	new_it->key      = key;
	new_it->previous = NULL;
	new_it->next     = NULL;
	return (iterator_list*)new_it;
}

static void _free_iterator_list(struct _iterator_list **it, free_key free_k){
	if(it && *it){
		free_k(&((*it)->key));
		(*it)->previous = NULL;
		(*it)->next     = NULL;
		free(*it);
		*it = NULL;
	}
	return;
}

list* new_list(free_key fk, copy_key ck, compare_key cmpk){
	list *new_list = (list*)malloc(sizeof(list));
	memset(new_list,0,sizeof(list));

	new_list->_key_free    = fk;
	new_list->_key_copy    = ck;
	new_list->_key_compare = cmpk;

	new_list->_size = 0;
	new_list->begin = NULL;

	return (list*)new_list;
}

list_error list_set(list *This, void *key){
	if(This->_size < SIZE_MAX){
		iterator_list *p_it = This->begin;
		if(p_it == NULL){//list is empty
			This->begin = _new_iterator_list(key);
			(This->_size)++;
			return (list_error)LIST_OK;
		}
		// used the compare key function for search the iterator with higher 
		// key than the new key or the last iterator if it is the end
		while(p_it && p_it->next != NULL){
			if(This->_key_compare(key,p_it->key) > 0){
				p_it = p_it->next;
			}else if(This->_key_compare(key,p_it->key) == 0){
				return (list_error)LIST_KEY_EXIST;
			}else{
				break;
			}
		}

		if(p_it != NULL){//insert at this place
			//used compare key function for insert the new iterator before ou after p_it
			struct _iterator_list *new_it = _new_iterator_list(key);
			if(This->_key_compare(key,p_it->key) < 0){
				//insert before
				if(p_it->previous == NULL){
					//first iterator of this list
					This->begin = new_it;
				}else{
					p_it->previous->next = new_it;
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
			return (list_error)LIST_OK;
		}
	}
	return (list_error)LIST_FULL;
}

list_error list_setByCopy(list *This, void *key){
	list_error code;
	void *k = This->_key_copy(key);

	if((code = list_set(This,k)) != LIST_OK){
		This->_key_free(&k);
		return (list_error)code;
	}

	return (list_error)LIST_OK;
}

iterator_list* list_find(list *This, const void *key){
	iterator_list *p_it = This->begin;
	while(p_it != NULL && This->_key_compare(key, p_it->key) != 0){
		p_it = p_it->next;
	}
	return (iterator_list*)p_it;
}

iterator_list* list_get_num(list *This, uint32_t num){
	iterator_list *p_it = This->begin;
	while(num-- > 0){
		p_it = p_it->next;
	}
	return (iterator_list*)p_it;
}

void* list_get(list *This, const void *key){
	iterator_list *p_it = list_find(This, key);
	return (void*)(p_it != NULL ? p_it->key : NULL);
}

bool list_exist(list *This, const void *key){
	return (bool)(list_find(This,key)!=NULL);
}

list_error list_erase(list *This, void *key){
	iterator_list *p_it = This->begin;
	while(p_it != NULL && This->_key_compare(key,p_it->key) != 0){
		p_it = p_it->next;
	}
	if(p_it != NULL){
		//if(p_it->previous == NULL){
		if(This->begin == p_it){
			//it is a first iterator of this list
			This->begin = p_it->next;
			if(p_it->next){
				p_it->next->previous = NULL;
			}
		}else{
			p_it->previous->next = p_it->next;
			if(p_it->next != NULL){
				p_it->next->previous = p_it->previous;
			}//else :It is the lastiterator of this list
		}
		_free_iterator_list(&p_it,This->_key_free);
		(This->_size)--;
		return (list_error)LIST_OK;
	}
	return (list_error)LIST_KEY_NOT_EXIST;
}

size_t list_size(list *This){
	return This->_size;
}

list_error list_clean(list **This){
	if(*This){
		iterator_list *p_it = (*This)->begin;
		while(p_it != NULL){
			iterator_list *next = p_it->next;
			_free_iterator_list(&p_it,(*This)->_key_free);
			p_it = next;
		}
		(*This)->begin = NULL;
	}
	return (list_error)LIST_DESTROY;
}

list_error list_destroy(list **This){
	if(*This){
		iterator_list *p_it = (*This)->begin;
		while(p_it != NULL){
			iterator_list *next = p_it->next;
			_free_iterator_list(&p_it,(*This)->_key_free);
			p_it = next;
		}
		free(*This);
		*This = NULL;
	}
	return (list_error)LIST_DESTROY;
}

iterator_list* list_begin(list *This){
	return (iterator_list*) This->begin;
}

iterator_list* list_end(list *This){
	struct _iterator_list *p_it = This->begin;
	while(p_it && p_it->next){
		p_it = p_it->next;
	}
	return (iterator_list*)p_it;
}

list_error list_copy(list *copy, list *original){
	if(copy->begin == NULL){
		struct _iterator_list *p_it = original->begin;
		while(p_it){
			list_setByCopy(copy,p_it->key);
			p_it = p_it->next;
		}
		return (list_error)LIST_COPY_OK;
	}
	return (list_error)LIST_COPY_NOK;
}

