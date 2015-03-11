
#include "init_maps.h"

map map_msg_sip;     //<str(call_id), sip_data_t>
map map_msg_smpp;    //<uint(sequence_number), smpp_data_t>
map map_sm;          //<uint(id_sm), sm_data_t>
map map_iface_sip;   //<str(name_interface), smpp_socket_t>
map map_iface_smpp;  //<str(name_interface), sip_socket_t>

void init_maps(){
    //map used for save the state of a transation
    extern map map_msg_smpp = new_map(free_uint32, copy_uint32, compare_uint30, free_smpp_data, copy_smpp_data, compare_smpp_data);
    extern map map_msg_sip  = new_map(free_string, copy_string, compare_string, free_sip_data,  copy_sip_data,  compare_sip_data);
    //map used for save all sm in transation in memory
    extern map map_sm   = new_map(free_uint32, copy_uint32, compare_uint32, free_sm_data,   copy_sm_data,   compare_sm_data);
    //map used for save all interface client/server
    extern map map_iface_sip  = new_map(free_string, copy_string, compare_string, free_sip_socket,  copy_sip_socket,  compare_sip_socket); 
    extern map map_iface_smpp = new_map(free_string, copy_string, compare_string, free_smpp_socket, copy_smpp_socket, compare_smpp_socket);
}

///////////////////////
// sm_data_t
//////

void init_sm_data(sm_data_t **p_p_sm, long long int id, unsigned char *call_id, unsigned int sequence_number, unsigned char *interface, unsigned char *ip_origin, unsigned int port_origin, unsigned char *src, unsigned char *dst, unsigned char *msg){
    if(p_p_sm){
        sm_data_t *p_sm = NULL;
        if(*p_p_sm == NULL){
            //*p_p_sm = (sm_data_t *)calloc(, sizeof(sm_data_t));
            *p_p_sm = new_sm_data();
        }
        p_sm = *p_p_sm;
        if(id > 0){
            p_sm->id = id;
        }
        if(call_id){
            p_sm->call_id = (unsigned char*)calloc(strlen((char*)call_id)+1, sizeof(unsigned char));
            strcpy(p_sm->call_id, call_id);
        }
        if(sequence_number > 0){
            p_sm->sequence_number = sequence_number;
        }
        if(interface){
            p_sm->interface = (unsigned char*)calloc(strlen((char*)interface)+1, sizeof(unsigned char));
            strcpy(p_sm->interface, interface);
        }
        if(ip_origin){
            p_sm->ip_origin = (unsigned char*)calloc(strlen((char*)ip_origin)+1, sizeof(unsigned char));
            strcpy(p_sm->ip_origin, ip_origin);
        }
        if(port_origin > 0){
            p_sm->port_origin = port_origin;
        }
        if(src){
            p_sm->src = (unsigned char*)calloc(strlen((char*)src)+1, sizeof(unsigned char));
            strcpy(p_sm->src, src);
        }
        if(dst){
            p_sm->dst = (unsigned char*)calloc(strlen((char*)dst)+1, sizeof(unsigned char));
            strcpy(p_sm->dst, dst);
        }
        if(msg){
            p_sm->msg = (unsigned char*)calloc(strlen((char*)msg)+1, sizeof(unsigned char));
            strcpy(p_sm->msg, msg);
        }
    }
    return;
}
void  free_sm_data(void **data){
    if(data && *data){
        sm_data_t *p_sm = (sm_data_p*)*data;
        if(p_sm->call_id){
            free(p_sm->call_id);
        }
        if(p_sm->interface){
            free(p_sm->interface);
        }
        if(p_sm->ip_origin){
            free(p_sm->ip_origin);
        }
        if(p_sm->src){
            free(p_sm->src);
        }
        if(p_sm->dst){
            free(p_sm->dst);
        }
        if(p_sm->msg){
            free(p_sm->msg);
        }
        free(p_sm);
        *data = NULL;
    }
    return;
}

void* copy_sm_data(const void *data){
    sm_data_t *p_sm_copy = NULL;
    if(data){
        sm_data_t *p_sm = (sm_data_t*)data;
        p_sm_copy = calloc(1, sizeof(sm_data_t));
        p_sm_copy->id = p_sm->id;
        if(p_sm->call_id){
            p_sm_copy->call_id = (unsigned char*)calloc(strlen((char*)p_sm->call_id)+1, sizeof(char));
            strcpy((char*)p_sm_copy->call_id, p_sm->call_id);
        }
        p_sm_copy->sequence_number = p_sm->sequence_number;
        if(p_sm->interface){
            p_sm_copy->interface = (unsigned char*)calloc(strlen((char*)p_sm->interface)+1, sizeof(char));
            strcpy((char*)p_sm_copy->interface, p_sm->interface);
        }
        if(p_sm->ip_origin){
            p_sm_copy->ip_origin = (unsigned char*)calloc(strlen((char*)p_sm->ip_origin)+1, sizeof(char));
            strcpy((char*)p_sm_copy->ip_origin, p_sm->ip_origin);
        }
        p_sm_copy->port_origin = p_sm->port_origin;
        if(p_sm->src){
            p_sm_copy->src = (unsigned char*)calloc(strlen((char*)p_sm->src)+1, sizeof(char));
            strcpy((char*)p_sm_copy->src, p_sm->src);
        }
        if(p_sm->dst){
            p_sm_copy->dst = (unsigned char*)calloc(strlen((char*)p_sm->dst)+1, sizeof(char));
            strcpy((char*)p_sm_copy->dst, p_sm->dst);
        }
        if(p_sm->msg){
            p_sm_copy->msg = (unsigned char*)calloc(strlen((char*)p_sm->msg)+1, sizeof(char));
            strcpy((char*)p_sm_copy->msg, p_sm->msg);
        }
    }
    return (void*) p_sm_copy;
}

