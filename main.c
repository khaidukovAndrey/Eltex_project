#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "pthread_init.h"

int main(int argc, char *argv[])
{
    if (argc != 2)
    {
        printf("Usage: %s <name of the network interface> or <-k - for kill existing daemon>\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    // форк для создания демона
    pid_t pid = fork();
    if (pid < 0)
    {
        perror("FORK");
        exit(EXIT_FAILURE);
    }

    if (pid > 0) // Родительский процесс
    {
        printf("Daemon started with PID: %d\n", pid);
        exit(EXIT_SUCCESS);
    }

    // Отсоединяемся от текущего терминала и создаем новую сессию
    if (setsid() < 0)
    {
        perror("SETSID");
        exit(EXIT_FAILURE);
    }

    // Закрываем стандартные файловые дескрипторы STDIN, STDOUT и STDERR.
    close(STDIN_FILENO);
    close(STDOUT_FILENO);
    close(STDERR_FILENO);

    pthread_init(argv[1]);

    exit(EXIT_SUCCESS);
}