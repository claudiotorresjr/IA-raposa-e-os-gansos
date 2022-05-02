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

    State *new = (State *)malloc(sizeof(State));
    new->max = s->max;
    new->min = s->min;
    new->prev = NULL;

    if (s->role == 'g')
    {
        new->role = 'r';
    }
    else
    {
        new->role = 'g';
    }
    new->init_pos[0] = s->init_pos[0];
    new->init_pos[1] = s->init_pos[1];
    new->new_pos[0] = s->new_pos[0];
    new->new_pos[1] = s->new_pos[1];
    new->move_type = s->move_type;
    
    queue->top = new;
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