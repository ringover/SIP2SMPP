/**
*   This part it used for the daemonize mode.
*
*   @author : Mickael ROUSSE
*/

/*
#include <assert.h>
#include <pcap.h>
#include <pcap-bpf.h>

#include <netinet/in.h>
#include <netinet/ip.h>
#include <netinet/udp.h>
#include <netinet/if_ether.h>

#include <arpa/inet.h>
#include <netdb.h>
#include <fcntl.h>
#include <net/if.h>
#include <unistd.h>
#include <signal.h>
*/

/*
int daemonize(int nofork){
	FILE *pid_stream;
        pid_t pid;
        int p;
	struct sigaction new_action;

	 if (!nofork) {

                if ((pid=fork())<0){
                        fprintf(stderr,"Cannot fork:%s\n", strerror(errno));
                        goto error;
                }else if (pid!=0){
                        exit(0);
                }
	}

        if (pid_file!=0){
                if ((pid_stream=fopen(pid_file, "r"))!=NULL){
                        if (fscanf(pid_stream, "%d", &p) < 0) {
                                fprintf(stderr,"could not parse pid file %s\n", pid_file);
                        }
                        fclose(pid_stream);
                        if (p==-1){
                                fprintf(stderr,"pid file %s exists, but doesn't contain a valid"
                                        " pid number\n", pid_file);
                                goto error;
                        }
                        if (kill((pid_t)p, 0)==0 || errno==EPERM){
                                fprintf(stderr,"running process found in the pid file %s\n",
                                        pid_file);
                                goto error;
                        }else{
                               fprintf(stderr,"pid file contains old pid, replacing pid\n");
                        }
                }
                pid=getpid();
                if ((pid_stream=fopen(pid_file, "w"))==NULL){
                        printf("unable to create pid file %s: %s\n",
                                pid_file, strerror(errno));
                        goto error;
                }else{
                        fprintf(pid_stream, "%i\n", (int)pid);
                        fclose(pid_stream);
                }
        }

	// sigation structure
	new_action.sa_handler = handler;
        sigemptyset (&new_action.sa_mask);
        new_action.sa_flags = 0;

	if( sigaction (SIGINT, &new_action, NULL) == -1) {
		perror("Failed to set new Handle");
		return -1;
	}
	if( sigaction (SIGTERM, &new_action, NULL) == -1) {
		perror("Failed to set new Handle");
		return -1;
	}

	return 0;
    error:
    return -1;
}
*/
