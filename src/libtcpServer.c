#include "libtcpServer.h"

TCP_BOOLEAN tcpServer_isCreatedServer(tcp_server_t *server)
{
    if (server == NULL)
    {
        fprintf(stderr, "[ERROR] Not have created tcp server\n");
        return false;
    }
    return true;
}

static TCP_SOCKETADDRESS tcpServer_allocClientAddr(void)
{
    TCP_SOCKETADDRESS addr = (TCP_SOCKETADDRESS)malloc(sizeof(struct sockaddr));
    if (addr == NULL)
    {
        fprintf(stderr, "[ERROR] Not enough space to create tcp server object\n");
        exit(EXIT_FAILURE);
    }
    return addr;
}

static TCP_SOCKETLENGTH tcpServer_allocClientAddrLen(void)
{
    TCP_SOCKETLENGTH len = (TCP_SOCKETLENGTH)malloc(sizeof(socklen_t));
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

TCP_SOCKETADDRESS tcpServer_getClientAddress(tcp_server_t *server)
{
    if (tcpServer_isCreatedServer(server) == false)
    {
        return NULL;
    }
    
    return server->clientAddr;
}
TCP_VOID tcpServer_setClientAddress(tcp_server_t *server, TCP_SOCKETADDRESS addr)
{
    if (tcpServer_isCreatedServer(server) == false)
    {
        return ;
    }
    server->clientAddr = addr;
}   

TCP_SOCKETLENGTH tcpServer_getClientAddressLength(tcp_server_t *server)
{
    return server->clientAddrLen;
}

TCP_VOID             tcpServer_setClientAddressLength(tcp_server_t *server, socklen_t *len)
{
    server->clientAddrLen = len;
}

TCP_INTEGER       tcpServer_acceptClient(tcp_server_t *server, void (*func)())
{
    int portNumLen = 0;
    char *portNum = NULL;
    int portServer = tcpServer_getPort(server);

    if (tcpServer_getPort(server) == TCP_ERROR)
    {
        fprintf(stderr, "[ERROR] Had not set port number\n");
        return TCP_ERROR;
    }    

    portNumLen = snprintf(NULL, 0, "%d", portServer);
    portNum = (char *)malloc(portNumLen + 1);
    snprintf(portNum, portNumLen + 1, "%d", portServer);

    if (tcp_config(server->socket, NULL, portNum) == TCP_ERROR)
    {
        fprintf(stderr, "[ERROR] tcp config error\n");
        return TCP_ERROR;
    }   
    tcpServer_setClientFileDescriptor(server, accept(tcp_getSockFileDescriptor(server->socket), server->clientAddr, server->clientAddrLen));
    if (tcpServer_getClientFileDescriptor(server) < 0)
    {
        fprintf(stderr, "[ERROR] Accept failed: %s\n", strerror(errno));
        return TCP_ERROR;
    }
    func();
    return 0;
}
TCP_INTEGER            tcpServer_setPort(tcp_server_t *server, TCP_UINT16 port)
{
    if (port < 0)
    {
        fprintf(stderr, "[ERROR] Invalid Port\n");
        return TCP_ERROR;
    }
    tcp_setPort(server->socket, port);
    return 0;
}

TCP_UINT16          tcpServer_getPort(tcp_server_t *server)
{
    return tcp_getPort(server->socket);
}

TCP_INTEGER         tcpServer_getClientFileDescriptor(tcp_server_t *server)
{
    return server->clientFd;
}
TCP_VOID            tcpServer_setClientFileDescriptor(tcp_server_t *server, TCP_INTEGER fd)
{
    server->clientFd = fd;
}