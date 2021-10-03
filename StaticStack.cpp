//
// Created by yauhe on 03/10/2021.
//

#include "StaticStack.h"


Stack CreateStack(int maxElements)
{
    Stack s;

    if(maxElements < MIN_STACK_SIZE)
        printf("Stack size is too small\n");

    s = static_cast<Stack>(malloc(sizeof(struct StackRecord)));
    if (s == NULL)
        printf("Out of memory space\n");

    s->array = static_cast<int *>(malloc(sizeof(int) * maxElements));
    if (s->array == NULL)
        printf("Out of memory space\n");

    s->capacity = maxElements;
    MakeEmptyStack(s);
    return s;
}


void MakeEmptyStack(Stack s)
{
    s->topOfStack = EMPTY_TOS;
}


int IsEmptyStack(Stack s)
{
    return s->topOfStack == EMPTY_TOS;
}


int IsFullStack(Stack s)
{
    return s->topOfStack == s->capacity;
}


void PushStack(int x, Stack s)
{
    if ( IsFullStack(s))
        printf("Stack is full\n");
    else
        s->array[++s->topOfStack] = x;
}


int PopStack(Stack s)
{
    if (!IsEmptyStack(s))
        return s->array[s->topOfStack--];
    else
    {
        printf("The stack is empty\n");
        return -1;
    }
}


int TopOfStack(Stack s)
{
    if (!IsEmptyStack(s))
        return s->array[s->topOfStack];
    else
    {
        printf("The stack is empty\n");
        return -1;
    }
}


void DisplayStack(Stack s)
{
    int i;

    printf("Stack content:\n");
    for (i=s->topOfStack; i>=0; i--)
        printf("\t--> %d\n", s->array[i]);
}
