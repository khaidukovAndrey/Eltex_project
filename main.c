#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "pthread_init.h"
#include <signal.h>

volatile int running = 1;

void signal_handlerd(int signo) {
    if (signo == SIGTERM) {
        running = 0;
    }
}

int main(int argc, char *argv[]) {
    if (argc != 2) {
        printf("Usage: %s <name of the network interface>\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    // Установите обработчик сигнала SIGTERM для завершения демона.
    signal(SIGTERM, signal_handlerd);

    // Форкните процесс, чтобы создать демона.
    pid_t pid = fork();
    if (pid < 0) {
        perror("fork");
        exit(EXIT_FAILURE);
    }

    if (pid > 0) { // Родительский процесс
        printf("Daemon started with PID: %d\n", pid);
        exit(EXIT_SUCCESS);
    }

    // Демон продолжает работу здесь.

    // Отсоединяемся от текущего терминала и создаем новую сессию.
    if (setsid() < 0) {
        perror("setsid");
        exit(EXIT_FAILURE);
    }

    // Закрываем стандартные файловые дескрипторы STDIN, STDOUT и STDERR.
    close(STDIN_FILENO);
    close(STDOUT_FILENO);
    close(STDERR_FILENO);

    // Здесь можно устанавливать текущий рабочий каталог и другие настройки, если необходимо.

    // Инициализируем ваш демон.
    pthread_init(argv[1]);

    // Вход в основной цикл демона.
    while (running) {
        // Здесь можно выполнять какую-либо полезную работу или ожидать события.
        sleep(1); // Пример: ожидание в течение 1 секунды.
    }

    printf("Daemon stopped\n");
    exit(EXIT_SUCCESS);
}