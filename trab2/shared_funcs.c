#include <stdio.h>
#include <string.h>

#include "shared_funcs.h"

void get_enemy_move(char *buf, Move *move)
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
}

void show_received_moves(Move *move)
{
    printf("--show_received_moves--\n");
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
    printf("\n----------------\n");
}

int pos_valida(int l, int c)
{
    if (l < 1 || l > 7 || c < 1 || c > 7)
        return 0;
    if ((l < 3 || l > 5) && (c < 3 || c > 5))
        return 0;
    return 1;
}

int aplica(char *buf, char *tabuleiro, char lado, char tipo,
           int num_mov, int *mov_l, int *mov_c)
{
    int i, l, c, p, ln, cn, pn, dist;

    strcpy(buf, tabuleiro);
    if (tipo == 'n')
        return 1;
    l = mov_l[0];
    c = mov_c[0];
    if (!pos_valida(l, c))
        return 0;
    p = POS(l, c);
    if (tipo == 'm')
    {
        if (buf[p] != lado)
            return 0;
        buf[p] = '-';
        ln = mov_l[1];
        cn = mov_c[1];
        if (!pos_valida(ln, cn))
            return 0;
        pn = POS(ln, cn);
        dist = ln + cn - l - c;
        dist = ABS(dist);
        if ((buf[pn] != '-') ||         // destino livre
            ((ln != l) && (cn != c)) || // movimento na linha ou coluna
            (dist != 1))                // movimento simples de 1 passo
            return 0;
        buf[pn] = lado;
    }
    else
    { // tipo s
        if ((lado != 'r') || (buf[p] != 'r'))
            return 0;
        for (i = 1; i < num_mov; i++)
        {
            buf[p] = '-';
            ln = mov_l[i];
            cn = mov_c[i];
            if (!pos_valida(ln, cn))
                return 0;
            pn = POS(ln, cn);
            dist = ln + cn - l - c;
            dist = ABS(dist);
            if ((buf[pn] != '-') ||         // destino livre
                ((ln != l) && (cn != c)) || // movimento na linha ou coluna
                (dist != 2))                // movimento de salto, 2 passos
                return 0;
            l = (l + ln) / 2; // posicao do ganso que sera saltado
            c = (c + cn) / 2;
            p = POS(l, c);
            if (buf[p] != 'g')
                return 0;
            buf[p] = '-';
            buf[pn] = 'r';
            l = ln;
            c = cn;
            p = pn;
        }
    }
    return 1;
}
