#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "minmax.h"

int *gooses_to_move(char grid[MAXSTR], int r, int c)
{
    //vetor que guarda a posicao de todos os gansos que estao em perigo
    //e dos gansos que podem proteger o ganso em perigo
    int *pos = (int *)malloc(26*sizeof(int));
    int i;
    for (i = 0; i < 26; ++i)
    {
        pos[i] = -1;
    }
    
    i = 0;
    for(int k = 1; k < 8; ++k)
    {
        for(int j = 1; j < 8; ++j)
        {
            if (grid[POS(k, j)] == 'g')
            {
                pos[i++] = k;
                pos[i++] = j;
            }
        }
    }

    return pos;
}

int goose_score(char grid[MAXSTR], int *r, int *c)
{
    int init_x = r[0];
    int init_y = c[0];
    int x = r[1];
    int y = c[1];

    for (int p_x = -1; p_x < 2; p_x++)
    {
        for (int p_y = -1; p_y < 2; p_y++)
        {
            if (p_x * p_y == 0 && p_x != p_y)
            {
                //verifica se o ganso na posicao incial protegia algum outro ganso
                if (pos_valida(init_x+p_x, init_y+p_y) && grid[POS(init_x+p_x, init_y+p_y)] == 'g')
                {
                    //se o ganso protegido ta embaixo
                    if (p_x == 1)
                    {
                        //verifica se embaixo desse ganso protegido tem a raposa
                        //se for, esse movimento foi ruim pois deixou o ganso desprotegido
                        if(pos_valida(init_x+p_x+1, init_y+p_y) && grid[POS(init_x+p_x+1, init_y+p_y)] == 'r')
                        {
                            return -PROTEGE;
                        }
                    }
                    //se o ganso protegido ta em cima
                    else if (p_x == -1)
                    {
                        //verifica se em cima desse ganso protegido tem a raposa
                        //se for, esse movimento foi ruim pois deixou o ganso desprotegido
                        if(pos_valida(init_x+p_x-1, init_y+p_y) && grid[POS(init_x+p_x-1, init_y+p_y)] == 'r')
                        {
                            return -PROTEGE;
                        }

                    }
                    //se o ganso protegido ta na direita
                    else if (p_y == 1)
                    {
                        //verifica se na direita desse ganso protegido tem a raposa
                        //se for, esse movimento foi ruim pois deixou o ganso desprotegido
                        if(pos_valida(init_x+p_x, init_y+p_y+1) && grid[POS(init_x+p_x, init_y+p_y+1)] == 'r')
                        {
                            return -PROTEGE;
                        }

                    }
                    //se o ganso protegido ta na esquerda
                    else if (p_y == -1)
                    {
                        //verifica se na esquerda desse ganso protegido tem a raposa
                        //se for, esse movimento foi ruim pois deixou o ganso desprotegido
                        if(pos_valida(init_x+p_x, init_y+p_y-1) && grid[POS(init_x+p_x, init_y+p_y-1)] == 'r')
                        {
                            return -PROTEGE;
                        }

                    }
                }

                //se nao, verifica se passou a proteger algum ganso
                else if (pos_valida(x+p_x, y+p_y) && grid[POS(x+p_x, y+p_y)] == 'g')
                {
                    //se o ganso protegido ta embaixo
                    if (p_x == 1)
                    {
                        //verifica se protegeu um ganso embaixo, que embaixo dele tem a raposa
                        if(pos_valida(x+p_x+1, y+p_y) && grid[POS(x+p_x+1, y+p_y)] == 'r')
                        {
                            return PROTEGE;
                        }
                    }
                    //se o ganso protegido ta em cima
                    else if (p_x == -1)
                    {
                        //verifica se protegeu um ganso em cima, que em cima dele tem a raposa
                        if(pos_valida(x+p_x-1, y+p_y) && grid[POS(x+p_x-1, y+p_y)] == 'r')
                        {
                            return PROTEGE;
                        }

                    }
                    //se o ganso protegido ta na direita
                    else if (p_y == 1)
                    {
                        //verifica se protegeu um ganso na direita, que na direita dele tem a raposa
                        if(pos_valida(x+p_x, y+p_y+1) && grid[POS(x+p_x, y+p_y+1)] == 'r')
                        {
                            return PROTEGE;
                        }

                    }
                    //se o ganso protegido ta na esquerda
                    else if (p_y == -1)
                    {
                        //verifica se protegeu um ganso na esquerda, que na esquerda dele tem a raposa
                        if(pos_valida(x+p_x, y+p_y-1) && grid[POS(x+p_x, y+p_y-1)] == 'r')
                        {
                            return PROTEGE;
                        }

                    }
                }
        
                //se nao, verifica se ta fugindo da raposa
                else if (pos_valida(init_x+p_x, init_y+p_y) && grid[POS(init_x+p_x, init_y+p_y)] == 'r')
                {
                    return FOGE;
                }
                //se nao, verifica se ta do lado da raposa
                else if (pos_valida(x+p_x, y+p_y) && grid[POS(x+p_x, y+p_y)] == 'r')
                {
                    return -PROTEGE;
                }
            }
        }
    }

    int *pos = find_r(grid);
    int v = distance_to_gooses(grid, pos[0], pos[1]);
    //faz qualquer movimento
    return AVANCA + ABS(v);
}

