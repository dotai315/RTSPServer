#include "main.h"

void test(void * arg)
{
    char buf[500];
    tcp_server_t *server = (tcp_server_t *)arg;
    tcpServer_receiveFromClient(server, buf, 500);
    printf("Client: %s\n", buf);
    tcpServer_sendClient(server, "DoTai", strlen("DoTai"));
}

int main(int argc, char *argv[])
{
    if (argc != 2)
    {
        fprintf(stderr, "Usage: %s PORT\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    tcp_server_t *server;   

    server = tcpServer_init();
    tcpServer_setPort(server, 1234);
    tcpServer_acceptClient(server, test);
    
    exit(EXIT_SUCCESS);
}