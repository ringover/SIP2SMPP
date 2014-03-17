
/*
 * Copyright (C) 2006 Raul Tremsal
 * File  : tcp.c
 * Author: Raul Tremsal <ultraismo@yahoo.com>
 *
 * This file is part of libsmpp34 (c-open-smpp3.4 library).
 *
 * The libsmpp34 library is free software; you can redistribute it and/or 
 * modify it under the terms of the GNU Lesser General Public License as 
 * published by the Free Software Foundation; either version 2.1 of the 
 * License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful, but 
 * WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY 
 * or FITNESS FOR A PARTICULAR PURPOSE. See the GNU Lesser General Public 
 * License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License 
 * along with this library; if not, write to the Free Software Foundation, 
 * Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307 USA 
 *
 */

#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <libxml/xmlmemory.h>
#include <libxml/parser.h>

#include "esme.h"

int do_tcp_connect( xmlNodePtr p, int *s )
{
    int ret = 0;
    int n = 1;
    struct hostent _host;
#ifdef __linux__
    struct hostent *__host_result;
#endif
    struct in_addr addr;
    struct sockaddr_in name;

    char h[256];
    char ahost[1024];
    int port = 0;


    GET_PROP_STR(h, p, "host");
    GET_PROP_INT(port, p, "port");

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
};


int do_tcp_close( int sock_tcp )
{

    close( sock_tcp );
    return( 0 );
};
