#include "vlan_tagger.h"

void *packet_sniffer(void *thread_data)
{
    int buffer_len;
    unsigned char buffer[ETHERNET_FRAME_LENGTH] = { 0 };
    struct thread_data params = { 0 };

    params = *((struct thread_data *)thread_data);

    while(1)
    {
        if (is_full(params.sniffer_queue) != 1)
        {
            buffer_len = recvfrom(*params.socket, buffer, sizeof(buffer), 0, (struct sockaddr *)params.saddr, (socklen_t *)params.saddr_len);
            if (buffer_len < 0)
            {
                printf("Symbols was written: %d\n",
                       printL(ERROR, SNIFFER, "Error receiving packets (error code: %d)!", errno));
                logging_programm_completion(params);
                stop_log();
                exit(EXIT_FAILURE);
            }
            int po = push(params.sniffer_queue, buffer, buffer_len);
            printf("Отправлено в очередь %d байт\n", po);
        }
    }

}
