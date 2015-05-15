#!/bin/bash

if [ -f serv ]; then
	rm -f serv
fi
gcc ../src/str.o ../src/log/log.o ../src/net/smpp/smpp.o ../src/linked_list/map.o ../src/net/tcp/tcp.o server.c -o serv -lsmpp34 -pthread -lrt -g -pthread
