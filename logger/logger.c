#include <stdio.h>
#include<time.h>
#include <string.h>

#define FILE_LOG_NAME "log"

typedef enum log_type
{
    INFO,
    ERROR,
    WARNING
} Type_t;

static FILE *file;

void printL(Type_t, const char *);

int start_log()
{
    if ((file = fopen(FILE_LOG_NAME, "a")) == NULL)
    {
        return -1;
    }

    return 0;
}

int stop_log()
{
    if (fclose(file) == EOF)
    {
        return -1;
    }

    return 0;
}

int get_time(char* buffer, size_t buf_size)
{
    static time_t currentTime;
    static struct tm *time_info;

    // Получить текущее время
    if ((currentTime = time(NULL)) == -1)
    {
        return -1;
    }

    // Преобразовать в структуру tm
    if ((time_info = localtime(&currentTime)) == NULL)
    {
        return -1;
    }

    // Определить формат вывода
    strftime(buffer, buf_size, "%d.%m.%Y %H:%M:%S", time_info);

    return 0;
}


int main()
{
    start_log();

    printL(INFO, "This is just info msg! Have a good day!");
    printL(WARNING, "This is a warning message! Maybe something will go wrong!");
    printL(ERROR, "So now this is a fatal error! The program cannot continue its work. Bye bye!");

    stop_log();
}



void printL(Type_t msg_type, const char *msg)
{

    if (!msg)
    {
        return;
    }
    static char buff[80];

    if (get_time(buff, 80) == -1)
    {
        return;
    }

    fputs(buff, file);

    switch (msg_type)
    {
        case INFO:
        {
            fputs("\t[INFO]\t", file);
            break;
        }
        case WARNING:
        {
            fputs("\t[WARN]\t", file);
            break;
        }
        case ERROR:
        {
            fputs("\t[ERR] \t", file);
            break;
        }
    }

    fputs(msg, file);
    fputc('\n', file);
}