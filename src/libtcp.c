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
    return obj;
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
