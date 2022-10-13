#include "libtcpServer.h"

bool tcpServer_isCreatedServer(tcp_server_t *server)
{
    if (server == NULL)
    {
        fprintf(stderr, "[ERROR] Not have created tcp server\n");
        return false;
    }
    return true;
}



static struct sockaddr * tcpServer_allocClientAddr(void)
{
    struct sockaddr *addr = (struct sockaddr *)malloc(sizeof(struct sockaddr));
    if (addr == NULL)
    {
        fprintf(stderr, "[ERROR] Not enough space to create tcp server object\n");
        exit(EXIT_FAILURE);
    }
    return addr;
}

static socklen_t *tcpServer_allocClientAddrLen(void)
{
    socklen_t *len = (socklen_t *)malloc(sizeof(socklen_t));
    if (len == NULL)
    {
        fprintf(stderr, "[ERROR] Not enough space to create address length object\n");
        exit(EXIT_FAILURE);
    }
    return len;
}

tcp_server_t *tcpServer_init(void)
{
    tcp_server_t *newObj = NULL;

    newObj = (tcp_server_t *)malloc(sizeof(tcp_server_t));
    newObj->socket = tcp_init();
    newObj->clientAddr = tcpServer_allocClientAddr();
    newObj->clientAddrLen = tcpServer_allocClientAddrLen();
    return newObj;
}

struct sockaddr *tcpServer_getClientAddress(tcp_server_t *server)
{
    if (tcpServer_isCreatedServer(server) == false)
    {
        return NULL;
    }
    
    return server->clientAddr;
}
void             tcpServer_setClientAddress(tcp_server_t *server, struct sockaddr *addr)
{
    if (tcpServer_isCreatedServer(server) == false)
    {
        return NULL;
    }
    
}

socklen_t       *tcpServer_getClientAddressLength(tcp_server_t *server);
void             tcpServer_setClientAddressLength(tcp_server_t *server, socklen_t *len);
