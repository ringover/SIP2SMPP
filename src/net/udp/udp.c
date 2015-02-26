
#include "udp.h"

int do_udp_receive(socket_t *sock, unsigned char **buffer, size_t *buffer_len, struct sockaddr_in *from){
    if(sock && buffer && buffer_len){
        int ret = 0;
        fd_set input_set;

        if(*buffer == NULL){
            *buffer_len = 2048;
            *buffer = (unsigned char*)calloc(2049, sizeof(unsigned char));
        }

//        memset(*buffer, 0, (*buffer_len)*sizeof(unsigned char));
        FD_ZERO(&input_set);
        FD_SET(sock->socket, &input_set);

        if((ret = select(sock->socket+1, &input_set, NULL, NULL, NULL)) < 0){
            ERROR(LOG_SCREEN | LOG_FILE,"UDP select failed (%d) %s", errno, strerror(errno))
            return (int) -1;
        }

        if(ret > 0  && FD_ISSET(sock->socket, &input_set)){
            int fromlen = 0;
            pthread_mutex_lock(&sock->mutex);
            
            fromlen = (from?sizeof(*from):0);
            if((ret = recvfrom(sock->socket, *buffer, *buffer_len, 0, (struct sockaddr*)from, (from?&fromlen:0))) < 0){
            //if((ret = recvfrom(sock->socket, *buffer, *buffer_len - 1, 0, NULL, 0)) < 0){
                ERROR(LOG_SCREEN | LOG_FILE,"UDP receive failed (%d) %s", errno, strerror(errno))
                ret = -1;
            }
            
            DEBUG(LOG_SCREEN, "Recv Buffer UDP[%d] : \n%s", ret, *buffer)
            pthread_mutex_unlock(&sock->mutex);
        }
        return (int) ret;
    }
    return (int) -1;
}

int do_udp_send(socket_t *sock, unsigned char *buffer, size_t buffer_len, char *ip_remote, unsigned int port_remote){
    if(sock && buffer && buffer_len > 0 && ip_remote && port_remote){

        struct sockaddr_in to = { 0 };
        int ret = 0;

        to.sin_family      = AF_INET;
        to.sin_port        = htons(port_remote);
        inet_aton(ip_remote, (struct in_addr*)&(to.sin_addr.s_addr));
            
        pthread_mutex_lock(&sock->mutex);

	DEBUG(LOG_SCREEN, "Send Buffer UDP[%d] :\n%s", strlen((char*)buffer), buffer)

        if((ret = sendto(sock->socket, buffer, buffer_len, 0, (struct sockaddr*)&to, (socklen_t)sizeof(to))) < 0){
            ERROR(LOG_SCREEN | LOG_FILE, "UDP send failed (%d) %s", errno, strerror(errno))
            return (int) -1;
        }
        
        pthread_mutex_unlock(&sock->mutex);

        return (int) 0;
    }
    return (int) -1;
}

int udp_socket(socket_t *sock, unsigned char *ip_host, unsigned int port_host){
    struct sockaddr_in sin = { 0 };
    int optval = 1;
    int res    = 0;

    if(pthread_mutex_init(&sock->mutex, NULL) != 0){
        ERROR(LOG_SCREEN | LOG_FILE, "Error in mutex (%d) %s", errno, strerror(errno))
        return (int) -1;
    }

    if((sock->socket = socket(AF_INET, SOCK_DGRAM, 0)) == -1){
        ERROR(LOG_SCREEN | LOG_FILE,"UDP Socket failed (%d) %s", errno, strerror(errno))
        return (int) -1;
    }

    if(setsockopt(sock->socket, SOL_SOCKET, SO_REUSEADDR, (char*)&optval, sizeof(optval)) == -1){
        ERROR(LOG_SCREEN | LOG_FILE,"Setsockopt failed (%d) %s", errno, strerror(errno))
        return -1;
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
          ERROR(LOG_SCREEN | LOG_FILE,"UDP Bind failed (%d) %s", errno, strerror(errno))
          return (int) res;
      }
    }

    return (int) res;
}

int udp_close(socket_t *sock){
    pthread_mutex_destroy(&sock->mutex);
    if(close(sock->socket) == -1){
        ERROR(LOG_SCREEN | LOG_FILE, "Close faield (%d) %s", errno, strerror(errno));
        return (int) -1;
    }
    return (int) 0;
}

