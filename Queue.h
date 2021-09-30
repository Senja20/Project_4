//
// Created by yauhe on 22/09/2021.
//

#ifndef PROJECT_4_QUEUE_H
#define PROJECT_4_QUEUE_H

struct QueueRecord
{
    int capacity;   // max size
    int front;      //
    int rear;       // last element
    int size;       // the current size
    int *array;     //
};

typedef struct QueueRecord *Queue;


Queue CreateQueue(int);
void MakeEmptyQueue(Queue);
int Succ(int, Queue);
void Enqueue(int, Queue);
int Dequeue(Queue);
int FrontOfQueue(Queue);
int RearOfQueue(Queue);
int IsFullQueue(Queue);
int IsEmptyQueue(Queue);
void DisplayQueue(Queue);

#endif //PROJECT_4_QUEUE_H
