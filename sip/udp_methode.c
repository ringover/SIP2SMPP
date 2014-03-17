
#include "udp_methode.h"


void do_udp_close(int sock){
        close(sock);
        return;
}

int do_udp_connect(int *sock,char *addrIP, int port){
        if((*sock = socket(AF_INET, SOCK_DGRAM, 0)) == -1){//SOCKET type = int
            printf("Error in socket()\n");
            return (int)-1;
        }

        struct sockaddr_in sin = { 0 };
        int res = 0;

        sin.sin_addr.s_addr = inet_addr(addrIP);
        sin.sin_family = AF_INET;
        sin.sin_port = htons(port);

	printf("\n-----------------------Connection SIP----------------------\n");
	printf("\n%s:%d\n",addrIP,port);
	printf("-----------------------------------------------------------\n");

        if((res = bind(*sock, (struct sockaddr*) &sin, sizeof(sin))) == -1){
            printf("Error in bind()\n");
            return (int)-1;
        }

        return 0;
}

int udp_receive(int sock, char** buffer, size_t buffer_len, int verbos){
        struct hostent *hostinfo = NULL;
        struct sockaddr_in from = { 0 };
        int fromsize = sizeof from;
        int n = 0;

	//pour le select
	fd_set input_set;
	struct timeval timeout;
	timeout.tv_sec  = 0;
	timeout.tv_usec = 250;

	FD_ZERO(&input_set);
	FD_SET(sock,&input_set);
	if((n = select(sock + 1,&input_set,NULL,NULL,&timeout)) < 0){
		printf("Error in select socket\n");
	        return (int)-1;
	}
	if(n > 0){
        	if((n = recvfrom(sock, *buffer, buffer_len - 1, 0, (struct sockaddr*)&from, (socklen_t*)&fromsize)) < 0){
            		printf("Error in recvfrom()");
            		return (int)-1;
        	}
      		(*buffer)[n] = '\0';
		if(verbos){
			printf("-------------------------SIP Message-----------------------\n");
        		printf("%s\n",*buffer);
			printf("-----------------------------------------------------------\n");
		}
		return (int)0;
	}
	return (int)-1;
}

int udp_send(int sock, char* buffer, size_t buffer_len, char* addrIP, int port, int verbos){
        struct hostent *hostinfo = NULL;
        struct sockaddr_in to = { 0 };
        int tosize = sizeof to;
        int n = 0;

	//to.sin_addr.s_addr = inet_addr(addrIP);
	if(inet_aton(addrIP,&(to.sin_addr))==0){
		printf("address IP is invalid\n");
	}
        to.sin_family      = AF_INET;
        to.sin_port        = htons(port);
        
	tosize = sizeof to;
		
        if(verbos){
           printf("-------------------------SIP Message-----------------------\n");
           printf("%s\n",buffer);
           printf("-----------------------------------------------------------\n");
        }

        if((n = sendto(sock, buffer, buffer_len, 0, (struct sockaddr*)&to, (socklen_t)tosize)) < 0){
            printf("Error in sendto() : %d\n",n);
            return (int)-1;
        }
        printf("sendto() : %d\n",n);
        //buffer[n] = '\0';
	return (int)0;
}
/*
int main(){
	char buffer[2048] = "";
	int sock_udp;
	if(do_udp_connect(&sock_udp,"127.0.0.1",5060) == -1){
               return -1; 
        }

	udp_receive(sock_udp, (char**)&buffer, sizeof(buffer), 1);
	udp_send(sock_udp, (char*)buffer, sizeof(buffer), 1);
	udp_receive(sock_udp, (char**)&buffer, sizeof(buffer), 1);
        
	do_udp_close(sock_udp);
	
	
	return 0;
}
*/
