#ifndef __SHARED_FUNCS_H__
#define __SHARED_FUNCS_H__

#define MAXSTR 512
#define MAXINT 16

typedef struct Move
{
    char role_me;
    char role_enemy;
    char move_type;
    int enemy_move_count;

    int mov_adv_l[MAXINT];
    int mov_adv_c[MAXINT];
} Move;

void get_enemy_move(char buf[MAXSTR], char grid[MAXSTR], Move *move);
void show_received_moves(Move *move);

#endif