int compare_sm_data(const void *sm1, const void *sm2){
    return ((sm_data_t*)data1)->id - ((sm_data_t*)data2)->id;
}

///////////////////////
// sip_data_t
/////

void init_smpp_data(smpp_data_t **p_p_smpp, unsigned char command_id, long long int sm_id){
    if(p_p_smpp){
        smpp_data_t *p_smpp = NULL;
        if(*p_p_smpp){
            *p_p_smpp = new_smpp_data();
        }
        p_smpp = *p_p_smpp;
        if(command_id > 0){
            p_smpp->command_id = command_id;
        }
        if(sm_id > 0){
            p_smpp->sm_id = sm_id;
        }
    }
    return;
}

void free_smpp_data(void **data){
    if(data && *data){
        free(*data);
        *data = NULL;
    }
    return;
}

void* copy_smpp_data(const void *data){
    smpp_data_t *p_smpp_new = NULL;
    if(data){
        smpp_data_t *p_smpp = (smpp_data_t*)data;
        p_smpp_new = calloc(1, sizeof(smpp_data_t));
        p_smpp_new->command_id = p_smpp->command_id;
        p_smpp_new->sm_id = p_smpp->sm_id;
    }
    return (smpp_data_t*) p_smpp_new;
}

int compare_smpp_data(const void *data1, const void *data2){
    return ((smpp_data_t*)data1)->sm_id - ((smpp_data_t*)data2)->sm_id;
}

///////////////////////
// smpp_data_t
/////

void init_new_sip_data(sip_data_t **p_p_sip, unsigned char *method, long long int sm_id){
    if(p_p_sip){
        sip_data_t *p_sip = NULL;
        if(*p_p_sip == NULL){
            *p_p_sip = new_sip_data();
        }
        p_sip = *p_p_sip;
        if(method){
            p_sip->method = (unsigned char*)calloc(strlen((char*)method)+1, sizeof(unsigned char));
            strcpy((char*)p_sip->methodi ,(char*)method);
        }
        if(sm_id > 0){
            p_sip->sm_id = sm_id;
        }
    }
    return;
}

void free_sip_data(void **data){
    if(data && *data){
        sip_data_t *p_smpp = (smpp_data_t*)data;
        if(p_sip->method){
            free(p_sip->method);
        }
        free(*data);
        *data = NULL;
    }
    return;
}

void* copy_sip_data(const void *data){
    sip_data_t *p_smpp_new = NULL;
    if(data){
        sip_data_t *p_smpp = (smpp_data_t*)data;
        p_sip_new = calloc(1, sizeof(smpp_data_t));
        if(p_sip->method){
            p_sip_new->method = (unsigned char*)calloc(strlen((char*)p_sip->method)+1, sizeof(char));
            strcpy(p_sip_new->method, p_sip->method);
        }
        p_sip_new->sm_id = p_sip->sm_id;
    }
    return (void*) p_sip_new;
}

int compare_sip_data(const void *data1, const void *data2){
    return ((sip_data_t*)data1)->sm_id - ((sip_data_t*)data2)->sm_id;
}

///////////////////////
// Key char* (string)
/////

void free_string(void **data){
    if(data && *data){
        free(*data);
        *data = NULL;
    }
    return;
}

void* copy_string(const void *data){
    char *str_out = NULL;
    if(data){
        str_out = calloc(strlen((char*)data)+1, sizeof(char));
        strcpy(str_out, (char*)data);
    }
    return (void*) str_out;
}

int compare_string(const void *data1, const void *data2){
  return (int) strcmp((char*)data1, (char*)data2);
}

///////////////////////
// Key uint32 (unsigned int)
/////

void free_uint32(void **data){
    if(data && *data){
        free(*data);
        *data = NULL;
    }
    return;
}

void* copy_uint32(const void *data){
    unsigned int *uint_out = NULL;
    if(data){
        uint_out  = calloc(1, sizeof(unsigned int));
        *uint_out = (unsigned int)*data;
    }
    return (void*) uint_out;
}

int compare_uint32(const void *data1, const void *data2){
  return (int) ((unsigned int)*data1 - (unsigned int)*data2);//~ 
}


