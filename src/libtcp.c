#include "libtcp.h"

static tcp_t *tcp_allocate(void)
{
    tcp_t *newObj = (tcp_t *)malloc(sizeof(tcp_t));
    if (newObj == NULL)
    {
        fprintf(stderr, "[ERROR] Not enough space to allocate tcp object\n");
        exit(EXIT_FAILURE);
    }
    return newObj;
}

static struct addrinfo *tcp_allocateAddrinfo(void)
{
    struct addrinfo *newHints = (struct addrinfo *)malloc(sizeof(struct addrinfo));
    if (newHints == NULL)
    {
        fprintf(stderr, "[ERROR] Not enough space to allocate tcp addrinfo object\n");
        exit(EXIT_FAILURE);
    }
    return newHints;
}

static struct addrinfo **tcp_allocateResAddrinfo(void)
{
    struct addrinfo **newResAddrinfo = (struct addrinfo **)malloc(sizeof(struct addrinfo *));
    if (newResAddrinfo == NULL)
    {
        fprintf(stderr, "[ERROR] Not enough space to allocate tcp result getaddrinfo object\n");
        exit(EXIT_FAILURE);
    }
    return newResAddrinfo;
}

tcp_t *tcp_init(void)
{
    tcp_t *obj; 
    obj = tcp_allocate();
    obj->hints = tcp_allocateAddrinfo();
    obj->hints->ai_addr = NULL;
    obj->hints->ai_canonname = NULL;
    obj->hints->ai_next = NULL;
    obj->hints->ai_family = AF_INET;
    obj->hints->ai_flags = AI_PASSIVE;
    obj->hints->ai_socktype = SOCK_STREAM;
    obj->hints->ai_protocol = IPPROTO_TCP;
    obj->port = TCP_ERROR;
    return obj;
}

int             tcp_getSockFileDescriptor(tcp_t *tcp)
{
    return tcp->sockFd;
}

int            tcp_setSockFileDescriptor(tcp_t *tcp, struct addrinfo *rp)
{
    int *opt = (int *)malloc(sizeof(int));

    *opt = 1;
    tcp->sockFd = socket(rp->ai_family, rp->ai_socktype, rp->ai_protocol);
    if (tcp->sockFd < 0)
    {
        return TCP_ERROR;
    }
    if (setsockopt(tcp->sockFd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, opt, sizeof(opt)))
    {
        return TCP_ERROR;
    }
    return tcp->sockFd;
}

struct addrinfo *tcp_getAddrinfo(tcp_t *tcp)
{
    return tcp->hints;
}
void            tcp_setAddrinfo(tcp_t *tcp, struct addrinfo *newHints)
{
    if (tcp->hints != NULL)
    {
        free(tcp->hints);
        tcp->hints = newHints;
    }
    else 
    {
        tcp->hints = newHints;
    }
}
void            tcp_printAddrinfo(tcp_t *tcp)
{
    char ip[INET_ADDRSTRLEN];
    struct sockaddr_in *info = (struct sockaddr_in *)tcp->hints->ai_addr;
    inet_ntop(AF_INET, &(info->sin_addr.s_addr), ip, INET_ADDRSTRLEN);
}

int             tcp_config(tcp_t *tcp, const char *node, const char *service)
{
    int error = -1;
    struct addrinfo **res = NULL;
    struct addrinfo *rp = NULL;

    res = tcp_allocateResAddrinfo();
    error = getaddrinfo(node, service, tcp->hints, res);
    if (error != 0)
    {
        fprintf(stderr, "[ERROR] getaddrinfo; %s\n", gai_strerror(errno));
        exit(EXIT_FAILURE);
    }

    for(rp = *res; rp != NULL; rp = rp->ai_next)
    {
        if (tcp_setSockFileDescriptor(tcp, rp) < 0)
        {
            continue;
        }

        if (bind(tcp_getSockFileDescriptor(tcp), rp->ai_addr, rp->ai_addrlen) == 0)
        {
            break;
        }

        close(tcp_getSockFileDescriptor(tcp));
    }

    if (rp == NULL)
    {
        fprintf(stderr, "[ERROR] config tcp: %s\n", strerror(errno));
        return TCP_ERROR;
    }

    freeaddrinfo(*res);
    if (listen(tcp_getSockFileDescriptor(tcp), 10) != 0)
    {
        fprintf(stderr, "[ERROR] Listen Failed: %s\n", strerror(errno));
        exit(EXIT_FAILURE);
    }
    return 0;
}

int             tcp_send(tcp_t *tcp, void *data, size_t len)
{
    int retVal = -1;
    retVal = send(tcp_getSockFileDescriptor(tcp), data, len, MSG_NOSIGNAL);
    if (retVal < 0)
    {
        return TCP_ERROR;
    }
    return retVal;
}
int             tcp_recv(tcp_t *tcp, void *data, size_t len)
{
    int retVal = -1;
    retVal = recv(tcp_getSockFileDescriptor(tcp), data, len, 0);
    if (retVal < 0)
    {
        return TCP_ERROR;
    }
    return retVal;
}


TCP_UINT16      tcp_getPort(tcp_t *tcp)
{
    return tcp->port;
}

TCP_VOID        tcp_setPort(tcp_t *tcp, TCP_UINT16 port)
{
    tcp->port = port;
}