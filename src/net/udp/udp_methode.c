
#include "udp_methode.h"

void do_udp_close(int sock){
    close(sock);
    return;
}

int32_t do_udp_connect(int *sock, uint8_t *addrIP, uint16_t port){
    if((*sock = socket(AF_INET, SOCK_DGRAM, 0)) == -1){//SOCKET type = int
        printf("Error in socket()\n");
        return (int32_t)-1;
    }

    struct sockaddr_in sin = { 0 };
    int32_t res = 0;

    sin.sin_addr.s_addr = inet_addr(addrIP);
    sin.sin_family = AF_INET;
    sin.sin_port = htons(port);

    printf("\n-----------------------Connection SIP----------------------\n");
    printf("\n%s:%d\n",addrIP,port);
    printf("-----------------------------------------------------------\n");

    if((res = bind(*sock, (struct sockaddr*) &sin, sizeof(sin))) == -1){
        printf("Error in bind()\n");
        return (int32_t)res;
    }

    return (int32_t)res;
}

int32_t udp_receive(int sock, uint8_t** buffer, size_t buffer_len){
    // struct hostent *hostinfo = NULL;
    struct sockaddr_in from = { 0 };
    int32_t fromsize = sizeof from;
    int32_t n = 0;

    //select
    fd_set input_set;
    struct timeval timeout;
    timeout.tv_sec  = 0;
    timeout.tv_usec = 250;

    FD_ZERO(&input_set);
    FD_SET(sock,&input_set);
    if((n = select(sock + 1,&input_set,NULL,NULL,&timeout)) < 0){
        printf("Error in select socket\n");
        return (int32_t)-1;
    }
    
    if(n > 0){
        if((n = recvfrom(sock, *buffer, buffer_len - 1, 0, (struct sockaddr*)&from, (socklen_t*)&fromsize)) < 0){
           printf("Error in recvfrom()");
           return (int32_t)-1;
       }
       (*buffer)[n] = '\0';
       printf("-------------------------SIP Message-----------------------\n");
       printf("%s\n",*buffer);
       printf("-----------------------------------------------------------\n");
       return (int32_t)0;
    }
    return (int32_t)-1;
}

int32_t udp_send(int sock, uint8_t* buffer, size_t buffer_len, uint8_t* addrIP, uint16_t port){
    //struct hostent *hostinfo = NULL;
    struct sockaddr_in to = { 0 };
    int32_t tosize = sizeof to;
    int32_t n = 0;

    //to.sin_addr.s_addr = inet_addr(addrIP);
    if(inet_aton(addrIP,&(to.sin_addr))==0){
        ERROR(LOG_SCREEN | LOG_FILE ,"IP address is invalid\n")
    }
    to.sin_family      = AF_INET;
    to.sin_port        = htons(port);
        
    tosize = sizeof to;
		
    ERROR(LOG_SCREEN | LOG_FILE, 
                   "-------------------------SIP Message-----------------------\n"
                   "%s\n"
                   "-----------------------------------------------------------\n", buffer)

    if((n = sendto(sock, buffer, buffer_len, 0, (struct sockaddr*)&to, (socklen_t)tosize)) < 0){
        ERROR(LOG_SCREEN | LOG_FILE, "Error in sendto() : %d", n)
        return (int32_t)-1;
    }
    INFO(LOG_SCREEN | LOG_FILE, "The UDP message is send [size : %d]", n)
    //buffer[n] = '\0';
    return (int32_t)0;
}

