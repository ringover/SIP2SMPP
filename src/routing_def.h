#ifndef ROUTING_DEF_H
#define ROUTING_DEF_H

typedef int (*func_start_routing)(void **functions, void **cfgs);
typedef int (*func_routing)(const unsigned char *interface_name, const unsigned char *origin_ip, const unsigned int *origin_port, sm_data_t *p_sm);
typedef int (*func_close_routing)(void);

extern func_start_routing f_start_routing;
extern func_routing f_routing;
extern func_close_routing f_close_routing;

#endif /* ROUTING_DEF_H */

