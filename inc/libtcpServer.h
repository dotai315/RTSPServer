#ifndef _TCP_SERVER_H_
#define _TCP_SERVER_H_

#include "libtcp.h"

typedef struct 
{
    tcp_t               *socket;
    TCP_ADDRESSINFO     clientAddr;
    TCP_SOCKETLENGTH    clientAddrLen;
} tcp_server_t;

tcp_server_t *tcpServer_init(void);

struct sockaddr *tcpServer_getClientAddress(tcp_server_t *server);
void             tcpServer_setClientAddress(tcp_server_t *server, struct sockaddr *addr);

socklen_t       *tcpServer_getClientAddressLength(tcp_server_t *server);
void             tcpServer_setClientAddressLength(tcp_server_t *server, socklen_t *len);

#endif 