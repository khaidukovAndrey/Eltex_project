#include <stdio.h>
#include <stdlib.h>
#include <sys/resource.h>
#include "pthread_init.h"

int main(
        int     argc,
        char    *argv[])
{
    if (argc != 2)
    {
        printf("Run the daemon like this: sudo ./vlan_tagger <name of the network interface>\n");
        exit(EXIT_FAILURE);
    }
    struct rlimit rlim;

    rlim.rlim_cur = 8 * 1024 * 1024; // 8 MB in bytes
    rlim.rlim_max = 8 * 1024 * 1024; // 8 MB in bytes

    if (setrlimit(RLIMIT_STACK, &rlim) == 0) {
        printf("Stack size limit set successfully.\n");
    } else {
        perror("Error setting stack size limit");
        return 1;
    }

    pthread_init(argv[1]);
}

#include <stdio.h>
