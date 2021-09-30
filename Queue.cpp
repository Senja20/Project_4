// Created by Marija and Yauhen

#include "Queue.h"
#include <cstdio>
#include <cstdlib>

#define MIN_QUEUE_SIZE 5

Queue CreateQueue(int maxElements)
{
    Queue q;

    if(maxElements < MIN_QUEUE_SIZE)
        printf("Queue size is too small\n");

    q = static_cast<Queue>(malloc(sizeof(struct QueueRecord)));
    if (q == NULL)
        printf("Out of memory space\n");

    q->array = static_cast<int *>(malloc(sizeof(int) * maxElements));
    if (q->array == NULL)
        printf("Out of memory space\n");

    q->capacity = maxElements;
    MakeEmptyQueue(q);
    return q;
}

void MakeEmptyQueue(Queue q)
{
    q->size = 0;
    q->front = 1;
    q->rear = 0;
}

int IsEmptyQueue(Queue q)
{
    return (q->size == 0);
}

int IsFullQueue(Queue q)
{
    return (q->size == q->capacity);
}

int Succ(int val, Queue q)
{
    if (++val == q->capacity)
        val = 0;
    return val;
}

void Enqueue(int x, Queue q)
{
    if (IsFullQueue(q))
        printf("Queue is full!\n");
    else
    {
        q->size++;
        q->rear = Succ(q->rear, q);
        q->array[q->rear] = x;
    }
}

int Dequeue(Queue q)
{
    int val;

    if (!IsEmptyQueue(q))
    {
        val = q->array[q->front];
        q->size--;
        q->front = Succ(q->front, q);
        return val;
    }
    else
    {
        printf("The queue is empty\n");
        return -1;
    }
}

int FrontOfQueue(Queue q)
{
    if (!IsEmptyQueue(q))
        return q->array[q->front];
    else
    {
        printf("The queue is empty\n");
        return -1;
    }
}

int RearOfQueue(Queue q)
{
    if (!IsEmptyQueue(q))
        return q->array[q->rear];
    else
    {
        printf("The queue is empty\n");
        return -1;
    }
}

void DisplayQueue(Queue q)
{
    int i, pos;

    pos=q->front;
    printf("Queue content:\n");

    for (i=0; i<q->size; i++)
    {
        printf("\t--> %d\n", q->array[pos]);
        pos = Succ(pos, q);
    }
}
