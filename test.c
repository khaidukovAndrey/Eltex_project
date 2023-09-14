#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>
#include <linux/if_packet.h>
#include <netinet/in.h>
#include <netinet/if_ether.h>

int main(int argc, char *argv[])
{
    int saddr_len;
    int sock_r;
    int buffer_len;
    int i = 1;
    unsigned char buffer[1522] = { 0 };
    struct sockaddr_ll saddr = { 0 };

    sock_r = socket(
            AF_PACKET,
            SOCK_RAW,
            htons(ETH_P_ALL));
    if (sock_r < 0)
    {
        perror("Socket opening error");
        exit(EXIT_FAILURE);
    }

    if (setsockopt(sock_r, SOL_SOCKET, SO_BINDTODEVICE, argv[1], strlen(argv[1])) < 0)
    {
        perror("Error setting socket options");
        close(sock_r);
        exit(EXIT_FAILURE);
    }
    saddr.sll_family = AF_PACKET;
    saddr.sll_protocol = htons(ETH_P_ALL);

    while(i)
    {
        saddr_len = sizeof saddr;
        buffer_len = recvfrom(sock_r, buffer, sizeof(buffer), 0, (struct sockaddr *)&saddr, (socklen_t *)&saddr_len);
        if (buffer_len < 0)
        {
            perror("Error receiving packets");
            close(sock_r);
            exit(EXIT_FAILURE);
        }
        i--;
    }

    ssize_t bytes_sent = sendto(sock_r, buffer, buffer_len, 0, (struct sockaddr *)&saddr, sizeof(saddr));
    if (bytes_sent == -1)
    {
        perror("err send");
    }
    else
    {
        printf("Отправлено %zd байт данных\n", bytes_sent);
    }
}