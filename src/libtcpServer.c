#include "libtcpServer.h"

static tcp_server_t *tcpServer_copyServer(tcp_server_t *server)
{
    tcp_server_t *copyServer = (tcp_server_t *)malloc(sizeof(*server));
    memcpy(copyServer, server, sizeof(*server));
    return copyServer;
}

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
    newObj->clients = NULL;

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

    if (portServer == TCP_ERROR)
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
    func(server);
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

TCP_INTEGER         tcpServer_sendClient(tcp_server_t *server, void *data, ssize_t len)
{
    if (data == NULL)
    {
        fprintf(stderr, "[ERROR] Tranfer Data is null\n");
        return -1;
    }

    return tcp_send(tcpServer_getClientFileDescriptor(server), data, len);
}
TCP_INTEGER         tcpServer_receiveFromClient(tcp_server_t *server, void *data, ssize_t len)
{
    if (data == NULL)
    {
        data = malloc(len);
    }
    return tcp_recv(tcpServer_getClientFileDescriptor(server), data, len);
}
TCP_INTEGER            tcpServer_acceptMultiClient(tcp_server_t *server, void *(*routine)(void *))
{
    int index = 0;
    int portNumLen = 0;
    char *portNum = NULL;
    int portServer = tcpServer_getPort(server);

    if (portServer == TCP_ERROR)
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

    if (tcpServer_getMultiClient(server) == NULL)
    {
        fprintf(stderr, "[ERROR] Not created thread for multiclient\n");
        return TCP_ERROR;
    }

    while (TCP_TRUE)
    {
        tcpServer_setClientFileDescriptor(server, accept(tcp_getSockFileDescriptor(server->socket), server->clientAddr, server->clientAddrLen));   
        tcp_server_t *copyServer = tcpServer_copyServer(server);
        if (pthread_create(server->clients + index, NULL, routine, copyServer) != 0)
        {
            printf("Have Create thread at: %d\n", index);
        }

        index++;
        if (index >= server->n_client)
        {
            index = 0;
            while (index < server->n_client)
            {
                pthread_join(*(server->clients + index), NULL);
                index++;
            }
            index = 0;
        }
    }
}

TCP_THREAD          tcpServer_getMultiClient(tcp_server_t *server)
{
    return server->clients;
}
TCP_VOID            tcpServer_setMuliClient(tcp_server_t *server, int n_client)
{
    if (server->clients != NULL)
    {
        free(server->clients);
    }
    server->n_client = n_client;
    server->clients = (TCP_THREAD)malloc(sizeof(pthread_t));
}