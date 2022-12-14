#ifndef __MINMAX_H__
#define __MINMAX_H__

#include "stack.h"
#include "shared_funcs.h"

int distance_to_gooses(char grid[MAXSTR], int r, int c);
int *find_r(char grid[MAXSTR]);
State *create_new_state(char grid[MAXSTR], char atual_player, char tipo_mov, int *r, int *c, char max);
void minmax(StateList *queue, State *state, char max, int depth);
char *make_move(State *state);

#endif