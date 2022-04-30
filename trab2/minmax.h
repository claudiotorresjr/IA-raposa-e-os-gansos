#ifndef __MINMAX_H__
#define __MINMAX_H__

#include "stack.h"
#include "shared_funcs.h"

int distance_to_gooses(char grid[MAXSTR], int r, int c);
int *find_r(char grid[MAXSTR]);
State *create_new_state(char grid[MAXSTR], char atual_player, char result[MAXSTR], int *r, int *c);
char *minmax(State *state, char atual_player);
char *make_move(State *state, char role);

#endif