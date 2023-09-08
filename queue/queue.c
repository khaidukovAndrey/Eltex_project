#include "queue.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

// Инициализация очереди
void init(Queue *q)
{
    q->front = 0;
    q->rear = 0;
    for (int i = 0; i < Q_SIZE; ++i)
    {
        q->pkg_sizes[i] = 0;
    }
    if (pthread_mutex_init(&q->back_mutex, NULL) != 0)
    {
        perror("Unsuccessful mutex initialisation back_mutex: ");
        exit(EXIT_FAILURE);
    }
    if (pthread_mutex_init(&q->front_mutex, NULL) != 0)
    {
        perror("Unsuccessful mutex initialisation front_mutex: ");
        exit(EXIT_FAILURE);
    }
}

int is_full(Queue *q)
{
    if (q->pkg_sizes[q->rear] == 0)
    {
        return 0;
    }
    return 1;
}

// Проверка на наличие элементов в очереди
int is_empty(Queue *q)
{
    if (q->pkg_sizes[q->front] == 0)
    {
        return 1;
    }
    return 0;
}

// Добавление элемента в конец очереди
void push(
        Queue *q, // Очередь
        unsigned char *pkg, // Пакет
        unsigned short pkg_size) // Размер пакета
{
    if (!pkg || pkg_size<1 || pkg_size>MAX_PKG_SIZE) // Проверка корректности аргумента
    {
        return;
    }
    if (!is_full(q)) // Проверка на наличие свободных мест в очереди
    {
        pthread_mutex_lock(&q->back_mutex);
        memcpy(q->queue[q->rear], pkg, pkg_size);
        q->pkg_sizes[q->rear] = pkg_size;
        q->rear++;
        if (q->rear == Q_SIZE)
        {
            q->rear = 0;
        }
        pthread_mutex_unlock(&q->back_mutex);
    }
}

// Удаляет первый элемент из начала очереди
void remove_front(Queue *q)
{
    if(is_empty(q))
    {
        return;
    }
    q->pkg_sizes[q->front] = 0;
    q->front++;
    if (q->front == Q_SIZE)
    {
        q->front = 0;
    }
}

// Получить первый элемент из очереди и удалить его
unsigned short pop(
        Queue *q,
        unsigned char *buff)
{
    pthread_mutex_lock(&q->front_mutex);
    memcpy(buff,q->queue[q->front],q->pkg_sizes[q->front]);
    unsigned short read_bytes = q->pkg_sizes[q->front];
    q->pkg_sizes[q->front] = 0;
    q->front++;
    if (q->front == Q_SIZE)
    {
        q->front = 0;
    }
    pthread_mutex_unlock(&q->front_mutex);
    return read_bytes;
}


//Записывает первый элемент в буфер и возвращает его размер
unsigned short front(
        Queue *q,
        unsigned char *buff)
{
    if (is_empty(q))
    {
        return 0;
    }
    pthread_mutex_lock(&q->front_mutex);
    memcpy(buff,q->queue[q->front],q->pkg_sizes[q->front]);
    pthread_mutex_unlock(&q->front_mutex);
    return q->pkg_sizes[q->front];
}

//Возвращает ссылку на последний и наиболее недавно добавленный элемент в конец
unsigned short back(
        Queue *q,
        unsigned char *buff)
{
    if (is_empty(q))
    {
        return 0;
    }
    pthread_mutex_lock(&q->back_mutex);
    memcpy(buff,q->queue[q->rear-1],q->pkg_sizes[q->rear-1]);
    pthread_mutex_unlock(&q->back_mutex);
    return q->pkg_sizes[q->rear-1];
}