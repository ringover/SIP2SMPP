#!/bin/bash

rm -f serv
clang ../../OBJ/log.o ../../OBJ/smpp.o ../../OBJ/map.o ../../OBJ/tcp.o server.c -o serv -lsmpp34 -pthread -lrt -g -pthread
