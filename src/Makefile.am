
AM_CFLAGS = -fPIC -Wall -pedantic -O3
AM_LDFLAGS = -pthread -lzdb -lsmpp34 -lrt -ldl

bin_PROGRAMS = sip2smpp
sip2smpp_SOURCES = main.c \
                   database.c \
                   routing.c \
                   sigtran_io.c \
                   sip_io.c \
                   smpp_io.c \
                   str.c \
                   sm_struct.c \
                   config/config.c \
                   daemonize/daemonize.c \
                   linked_list/keys.c \
                   linked_list/list.c \
                   linked_list/map.c \
                   log/log.c \
                   net/net.c \
                   net/sip/sip.c \
                   net/sip/sip_callid.c \
                   net/sip/sip_parser.c \
                   net/sip/sip_struct.c \
                   net/sip/sip_to_string.c \
                   net/smpp/smpp.c \
                   net/tcp/tcp.c \
                   net/udp/udp.c \
                   threadpool/threadpool.c

