#ifndef _TCP_SERVER_H_
#define _TCP_SERVER_H_

#include "libtcp.h"
#include <pthread.h>
#include <signal.h>

#define TCP_THREAD pthread_t *

typedef struct 
{
    tcp_t               *socket;
    TCP_SOCKETADDRESS   clientAddr;
    TCP_SOCKETLENGTH    clientAddrLen;
    TCP_INTEGER         clientFd;
    TCP_THREAD          clients;
    TCP_INTEGER         n_client;
} tcp_server_t;

tcp_server_t        *tcpServer_init(void);
TCP_INTEGER         tcpServer_acceptClient(tcp_server_t *server, void (*routine)(void *));
TCP_INTEGER         tcpServer_acceptMultiClient(tcp_server_t *server, void *(*rouine)(void *));
TCP_INTEGER         tcpServer_sendClient(tcp_server_t *server, void *data, ssize_t len);
TCP_INTEGER         tcpServer_receiveFromClient(tcp_server_t *server, void *data, ssize_t len);

TCP_SOCKETADDRESS   tcpServer_getClientAddress(tcp_server_t *server);
TCP_VOID            tcpServer_setClientAddress(tcp_server_t *server, TCP_SOCKETADDRESS addr);

TCP_SOCKETLENGTH    tcpServer_getClientAddressLength(tcp_server_t *server);
TCP_VOID            tcpServer_setClientAddressLength(tcp_server_t *server, TCP_SOCKETLENGTH len);

TCP_INTEGER         tcpServer_setPort(tcp_server_t *server, TCP_UINT16 port);
TCP_UINT16          tcpServer_getPort(tcp_server_t *server);

TCP_INTEGER         tcpServer_getClientFileDescriptor(tcp_server_t *server);
TCP_VOID            tcpServer_setClientFileDescriptor(tcp_server_t *server, TCP_INTEGER fd);

TCP_THREAD          tcpServer_getMultiClient(tcp_server_t *server);
TCP_VOID            tcpServer_setMuliClient(tcp_server_t *server, int n_client);

#endif 