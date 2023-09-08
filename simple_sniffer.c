#include "sniffer_header.h"

void packet_sniffer(char *interface_name){
    int sock_r;
    int saddr_len;
    int buffer_len;
    unsigned char buffer[1522] = {0};
    struct sockaddr_ll saddr = {0};

    sock_r = socket(AF_PACKET,SOCK_RAW,htons(ETH_P_ALL));
    if (sock_r < 0)
    {
        perror("Socket opening error");
        exit(EXIT_FAILURE);
    }

    saddr.sll_family = AF_PACKET;
    saddr.sll_protocol = htons(ETH_P_ALL);

    if (setsockopt(sock_r, SOL_SOCKET, SO_BINDTODEVICE, interface_name, strlen(interface_name)) < 0)
    {
        perror("Error setting socket options");
        close(sock_r);
        exit(EXIT_FAILURE);
    }

    while(1)
    {
        saddr_len = sizeof saddr;
        buffer_len = recvfrom(sock_r, buffer, 1522, 0, (struct sockaddr*)&saddr, (socklen_t*)&saddr_len);
        if (buffer_len < 0)
        {
            perror("Error receiving packets");
            close(sock_r);
            exit(EXIT_FAILURE);
        }
    }
    close(sock_r);
}
