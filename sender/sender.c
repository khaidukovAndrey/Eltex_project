#include <string.h>
#include "../tagger/vlan_tagger.h"
#include "../logger/logger.h"
#include "errno.h"
#include "../init/pthread_init.h"

void *packet_sender(void *thread_data)
{
    int bytes_read;
    unsigned char buffer[ETHERNET_FRAME_LENGTH] = { 0 };
    struct thread_data params = { 0 };

    params = *((struct thread_data *)thread_data);
    while(1)
    {
        bytes_read = pop(params.sender_queue, buffer);
        printf("Получено из очереди %d байт\n", bytes_read);

        ssize_t bytes_sent = sendto(*params.socket, buffer, bytes_read, 0, (struct sockaddr *)params.saddr, *params.saddr_len);
        if (bytes_sent == -1)
        {
            printf("Symbols was written: %d\n",
                   printL(ERROR, SENDER, "Error sending packets (error code: %d)!", errno));
            logging_programm_completion(params);
            stop_log();
            exit(EXIT_FAILURE);
        }
        else
        {
            printf("Отправлено %ld байт\n", bytes_sent);
        }
        memset(buffer, 0, ETHERNET_FRAME_LENGTH);
    }
    pthread_exit(NULL);
}