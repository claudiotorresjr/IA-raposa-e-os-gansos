#ifndef _STACK_H
#define _STACK_H

#include <stdio.h>
#include <stdlib.h>

#include "shared_funcs.h"

typedef struct State
{
    char grid[MAXSTR];
    int min;
    int max;
    char move_type;
    int init_pos[2];
    int new_pos[2];
    struct State *prev;
} State;

typedef struct StateQueue
{
    struct State *top;
} StateQueue;

void push(StateQueue *queue, State *s);
State *pop(StateQueue *queue);

#endif