#include "tcp.h"

int do_tcp_send(socket_t *sock, unsigned char *buffer, size_t buffer_len, int flag){
    int ret = 0;

    pthread_mutex_lock(&sock->mutex);

    ret = send(sock->socket, buffer, buffer_len, flag);
    if(ret != buffer_len){
        ERROR(LOG_FILE | LOG_SCREEN, "TCP send failed [%d]-> %s", errno, strerror(errno))
        return(int32_t) -1;
    }

    pthread_mutex_unlock(&sock->mutex);
    return ret;
}

int do_tcp_recv(socket_t *sock, unsigned char *buffer, size_t buffer_len, int flags){
    if(buffer && buffer_len > 0){
        int ret = 0;
        fd_set input_set;

        FD_ZERO(&input_set);
        FD_SET(sock->socket, &input_set);

        if((ret = select(sock->socket+1 ,&input_set, NULL, NULL, NULL)) == -1){
            ERROR(LOG_FILE | LOG_SCREEN, "packet select error [%d]-> %s", errno, strerror(errno))
        }
        if(ret > 0 && FD_ISSET(sock->socket, &input_set)){
            pthread_mutex_lock(&sock->mutex);
        
            if((ret = recv(sock->socket, buffer, buffer_len, flags)) == -1){
                ERROR(LOG_FILE | LOG_SCREEN, "packet receive error [%d]-> %s", errno, strerror(errno))
            }
            if(ret == 0){
                ERROR(LOG_FILE | LOG_SCREEN, "Client disconected")
                //client disconected
            }
            INFO(LOG_SCREEN, "Recv Msg[%d]", ret);

            pthread_mutex_unlock(&sock->mutex);
        }
        return (int)ret;
    }
}

/*
int tcp_socket_server(socket_t *sock, char *ip_host, int port_host){
    int optval  = 1;
    int res     = 0;
    //maximum length to which the queue of pending connections
    int backlog = 5;
    struct sockaddr_in sin;

    if(pthread_mutex_init(&sock->mutex, NULL) != 0){
        ERROR(LOG_SCREEN, "Error in mutex");
        return (int)-1;
    }

    if((sock->socket = socket(PF_INET, SOCK_STREAM, 0)) == -1){
        ERROR(LOG_SCREEN | LOG_FILE, "Socket failed with (%d) %s", errno, strerror(errno));
        return (int)-1;
    }

    if((res = setsockopt(sock->socket, SOL_SOCKET, SO_REUSEADDR, (char*)&optval, sizeof(optval))) == -1){
        ERROR(LOG_SCREEN | LOG_FILE,"Setsockopt failed (%d) %s", errno, strerror(errno));
        return (int)-1;
    }

    if(ip_host && port_host > 0){
      if(ip_host){
          sin.sin_addr.s_addr = inet_addr(ip_host);
      }
      sin.sin_family = AF_INET;
      if(port_host > 0){
          sin.sin_port = htons(port_host);
      }
      if((res = bind(sock->socket, (struct sockaddr*) &sin, sizeof(sin))) == -1){
          ERROR(LOG_SCREEN | LOG_FILE,"UDP Bind failed (%d) %s", errno, strerror(errno));
          return (int)res;
      }
    }

 
    if((res = listen(sock->socket, 5)) == -1){
        ERROR(LOG_SCREEN | LOG_FILE,"Listen failed (%d) %s", errno, strerror(errno));
        return (int)-1;
    }

    //TODO: multi-client
    //accept()
    return 0;
}
*/

/**
 * \brief This function allow connect the socket in parametter
 */
int tcp_socket_client(socket_t *sock, unsigned char *ip_remote, int port_remote){
    int optval = 1;
    int res    = 0;
    struct sockaddr_in sin;

    if(pthread_mutex_init(&sock->mutex, NULL) != 0){
        ERROR(LOG_SCREEN, "Error in mutex");
        return (int)-1;
    }

    /* create new stream socket */
    if((sock->socket = socket(PF_INET, SOCK_STREAM, 0)) == -1){
        ERROR(LOG_SCREEN | LOG_FILE, "Socket failed with (%d) %s", errno, strerror(errno));
        return -1;
    }

    if(setsockopt(sock->socket, SOL_SOCKET, SO_REUSEADDR, (char*)&optval, sizeof(optval)) == -1){
        ERROR(LOG_SCREEN | LOG_FILE,"Setsockopt failed (%d) %s", errno, strerror(errno));
        return -1;
    }

    memset(&sin, 0, sizeof(struct sockaddr_in));
    inet_aton(ip_remote, (struct in_addr*)&sin.sin_addr.s_addr);
    sin.sin_family = AF_INET;
    sin.sin_port   = htons(port_remote);

    /* connect procedure */
    do {
        if((res = connect(sock->socket,(struct sockaddr *)&sin,sizeof(sin))) == -1){
            if(errno == EINTR){//wakeup signal
                continue;
            }
            ERROR(LOG_SCREEN | LOG_FILE,"%s:%d connect failed with (%d) %s", ip_remote, port_remote, errno, strerror(errno));
            return -1;
        }
    }while(res != 0);
    
    return 0;
}

/**
 * \brief This function allow to close the TCP socket
 */
int tcp_close(socket_t *sock){
    pthread_mutex_destroy(&sock->mutex);
    if(close(sock->socket) == -1){
        ERROR(LOG_SCREEN | LOG_FILE, "Close faield (%d) %s", errno, strerror(errno));
        return -1;
    }
    return 0;
}
