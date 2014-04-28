#include "tcp.h"

int do_tcp_connect(TcpConnect p, int *s){
    int ret = 0;
    int n = 1;
    struct hostent _host;
#ifdef __linux__
    struct hostent *__host_result;
#endif
    struct in_addr addr;
    struct sockaddr_in name;

    char h[256];
    sprintf(h,"%s",p.host);
    char ahost[1024];
    int port = p.port;

    if((*s = socket(AF_INET, SOCK_STREAM, 0)) == -1){
        printf("Error in socket()\n");
        ret = -1; goto lb_tcp_connect_end;
    };
    if( setsockopt(*s, SOL_SOCKET, SO_REUSEADDR, (char*)&n, sizeof(n)) == -1){
        printf("Error in setsockopt().\n");
        ret = -1; goto lb_tcp_connect_end;
    };

#ifdef __linux__
    if( gethostbyname_r(h,&_host,ahost,sizeof(ahost),&__host_result,&n) != 0)
#else /* solaris */
    if( gethostbyname_r(h,&_host,ahost,sizeof(ahost),&n) == NULL)
#endif
    {
        printf("Error in gethostbyname_r().\n");
        ret = -1; goto lb_tcp_connect_end;
    };

    memcpy(&addr.s_addr, _host.h_addr_list[0], sizeof(struct in_addr));
    name.sin_family = AF_INET;
    name.sin_port = htons( port );
    name.sin_addr = addr;

    if(connect(*s,(struct sockaddr *)&name,sizeof(name)) != 0){
        printf("Error in connect(%s:%d)\n", h, port);
        ret = -1; goto lb_tcp_connect_end;
    };

lb_tcp_connect_end:
    return( ret );
}


int do_tcp_close(int sock_tcp){
    close(sock_tcp);
    return(0);
}
