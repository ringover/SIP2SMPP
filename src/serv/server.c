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

char buffer[2048];
socket_t *sock;


int main(){
    char *ip_remote = NULL;
    int port_remote = 0;
    log2display(LOG_ALERT);
    log_init("logFile",NULL);
    memset(buffer, 0, 2048);
    sock = new_socket_t();
    smpp_send_bind_server(sock, "127.0.0.1", 2780);
    sock->socket = smpp_wait_client(sock, &ip_remote, &port_remote);

    while(true){
        void *data = NULL;
        if(smpp_scan_sock(sock, &data) == 0){
            //INFO(LOG_SCREEN, "Client disconected");
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

    tcp_close(sock);
    return 0;
}
