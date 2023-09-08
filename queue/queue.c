#include "queue.h"
#include <string.h>

// Инициализация очереди
void init(Queue *q)
{
    q->front = 0;
    q->rear = 0;
    for (int i = 0; i < Q_SIZE; ++i)
    {
        q->pkg_sizes[i] = 0;
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
        memcpy(q->queue[q->rear], pkg, pkg_size);
        q->pkg_sizes[q->rear] = pkg_size;
        q->rear++;
        if (q->rear == Q_SIZE)
        {
            q->rear = 0;
        }
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
    unsigned short read_bytes = front(q, buff);
    remove_front(q);
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
    memcpy(buff,q->queue[q->front],q->pkg_sizes[q->front]);
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
    memcpy(buff,q->queue[q->rear-1],q->pkg_sizes[q->rear-1]);
    return q->pkg_sizes[q->rear-1];
}