int distance_to_gooses(char grid[MAXSTR], int r, int c)
{
    //salva a quantidade de casas livres ao redor do ganso, seguido do s
    int pos[26];
    int idx = 0;

    int s;
    char aux_grid[MAXSTR];
    strcpy(aux_grid, grid);
    //for para ir montando submatrizes tendo a raposa como elemento central
    //o valor do s e a distancia de um possivel ganso, caso esse seja encontrado na submatriz
    for (s = 1; s < 8; ++s)
    {
        for (int i = r-s; i <= r+s; ++i)
        {
            for (int j = c-s; j <= c+s; ++j)
            {
                //se encontrar algum ganso na submatriz,
                if(pos_valida(i, j) && aux_grid[POS(i, j)] == 'g')
                {
                    int cont = 0;
                    for (int p_x = -1; p_x < 2; p_x++)
                    {
                        for (int p_y = -1; p_y < 2; p_y++)
                        {
                            if (p_x * p_y == 0 && p_x != p_y)
                            {
                                //verifica se tem uma casa vazia proxima.
                                if (pos_valida(i+p_x, j+p_y) && aux_grid[POS(i+p_x, j+p_y)] == '-')
                                {
                                    cont++;
                                }
                            }
                        }
                    }

                    //deixa posicao como visitada (retira o ganso)
                    // aux_grid[POS(i, j)] = '!';

                    //salva info do ganso encontrada
                    pos[idx++] = cont;
                    pos[idx++] = s;
                }
            }
        }

        //achou todos os gansos. s eh o mais distante
        if (idx == 26)
        {
            break;
        }
    }

    //s servira como peso. gansos mais proximos da raposa tera o s como peso maximo que sera multiplicado
    //pelo total de casas vazias ao redor. sao gansos mais propensos a serem devorados
    int total = 0;
    for (int i = 0; i < 26; i+=2)
    {
        total += pos[i]*(s - pos[i+1]);
    }

    return total;
}

int *find_r(char grid[MAXSTR])
{
    int *pos = (int *)malloc(2*sizeof(int));
    for(int i = 1; i < 8; ++i)
    {
        for(int j = 1; j < 8; ++j)
        {
            if (grid[POS(i, j)] == 'r')
            {
                pos[0] = i;
                pos[1] = j;
                return pos;
            }
        }
    }

    return NULL;
}

State *create_new_state(char grid[MAXSTR], char atual_player, char tipo_mov, int *r, int *c, char max)
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

    State *new = (State *)malloc(sizeof(State));
    new->max = MAX;
    new->min = MIN;
    new->prev = NULL;
    new->init_pos[0] = r[0];
    new->init_pos[1] = c[0];
    new->new_pos[0] = r[1];
    new->new_pos[1] = c[1];
    new->move_type = tipo_mov;

    aplica(new->grid, grid, atual_player, tipo_mov, 1, r, c);

    if (atual_player == 'r')
    {
        new->role = 'g';
        if (tipo_mov == 's')
        {
            new->min = 1000;
        }
        else
        {
            new->min = distance_to_gooses(&(new->grid[i]), r[1], c[1]);
        }
    }
    else
    {
        new->role = 'r';
        new->max = goose_score(&(new->grid[i]), r, c);
    }

    return new;
}

