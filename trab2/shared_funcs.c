#include <stdio.h>
#include <string.h>

#include "shared_funcs.h"

void get_enemy_move(char buf[MAXSTR], char grid[MAXSTR], Move *move)
{
    // separa os elementos do string recebido
    sscanf(strtok(buf, " \n"), "%c", &(move->role_me));
    sscanf(strtok(NULL, " \n"), "%c", &(move->role_enemy));
    sscanf(strtok(NULL, " \n"), "%c", &(move->move_type));

    if (move->move_type == 'm')
    {
        move->enemy_move_count = 2;
        for (int i = 0; i < move->enemy_move_count; ++i)
        {
            sscanf(strtok(NULL, " \n"), "%d", &(move->mov_adv_l[i]));
            sscanf(strtok(NULL, " \n"), "%d", &(move->mov_adv_c[i]));
        }
    }
    else if (move->move_type == 's')
    {
        sscanf(strtok(NULL, " \n"), "%d", &move->enemy_move_count);
        for (int i = 0; i < move->enemy_move_count; ++i)
        {
            sscanf(strtok(NULL, " \n"), "%d", &(move->mov_adv_l[i]));
            sscanf(strtok(NULL, " \n"), "%d", &(move->mov_adv_c[i]));
        }
    }
    strncpy(grid, buf, MAXSTR);
}

void show_received_moves(Move *move)
{
    printf("%c\n", move->role_me);
    printf("%c %c", move->role_enemy, move->move_type);
    if (move->move_type != 'n')
    {
        if (move->move_type == 's')
            printf(" %d", move->enemy_move_count);
        for (int i = 0; i < move->enemy_move_count; i++)
        {
            printf(" %d", move->mov_adv_l[i]);
            printf(" %d", move->mov_adv_c[i]);
        }
    }
    printf("\n");
}