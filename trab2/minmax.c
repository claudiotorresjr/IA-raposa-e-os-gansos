#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "minmax.h"

int ganso_burro[2] = {3, 5};

int distance_to_gooses(char grid[MAXSTR], int r, int c)
{
    int distance = 0;
    for(int i = r; i < 8; ++i)
    {
        if (grid[POS(i, c)] == 'g')
        {
            distance++;
            break;
        }
    }
    return distance;
}

int *find_r(char grid[MAXSTR])
{
    int *pos = (int *)malloc(2*sizeof(int));
    for(int i = 1; i < 8; ++i)
    {
        for(int j = 1; j < 8; ++j)
        {
            if (grid[POS(i,j)] == 'r')
            {
                pos[0] = i;
                pos[1] = j;
                return pos;
            }
        }
    }

    return NULL;
}

State *create_new_state(char grid[MAXSTR], char atual_player, char result[MAXSTR], int *r, int *c)
{
    int i;
    //acha onde ta o inicio do tabuleiro
    for (i = 0; i < MAXSTR; ++i)
    {
        if (grid[i] == '#')
        {
            break;
        }
    }
    char *aux_grid = &(grid[i]);

    char tipo_mov;
    //verifica se a nova posicao ta livre
    //se sim, move
    // printf("%c %d %d %c\n", atual_player, r[1], c[1], aux_grid[POS(r[1], c[1])]);
    if (aux_grid[POS(r[1], c[1])] == '-')
    {
        tipo_mov = 'm';
    }
    //se nao, salta
    else if (atual_player == 'r' && aux_grid[POS(r[1], c[1])] == 'g')
    {
        tipo_mov = 's';
    }
    else
    {
        return NULL;
    }

    if (strlen(result) == 1)
    {
        sprintf(result + strlen(result), " %c %d %d", tipo_mov, r[0], c[0]);
    }

    State *new = (State *)malloc(sizeof(State));
    new->max = MAX;
    new->prev = NULL;
    new->new_pos[0] = r[1];
    new->new_pos[1] = c[1];

    aplica(new->grid, grid, atual_player, tipo_mov, 1, r, c);
    new->min = distance_to_gooses(new->grid, r[1], c[1]);

    return new;
}

char *minmax(State *state, char atual_player)
{
    int depth = 0;
    char save_player = atual_player;
    char *result = (char *)malloc(MAXSTR*sizeof(char));
    result[0] = save_player;

    StateQueue *queue = (StateQueue *)malloc(sizeof(StateQueue));
    queue->top = NULL;

    push(queue, state);

    while (queue->top)
    {   
        //pega o primeiro state da pilha
        State *current = pop(queue);

        //se chegou na profundidade 2, nao faz mais nada
        if (depth == 2)
        {
            continue;
        }

        int move_r[2];
        int move_c[2];
        if (atual_player == 'r')
        {
            int k;
            for (k = 0; k < MAXSTR; ++k)
            {
                if (current->grid[k] == '#')
                {
                    break;
                }
            }
            int *pos = find_r(&(current->grid[k]));
            int r = pos[0];
            int c = pos[1];

            move_r[0] = r;
            move_c[0] = c;
            // printf("init %d %d\n", r, c);

            State *states[4];

            //faz um estado novo para cada nova posicao e calcula o minmax
            //modificando o mapa atual com a nova posicao
            // -1 0
            // 0 -1
            // 0 1
            // 1 0
            int s_pos = 0;
            for (int p_x = -1; p_x < 2; p_x++)
            {
                for (int p_y = -1; p_y < 2; p_y++)
                {
                    if (p_x * p_y == 0 && p_x != p_y && pos_valida(r+p_x, c+p_y))
                    {
                        move_r[1] = r+p_x;
                        move_c[1] = c+p_y;
                        states[s_pos++] = create_new_state(current->grid, atual_player, result, move_r, move_c);
                    }
                }
            }

            int min = states[0]->min;
            int p = 0;
            for (int i = 1; i < 4; ++i)
            {
                if (states[i] && states[i]->min < min)
                {
                    min = states[i]->min;
                    p = i;
                }
            }
            push(queue, states[p]);

            if (atual_player == save_player)
            {
                printf("result: %s\n", result);
                sprintf(result + strlen(result), " %d %d", states[p]->new_pos[0], states[p]->new_pos[1]);
            }
            
            atual_player = 'g';
        }
        //o movimento dos gansos. por enquanto move so 1 loucamente pra baixo
        else
        {
            move_r[0] = ganso_burro[0];
            move_c[0] = ganso_burro[1];
            if(pos_valida(move_r[0]+1, move_c[0]))
            {
                // printf("ganso %d %d\n", ganso_burro[0], ganso_burro[1]);
                move_r[1] = move_r[0]+1;
                move_c[1] = move_c[0];
                ganso_burro[0] = move_r[1];
                ganso_burro[1] = move_c[1];
                State *new = create_new_state(current->grid, atual_player, result, move_r, move_c);
                
                push(queue, new);

                if (atual_player == save_player)
                {
                    sprintf(result + strlen(result), " %d %d", new->new_pos[0], new->new_pos[1]);
                }
                
                atual_player = 'r';
            }
        }

        depth++;

        free(current);   
    }
    
    sprintf(result + strlen(result), "\n");

    // printf("result: %s", result);
    return result;
}

char *make_move(State *state, char role)
{
    char *line = (char *)malloc(MAXSTR*sizeof(char));
    line = minmax(state, role);

    return line;
}