void minmax(StateList *list, State *state, char max, int depth)
{
    //se chegou na profundidade 5, acabou
    if (depth == 2)
    {
        return;
    }

    int move_r[2];
    int move_c[2];

    int k;
    for (k = 0; k < MAXSTR; ++k)
    {
        if (state->grid[k] == '#')
        {
            break;
        }
    }

    //acha a posicao da raposa
    int *pos = find_r(&(state->grid[k]));
    int r = pos[0];
    int c = pos[1];

    // printf("init %d %d\n", r, c);

    //se o estado atual for raposa
    if (state->role == 'r')
    {

        move_r[0] = r;
        move_c[0] = c;
        // printf("raposa %d %d\n", r, c);

        //faz um estado novo para cada nova posicao e calcula o minmax
        //modificando o mapa atual com a nova posicao
        // -1 0 esquerda
        // 0 -1 cima
        // 0 1 baixo
        // 1 0 direita
        for (int p_x = -1; p_x < 2; p_x++)
        {
            for (int p_y = -1; p_y < 2; p_y++)
            {
                if (p_x * p_y == 0 && p_x != p_y)
                {
                    //verifica se a posicao q ta indo eh um ganso.
                    //se for, a raposa pula pra proxima posicao livre, comendo o ganso
                    char tipo_mov = 'm';
                    int x = p_x;
                    int y = p_y;
                    if (state->grid[k+POS(r+p_x, c+p_y)] == 'g')
                    {
                        // printf("achei um ganso na nova pos %d %d\n", r+p_x, c+p_y);
                        x *= 2;
                        y *= 2;

                        //se posicao nao tiver vazia, nao pula
                        if (state->grid[k+POS(r+x, c+y)] != '-')
                        {
                            continue;
                        }

                        tipo_mov = 's';
                    }
                    move_r[1] = r+x;
                    move_c[1] = c+y;
                    // printf("pos: %d %d\n", move_r[1], move_c[1]);
                    // printf("move: %c\n", tipo_mov);

                    //verifica se a nova posicao eh valida
                    if (pos_valida(move_r[1], move_c[1]))
                    {
                        // printf("mv: %c init: %d %d fim: %d %d\n", tipo_mov, move_r[0], move_c[0], move_r[1], move_c[1]);
                        State *new = create_new_state(state->grid, state->role, tipo_mov, move_r, move_c, max);

                        //quando sai da recursao, se for o joador MAX, atualiza o MAX com o maior valor calculado pelo MIN(mesma coisa com o MIN)
                        if (state->role == max)
                        {
                            // printf("%d %d\n", new->min, state->max);
                            if (new->min > state->max)
                            {
                                list->list[depth] = new;

                                state->max = new->min;
                            }
                            //chama recursivamente o minmax para esse proximo estado criado
                            minmax(list, new, max, depth+1);
                        }
                        else
                        {
                            // printf("point %d %d\n", new->max, state->min);
                            if (new->max < state->min)
                            {
                                list->list[depth] = new;

                                state->min = new->max;
                            }
                            //chama recursivamente o minmax para esse proximo estado criado
                            minmax(list, new, max, depth+1);
                        }

                    }
                }
            }
        }
    }
    //o movimento dos gansos.
    else
    {
        //pega a posicao de todos os gansos
        int *gooses_pos = gooses_to_move(&(state->grid[k]), r, c);

        //para cada ganso, realiza os 4 movimentos possiveis para ele
        for (int g = 0; g < 26; g+=2)
        {
            int r = gooses_pos[g];
            int c = gooses_pos[g+1];

            move_r[0] = r;
            move_c[0] = c;
            // printf("ganso %d %d\n", r, c);
            for (int p_x = -1; p_x < 2; p_x++)
            {
                for (int p_y = -1; p_y < 2; p_y++)
                {
                    if (p_x * p_y == 0 && p_x != p_y)
                    {
                        char tipo_mov = 'm';
                        move_r[1] = r+p_x;
                        move_c[1] = c+p_y;

                        // printf("pos %c\n", state->grid[k+POS(r+p_x, c+p_y)]);
                        if (state->grid[k+POS(r+p_x, c+p_y)] != '-')
                        {
                            continue;
                        }

                        if(pos_valida(move_r[1], move_c[1]))
                        {
                            // printf("pos: %d %d\n", move_r[1], move_c[1]);
                            State *new = create_new_state(state->grid, state->role, tipo_mov, move_r, move_c, max);
                            
                            //quando sai da recursao, se for o joador MAX, atualiza o MAX com o maior valor calculado pelo MIN(mesma coisa com o MIN)
                            if (state->role == max)
                            {
                                if (list->list[depth] && list->list[depth]->move_type == 's')
                                {
                                    
                                }
                                // printf("%d %d\n", new->min, state->max);
                                if (new->min > state->max)
                                {
                                    list->list[depth] = new;
                                    
                                    state->max = new->min;
                                }
                                //chama recursivamente o minmax para esse proximo estado criado
                                minmax(list, new, max, depth+1);
                            }
                            else
                            {
                                // printf("%d %d\n", new->min, state->max);
                                if (new->max < state->min)
                                {
                                    list->list[depth] = new;

                                    state->min = new->max;
                                }
                                //chama recursivamente o minmax para esse proximo estado criado
                                minmax(list, new, max, depth+1);
                            }
                        }
                    }
                }
            }
        }
    }
}

char *make_move(State *state)
{
    srand(time(NULL));   // Initialization, should only be called once.


    StateQueue *queue = (StateQueue *)malloc(sizeof(StateQueue));
    queue->top = NULL;
    queue->depth = 4;
    
    StateList *list = (StateList *)malloc(sizeof(StateList));
    list->size = 0;

    for (int i = 0; i < 3; i++)
    {
        list->list[i] = NULL;
    }

    minmax(list, state, state->role, 0);

    char *line = (char *)malloc(MAXSTR*sizeof(char));

    //se o movimento foi salto, faz o minmax de novo pra ver se pode saltar novamente
    if (list->list[0]->move_type == 's')
    {
        sprintf(line, "%c %c 2 %d %d %d %d", state->role, list->list[0]->move_type, \
            list->list[0]->init_pos[0], list->list[0]->init_pos[1], \
            list->list[0]->new_pos[0], list->list[0]->new_pos[1]);
    }
    else
    {
        sprintf(line, "%c %c %d %d %d %d", state->role, list->list[0]->move_type, \
            list->list[0]->init_pos[0], list->list[0]->init_pos[1], \
            list->list[0]->new_pos[0], list->list[0]->new_pos[1]);
    }
    
    printf("%s\n", line);

    return line;
}