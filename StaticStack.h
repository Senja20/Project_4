//
// Created by yauhe on 03/10/2021.
//

#ifndef PROJECT_4_STATICSTACK_H
#define PROJECT_4_STATICSTACK_H

#include <stdio.h>
#include <stdlib.h>


#define EMPTY_TOS -1
#define MIN_STACK_SIZE 5
#define FALSE 0
#define TRUE 1


struct StackRecord
{
    int capacity;
    int topOfStack;
    int *array;
};

typedef struct StackRecord *Stack;


Stack CreateStack(int);
void MakeEmptyStack(Stack);
void PushStack(int, Stack);
int PopStack(Stack);
int TopOfStack(Stack);
int IsFullStack(Stack);
int IsEmptyStack(Stack);
void DisplayStack(Stack);

#endif //PROJECT_4_STATICSTACK_H
