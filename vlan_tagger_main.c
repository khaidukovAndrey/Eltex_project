#include "vlan_tagger.h"


int main(
        int     argc,
        char    *argv[])
{
    if (argc != 2)
    {
        printf("Run the daemon like this: sudo ./vlan_tagger <name of the network interface>\n");
        exit(EXIT_FAILURE);
    }

    pthread_init(argv[1]);
}
