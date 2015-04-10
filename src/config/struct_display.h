
#ifndef STRUCT_DISPLAY_H
#define STRUCT_DISPLAY_H

#include "../linked_list/map.h"
#include "struct.h"

inline void display_config_main(config_main_t *main);
inline void display_config_sqlite(config_sqlite_t *sqlite);
//inline void display_config_sigtran(config_sigtran_t *sigtran);//TODO
inline void display_config_client_smpp(config_client_smpp_t *c_smpp);
inline void display_config_smpp(config_smpp_t *smpp);
inline void display_config_sip(config_sip_t *sip);

void display_config_file(enum_config_load_t flags, const char *name);

#endif /* STRUCT_DISPLAY_H */

