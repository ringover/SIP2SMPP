#ifndef TYPE_PROJET_H
#define TYPE_PROJET_H

#define sizearray(a) (sizeof(a) / sizeof((a)[0]))

#define VERSION             "1.0"
#define DEFAULT_CONFIG      "/etc/sip2smpp/config.ini"
#define DEFAULT_PIDFILE     "/var/run/sip2smpp.pid"
#define DEFAULT_PORT        "5060"

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
#define NONE       "\033[0m"
//End Color

//utile pour la partie c
typedef enum{
    FALSE=0,
    TRUE=1
}Boolean;

/**
 * use only on static variables
 */
#define ARRAY_SIZE(a)                   \
        (sizeof(a) / sizeof(*(a)))

#endif /*TYPE_PROJET_H*/
