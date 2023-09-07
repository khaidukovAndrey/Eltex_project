#include <stdio.h>
#include <string.h>

#define Q_SIZE 100
#define MAX_PKG_SIZE 1522

// Структура очереди
typedef struct Queue
{
    unsigned char queue[Q_SIZE][MAX_PKG_SIZE];
    unsigned short front; // позиция первого элемента
    unsigned short rear; // позиция последнего элемента
} Queue;

// Инициализация очереди
void init(Queue *q)
{
    q->front = 0;
    q->rear = 0;
}

int is_full(Queue *q)
{
    if (q->rear < Q_SIZE-1)
    {
        return 0;
    }
    return 1;
}

// Проверка на наличие элементов в очереди
int is_empty(Queue *q) {
    if(q->rear < q->front)
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
        memcpy(q->queue[q->rear++], pkg, pkg_size);
    }
}

//Возвращает ссылку на первый элемент в начале
unsigned char* front(Queue *q)
{
    if (is_empty(q))
    {
        return NULL;
    }
    return q->queue[q->front];
}

//Возвращает ссылку на последний и наиболее недавно добавленный элемент в конец
unsigned char* back(Queue *q)
{
    if (is_empty(q))
    {
        return NULL;
    }
    return q->queue[q->rear-1];
}
