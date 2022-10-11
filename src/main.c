#include "main.h"

int main(int argc, char *argv[])
{
    if (argc != 2)
    {
        fprintf(stderr, "Usage: %s PORT\n", argv[0]);
        exit(EXIT_FAILURE);
    }
    printf("Hello World\n");
    exit(EXIT_SUCCESS);
}