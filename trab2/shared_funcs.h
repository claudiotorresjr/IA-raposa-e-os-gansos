#ifndef __SHARED_FUNCS_H__
#define __SHARED_FUNCS_H__

#define MAXSTR 512
#define MAXINT 16
#define MIN -1000000
#define MAX 1000000

#define FOGE 10
#define PROTEGE 30
#define AVANCA 5

#define POS(l,c) ((l) * 10 + (c))
#define ABS(x) ((x < 0)?(-(x)):(x))

typedef struct Move
{
    char role_me;
    char role_enemy;
    char move_type;
    int enemy_move_count;

    int mov_adv_l[MAXINT];
    int mov_adv_c[MAXINT];
} Move;

void get_enemy_move(char *buf, Move *move);
void show_received_moves(Move *move);
int pos_valida(int l, int c);
int aplica(char *buf, char *tabuleiro, char lado, char tipo,
           int num_mov, int *mov_l, int *mov_c);

#endif