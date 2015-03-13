/**
 * SMPP Server mode
 */
#include <stdio.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/time.h>
#include <pthread.h>
#include <sys/types.h>
#include <netdb.h>
#include "../net/tcp/tcp.h"
#include "../log/log.h"
#include "../smpp_io.h"
#include "../net/smpp/smpp.h"

smpp_socket *sock;
char buffer[2048];

int main(){
	int res = 0, i = 0;
	char *ip_remote = NULL;
	int port_remote = 0;
	int type_bind   = 0;
	struct sockaddr_in sin = { 0 };
	int sinsize = sizeof(sin);
	memset(buffer, 0, 2048);
  sock = new_smppçsocket_t();
	init_smpp_socket(sock, "server_smpp", NULL, 0, "127.0.0.1", 2780, "test", "test", 3, "WWW", 1, 1, 1, 1);
	do_smpp_bind_server(sock->sock, "127.0.0.1", 2780);
	do_smpp_wait_client(sock->sock, &ip_remote, &port_remote, &type_bind);

	while(i++ < 333){
		DEBUG(LOG_SCREEN, "Wait SMS...")
		char *p_from = NULL;
		char *p_to   = NULL;
		char *p_msg  = NULL;
		//do_tcp_recv(sock->sock, buffer, 2048, 0);
		do_smpp_receive_sms(sock->sock, &p_from, &p_to, &p_msg);
		display_sms(p_from, p_to, p_msg);
		if(p_from) free(p_from);
		if(p_to)   free(p_to);
		if(p_msg)  free(p_msg);
		DEBUG(LOG_SCREEN, "SMS Recv")
	}
/*
	sleep(3);
	do_smpp_send_sms(sock->sock, "33643844688", "442871140400", "Hello World", 1, 1, 1, 1);
	sleep(3);
*/
	tcp_close(sock->sock);
	smpp_socket_free(&sock);
	return 0;
}
