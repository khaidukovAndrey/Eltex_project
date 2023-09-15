#include "queue.h"
#include <stdio.h>
#include <string.h>
#include <unistd.h>

// Output: Was read 19 bytes: This is first pkg!


Queue_t queue;

void* func_to_write(void* arg)
{
    char buff[100];
    ssize_t writen_bytes;
    for (int i = 0; i < 20; ++i)
    {
        sleep(1);
        if (i%2 == 0)
        {
            sprintf(buff,"This is print №%d",i);
            writen_bytes = push(&queue, buff, strlen(buff)+1);
        }
        else
        {
            sprintf(buff,"This is another print №%d",i);
            writen_bytes = push(&queue, buff, strlen(buff)+1);
        }
        printf("push (%ld) completed!\n", writen_bytes);
    }
    pthread_exit(NULL);
}

void* func_to_read(void* arg)
{
    char buff[100];
    unsigned short read_bytes;
    while (1)
    {
        read_bytes = pop(&queue,buff);
        printf("read %d: %s\t",read_bytes ,buff);
        printf("read completed!\n");
    }
    pthread_exit(NULL);
}


int main()
{
    init(&queue);

    pthread_t threads[2];
    pthread_create(&threads[0],NULL,func_to_read,NULL);
    pthread_create(&threads[1],NULL,func_to_write,NULL);

    for (int i = 0; i < 2; ++i) {
        pthread_join(threads[i],NULL);
    }

    queue_destroy(&queue);

}
