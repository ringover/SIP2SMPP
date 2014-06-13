/**
 * ROADMAP :
 *   -  Add LOG_DB (used to centralize all logs or some logs in a DB)
 */

#ifdef __cplusplus
extern "C"{
#endif

#ifndef LOG___H
#define LOG___H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include <unistd.h>
#include <pthread.h>
#include <sys/types.h>

#ifdef __linux
# ifndef _GNU_SOURCE
#  define _GNU_SOURCE
# endif
# include <linux/unistd.h>
# include <sys/syscall.h>
# define GET_PID syscall(__NR_gettid)
#else
# define GET_PID getpid()
#endif

#ifndef GET_TID
# define GET_TID pthread_self()
#endif

#ifndef LOG_BUFFER_LEN
# define LOG_BUFFER_LEN 4096
#endif

/**
 * \brief Mode to display
 */
typedef enum _LogFlagsDisplay{
	LOG_FILE   = 0x01,
	LOG_SCREEN = 0x02,
	LOG_DB     = 0x04 //NOT IMPLEMENTED
}LogFlagsDisplay;

typedef enum _Loglevel{
	LOG_NONE    = 0x00,
	LOG_CONSOLE = 0x01,
	LOG_DEBUG,
	LOG_INFO,
	LOG_NOTICE,
	LOG_WARNING,
	LOG_ERROR,
	LOG_CRIT,
	LOG_ALERT
}Loglevel;

#define LOG_CONSOLE_STR "CONSOLE"
#define LOG_DEBUG_STR   "DEBUG  " 
#define LOG_INFO_STR    "INFO   "
#define LOG_NOTICE_STR  "NOTICE "
#define LOG_WARNING_STR "WARNING"
#define LOG_ERROR_STR   "ERROR  "
#define LOG_CRIT_STR    "CRIT   "
#define LOG_ALERT_STR   "ALERT  "

extern char* str_loglevel[];
extern int (*printf_function)(const char *restrict, ...);

void log2display(Loglevel l);
int  log_init(const char *path,const void *p_function);
int  log_destroy(void);
int  log_change_file(const char *path);

void log_hook(Loglevel lvl, unsigned int display,pthread_t tid,pid_t pid, const char* func, const char* file, unsigned int line, const char* buff);

#define _LOG(lvl, display_flags, text, args...) \
	if( (lvl >= LOG_CONSOLE || lvl <= LOG_ALERT) && text){ \
                char buff[LOG_BUFFER_LEN]; \
                int n_ = snprintf(buff, sizeof(buff), text, ##args); \
		if(buff[n_ - 1] == '\n') buff[n_ - 1] = '\0'; \
		log_hook(lvl, display_flags, GET_TID, GET_PID, __FUNCTION__, __FILE__, __LINE__, buff);	\
        }

#define CONSOLE(display_flags, text, ...) _LOG(LOG_CONSOLE, display_flags, text, ##__VA_ARGS__)
#define DEBUG(display_flags, text, args...) _LOG(LOG_DEBUG, display_flags, text, ##args)
#define INFO(display_flags, text, args...) _LOG(LOG_INFO, display_flags, text, ##args)
#define NOTICE(display_flags, text, args...) _LOG(LOG_NOTICE, display_flags, text, ##args)
#define WARNING(display_flags, text, args...) _LOG(LOG_WARNING, display_flags, text, ##args)
#define ERROR(display_flags, text, args...) _LOG(LOG_ERROR, display_flags, text, ##args)
#define CRIT(display_flags, text, args...) _LOG(LOG_CRIT, display_flags, text, ##args)
#define ALERT(display_flags, text, args...) _LOG(LOG_ALERT, display_flags, text, ##args)

#endif /*LOG_H*/

#ifdef __cplusplus
}
#endif

