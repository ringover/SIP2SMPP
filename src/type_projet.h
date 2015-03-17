#ifdef __cplusplus
extern "C"{
#endif

#ifndef TYPE_PROJET_H
#define TYPE_PROJET_H

#include <stdint.h> 
#include <stdbool.h> 

#define sizearray(a) (sizeof(a) / sizeof((a)[0]))

#define VERSION          "0.2.0"
#define DEFAULT_CONFIG   "/etc/sip2smpp/config.ini"
#define DEFAULT_PIDFILE  "/var/run/sip2smpp.pid"

/**
*    Color :
*    0 -> normal ; 31 -> red
*    1 -> bold ; 36 -> cyan
*    4 -> underline ; 32 -> green
*    9 -> strike ; 37 -> grey
*/
#define GRAY       "\033[0;37m]"
#define RED        "\033[0;31m"
#define GREEN      "\033[0;32m"
#define CYAN       "\033[0;36m"
#define END_COLOR  "\033[0m"
//End Color

#endif /*TYPE_PROJET_H*/

#ifdef __cplusplus
}
#endif

