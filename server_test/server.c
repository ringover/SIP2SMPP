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
#include "../src/net/tcp/tcp.h"
#include "../src/log/log.h"
#include "../src/smpp_io.h"
#include "../src/net/smpp/smpp.h"

char buffer[2048];
socket_t *sock;

void* server_listen(void *p_data){
    while(true){
        void *data = NULL;
        if(smpp_scan_sock(sock, &data) == 0){
            break;
        }
        if(data){
            switch(((generic_nack_t*)data)->command_id){
                case BIND_TRANSMITTER :
                {   bind_transmitter_t *p_bind = (bind_transmitter_t*)data;
                    smpp_send_bind_receiver_resp(sock, (unsigned char*)"smpp34_serv", p_bind->sequence_number, ESME_ROK, false);
                }   break;
                case BIND_RECEIVER :
                {   bind_receiver_t *p_bind = (bind_receiver_t*)data;
                    smpp_send_bind_transmitter_resp(sock, (unsigned char*)"smpp34_serv", p_bind->sequence_number, ESME_ROK, false);
                }   break;
                case BIND_TRANSCEIVER :
                {   bind_transceiver_t *p_bind = (bind_transceiver_t*)data;
                    smpp_send_bind_transceiver_resp(sock, (unsigned char*)"smpp34_serv", p_bind->sequence_number, ESME_ROK, false);
                }   break;
                case DELIVER_SM : //client
                {   deliver_sm_t *p_d_sm = (deliver_sm_t*)data;
                    smpp_send_deliver_sm_resp(sock, NULL, p_d_sm->sequence_number, ESME_ROK);
                }   break;
                case SUBMIT_SM : //server
                {   submit_sm_t *p_s_sm = (submit_sm_t*)data;
                    smpp_send_submit_sm_resp(sock, NULL, p_s_sm->sequence_number, ESME_ROK);
                }   break;
            }
            free(data);
        }
    }
    return NULL;
}

void* server_send(void *p_data){
    while(true){
        char str[1024] = "";
        scanf("%s", str);
        if(strcmp(str, "send") == 0){
            unsigned int seq_num = (unsigned int)rand();
            smpp_send_submit_sm(sock, "33123456789", "44123456789", "Kikoo World", &seq_num, 1, 1, 1, 1);
        }
    }
    return NULL;
}

pthread_t pth_listen;
pthread_t pth_send;

int main(){
    char *ip_remote = NULL;
    int port_remote = 0;
    log2display(LOG_ALERT);
    log_init("logFile",NULL);
    memset(buffer, 0, 2048);
    sock = new_socket();
    smpp_send_bind_server(sock, "0.0.0.0", 2780);
    sock->socket = smpp_wait_client(sock, &ip_remote, &port_remote);

    pthread_create(&pth_listen, NULL, server_listen, NULL);
    pthread_create(&pth_send, NULL, server_send, NULL);

    pthread_join(pth_listen, NULL);
    pthread_join(pth_send, NULL);

    tcp_close(sock);
    return 0;
}
