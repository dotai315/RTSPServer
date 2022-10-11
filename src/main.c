#include "main.h"

int main(int argc, char *argv[])
{
    if (argc != 2)
    {
        fprintf(stderr, "Usage: %s PORT\n", argv[0]);
        exit(EXIT_FAILURE);
    }
    tcp_t *newTCP;
    newTCP = tcp_init();
    tcp_config(newTCP, NULL, argv[1]);
    exit(EXIT_SUCCESS);
}