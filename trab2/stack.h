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
    char role;
    char move_type;
    int init_pos[2];
    int new_pos[2];
    struct State *prev;
} State;

typedef struct StateQueue
{
    int depth;
    struct State *top;
} StateQueue;

typedef struct StateList
{
    int size;
    struct State *list[3];
} StateList;

void push(StateQueue *queue, State *s);
State *pop(StateQueue *queue);

#endif