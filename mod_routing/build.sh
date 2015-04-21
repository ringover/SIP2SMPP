#!/bin/bash

clang -c mod_routing.c -o mod_routing.o -fno-strict-aliasing -Wall -Wno-reorder -Wno-unused -pedantic -g  -fPIC
gcc mod_routing.o ../OBJ/map.o ../OBJ/log.o -o mod_routing.so -shared -lm -ldl -lzdb -lrt 
