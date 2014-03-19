#ifndef DAEMONIZE_H
#define DAEMONIZE_H

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

//char* pid_file = (char*)DEFAULT_PIDFILE;

int daemonize(int nofork);

#endif /*DAEMONIZE*/
