#ifndef _TCP_H_
#define _TCP_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/ip.h>
#include <netinet/in.h>
#include <netdb.h>
#include <arpa/inet.h>

#include <errno.h>
#include <unistd.h>

#define TCP_TRUE            true
#define TCP_FALSE           false
#define TCP_BOOLEAN         bool
#define TCP_INTEGER         int
#define TCP_ADDRESSINFO     struct addrinfo * 
#define TCP_SOCKETLENGTH    socklen_t *
typedef struct 
{
    TCP_ADDRESSINFO     hints;
    TCP_INTEGER         sockFd;
} tcp_t;

/* Allocate and initialization tcp object*/
tcp_t *tcp_init(void);

/* Method to set and get value of attribute tcp struct*/
TCP_INTEGER     tcp_getSockFileDescriptor(tcp_t *tcp);
TCP_INTEGER     tcp_setSockFileDescriptor(tcp_t *tcp, TCP_ADDRESSINFO rp);

TCP_ADDRESSINFO tcp_getAddrinfo(tcp_t *tcp);
void            tcp_setAddrinfo(tcp_t *tcp, TCP_ADDRESSINFO newHints);
void            tcp_printAddrinfo(tcp_t *tcp);

TCP_INTEGER     tcp_config(tcp_t *tcp, const char *node, const char *service);
TCP_INTEGER     tcp_send(tcp_t *tcp, void *data, size_t len);
TCP_INTEGER     tcp_recv(tcp_t *tcp, void *data, size_t len);

TCP_BOOLEAN     tcp_isCreateSocket(tcp_t *tcp);            

#endif