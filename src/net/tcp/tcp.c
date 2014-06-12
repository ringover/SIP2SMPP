#include "tcp.h"

/**
 * \brief This function allow connect the socket in parametter
 *
 * \param s	The socket
 * \param host	The server host
 * \param port	The server port 
 */
int do_tcp_connect(int *s, char *host, int port){
    int n = 1;
    struct hostent *hostInfo = NULL;
    struct in_addr addr;
    struct sockaddr_in name;

    /* create new stream socket */
    if((*s = socket(AF_INET, SOCK_STREAM, 0)) == -1){
        ERROR(LOG_SCREEN | LOG_FILE, "Socket failed with (%d) %s", errno, strerror(errno));
        return -1;
    }

    if(setsockopt(*s, SOL_SOCKET, SO_REUSEADDR, (char*)&n, sizeof(n)) == -1){
        ERROR(LOG_SCREEN | LOG_FILE,"Setsockopt failed (%d) %s", errno, strerror(errno));
        return -1;
    }

    if((hostInfo = gethostbyname(host)) == NULL){
        ERROR(LOG_SCREEN | LOG_FILE,"Gethostbyname failed (%d) %s", errno, strerror(errno));
        return -1;
    }

    memcpy(&addr.s_addr, hostInfo->h_addr_list[0], sizeof(struct in_addr));
    if(hostInfo){
//       free(hostInfo);
    }
    name.sin_family = AF_INET;
    name.sin_port   = htons(port);
    name.sin_addr   = addr;

    /* connect procedure */
    do {
        if((n =connect(*s,(struct sockaddr *)&name,sizeof(name))) == -1){
            if(errno == EINTR){//wakeup signal
                continue;
            }
            ERROR(LOG_SCREEN | LOG_FILE,"%s:%d connect failed with (%d) %s", host, port, errno, strerror(errno));
            return -1;
        }
    }while(n != 0);
    
    return 0;
}

/**
 * \brief This function allow to close the TCP socket
 * 
 */
int do_tcp_close(int sock_tcp){
    if(close(sock_tcp) == -1){
        ERROR(LOG_SCREEN | LOG_FILE, "Close faield (%d) %s", errno, strerror(errno));
	return -1;
    }
    return 0;
}
