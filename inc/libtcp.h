#ifndef _TCP_H_
#define _TCP_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/ip.h>
#include <netinet/in.h>
#include <netdb.h>
#include <arpa/inet.h>

#include <errno.h>
#include <unistd.h>

typedef struct 
{
    struct addrinfo *hints;
} tcp_t;

/* Allocate and initialization tcp object*/
tcp_t *tcp_init(void);

/* Method to set and get value of attribute tcp struct*/
struct addrinfo *tcp_getAddrinfo(tcp_t *tcp);
void            tcp_setAddrinfo(tcp_t *tcp, struct addrinfo *newHints);
void            tcp_printAddrinfo(tcp_t *tcp);

#endif