/**
*  \file daemonize.c
* 
*  \brief This part it used for the daemonize mode.
*/

#include "daemonize.h"

int daemonize(int nofork){
   FILE *pid_stream;
   pid_t pid;
   int p;
   struct sigaction new_action;

   if(!nofork){
      if((pid=fork()) < 0){
         ERROR("Cannot fork:%s", strerror(errno));
         return -1;
      }else if(pid != 0){
         //printf("parent process");
         exit(0);
      }
   }//else: if(pid == 0) printf("children process");

//setsid + fork

   //Create a pid file for the main process
   if (pid_file!=0){
      if((pid_stream = fopen(pid_file, "r")) != NULL){
         if(fscanf(pid_stream, "%d", &p) <= 0) {
            ERROR(LOG_SCREEN|LOG_FILE,"Could not parse pid file %s", pid_file);
         }
         fclose(pid_stream);
         if(p==-1){
            ERROR(LOG_SCREEN|LOG_FILE,"Pid file %s exists, but doesn't contain a valid pid number", pid_file);
            return -1;
         }else if (kill((pid_t)p, 0) == 0 || errno == EPERM){
            ERROR(LOG_SCREEN|LOG_FILE,"Running process found in the pid file %s",pid_file);
            return -1;
         }else{
            INFO(LOG_SCREEN|LOG_FILE,"Pid file contains old pid, replacing pid");
         }
      }
      pid=getpid();
      if((pid_stream = fopen(pid_file, "w")) == NULL){
         ERROR(LOG_SCREEN|LOG_FILE,"Unable to create pid file %s: %s", pid_file, strerror(errno));
         return -1;
      }else{
         if((fprintf(pid_stream, "%i\n", (int)pid)) <= 0){
            ERROR(LOG_SCREEN|LOG_FILE,"Unable to write pid to file %s: %s", pid_file, strerror(errno));
            return -1;
         }
         fclose(pid_stream);
/*         if(chown(pid_file, uid?uid:-1, gid?gid:-1) != 0){
            ERROR("Failed to change owner of pid file %s: %s(%d)", pid_file, strerror(errno) errno);
            return -1;
         }
*/
      }
   }
//pgid_file ??

   if(!nofork){
      // try to replace stdin, stdout & stderr with /dev/null
      if(freopen("/dev/null", "r", stdin) == 0){
         WARNING(LOG_SCREEN|LOG_FILE,"Unable to replace stdin with /dev/null: %s\n", strerror(errno));
      }
      if(freopen("/dev/null", "w", stderr) == 0){//log_syslog &&
         WARNING(LOG_SCREEN|LOG_FILE,"Unable to replace stderr with /dev/null: %s\n", strerror(errno));
      }
      if(freopen("/dev/null", "w", stdout) == 0){
         WARNING(LOG_SCREEN|LOG_FILE,"Unable to replace stdout with /dev/null: %s\n", strerror(errno));
      }
   }
/*
   // sigation structure
   new_action.sa_handler = handler;
   sigemptyset (&new_action.sa_mask);
   new_action.sa_flags = 0;

   if(sigaction (SIGINT, &new_action, NULL) == -1){
      ERROR(LOG_SCREEN|LOG_FILE,"Failed to set new Handle");
      return -1;
   }
   if(sigaction (SIGTERM, &new_action, NULL) == -1){
      ERROR(LOG_SCREEN|LOG_FILE,"Failed to set new Handle");
      return -1;
   }
*/
   return 0;

   error:
   return -1;
}

