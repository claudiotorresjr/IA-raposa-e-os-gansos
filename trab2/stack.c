#include <stdlib.h>

#include "stack.h"

void push(StateQueue *queue, State *s)
{    
    if(!queue->top)
    {
        s->prev = NULL;
    }
    else
    {
        s->prev = queue->top;
    }
    queue->top = s;
}

State *pop(StateQueue *queue)
{
    if (!queue->top)
    {
        return NULL;
    }

    State *aux = queue->top;
    queue->top = queue->top->prev;

    aux->prev = NULL;

    return aux;
}