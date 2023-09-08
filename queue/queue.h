#ifndef QUEUE_QUEUE_H
#define QUEUE_QUEUE_H
#include<pthread.h>


#define Q_SIZE 10 // Длина очереди
#define MAX_PKG_SIZE 1522 // Максимальная длина загружаемого пакета


// Структура очереди
typedef struct Queue
{
    unsigned char queue[Q_SIZE][MAX_PKG_SIZE];
    unsigned short pkg_sizes[Q_SIZE];
    unsigned short front; // позиция первого элемента
    unsigned short rear; // позиция последнего элемента
    pthread_mutex_t front_mutex;
    pthread_mutex_t back_mutex;
} Queue;

void init(Queue *); // Инициализация очереди

int is_full(Queue *); // Проверка заполненности очереди

int is_empty(Queue *); // Проверка наличия элементов в очереди

// Добавление элемента в очередь в конец
void push(
        Queue *, // Очередь
        unsigned char *, // Пакет
        unsigned short); // Размер пакета


void remove_front(Queue *); // Удаление первого элемента очереди

unsigned short pop(
        Queue *q, // Очередь
        unsigned char *buff); // Буфер, в который будет занесен пакет

//Получить первый элемент из очереди и записать его в буфер, функция возвращает кол-во записанных байт
unsigned short front(
        Queue *q,
        unsigned char *buff);

unsigned short back(
        Queue *q, // Очередь
        unsigned char *buff); // Буфер, в который будет занесен пакет

#endif //QUEUE_QUEUE_H
