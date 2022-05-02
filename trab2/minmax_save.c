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
                    aux_grid[POS(i, j)] = '!';

                    //salva info do ganso encontrada
                    pos[idx] = cont;
                    pos[idx+1] = s;

                    //se encontrou um ganso colado a raposa, verifica se a raposa consegue capturar 
                    //esse ganso
                    if (s == 0)
                    {
                        //se a linha do ganso for maior que a da raposa, o ganso ta embaixo da raposa
                        if (i > r)
                        {
                            //verifica se embaixo do ganso tem posicao livre
                            if(pos_valida(i+1, j) && aux_grid[POS(i+1, j)] == '-')
                            {
                                //se for posivel a captura, da um peso maior a essa posicao
                                pos[idx] += 10;
                            }
                        }
                        //se a linha do ganso for menor que a da raposa, o ganso ta em cima da raposa
                        else if (i < r)
                        {
                            //verifica se em cima do ganso tem posicao livre
                            if(pos_valida(i-1, j) && aux_grid[POS(i-1, j)] == '-')
                            {
                                //se for posivel a captura, da um peso maior a essa posicao
                                pos[idx] += 10;
                            }
                        }
                        //se a coluna do ganso for maior que a da raposa, o ganso ta a direita da raposa
                        else if (j > c)
                        {
                            //verifica se a direita do ganso tem posicao livre
                            if(pos_valida(i, j+1) && aux_grid[POS(i, j+1)] == '-')
                            {
                                //se for posivel a captura, da um peso maior a essa posicao
                                pos[idx] += 10;
                            }
                        }
                        //se a coluna do ganso for menor que a da raposa, o ganso ta a esquerda da raposa
                        else if (j < c)
                        {
                            //verifica se a esquerda do ganso tem posicao livre
                            if(pos_valida(i, j+1) && aux_grid[POS(i, j+1)] == '-')
                            {
                                //se for posivel a captura, da um peso maior a essa posicao
                                pos[idx] += 10;
                            }
                        }
                    }
                    idx++;
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

State *create_new_state(char grid[MAXSTR], char atual_player, char tipo_mov, int *r, int *c)
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
    new->prev = NULL;
    new->init_pos[0] = r[0];
    new->init_pos[1] = c[0];
    new->new_pos[0] = r[1];
    new->new_pos[1] = c[1];
    new->move_type = tipo_mov;

    aplica(new->grid, grid, atual_player, tipo_mov, 1, r, c);

    if (atual_player == 'r')
    {
        new->min = distance_to_gooses(&(new->grid[i]), r[1], c[1]);
    }
    else
    {
        new->min = goose_score(&(new->grid[i]), r, c) + rand() % 26;
    }

    return new;
}

char *minmax(State *state, char atual_player)
{
    int depth = 0;
    // int move_type = 'm';
    // int init_pos[2];
    char save_player = atual_player;
    char *result = (char *)malloc(MAXSTR*sizeof(char));
    result[0] = save_player;
    result[1] = '.';
    result[2] = '.';
    result[3] = '.';
    result[4] = '.';
    result[5] = '.';
    result[6] = '.';

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

        if (atual_player == 'r')
        {

            move_r[0] = r;
            move_c[0] = c;
            // printf("init %d %d\n", r, c);

            State *states[4];
            for (int s = 0; s < 4; ++s)
            {
                states[s] = NULL;
            }

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
                    if (p_x * p_y == 0 && p_x != p_y)
                    {
                        //verifica se a posicao q ta indo eh um ganso.
                        //se for, a raposa pula pra proxima posicao livre, comendo o ganso
                        char tipo_mov = 'm';
                        // printf("pos: %d %d\n", p_x, p_y);
                        int x = p_x;
                        int y = p_y;
                        if (current->grid[k+POS(r+p_x, c+p_y)] == 'g')
                        {
                            // printf("achei um ganso na nova pos\n");
                            x *= 2;
                            y *= 2;

                            //se posicao nao tiver vazia, nao pula
                            if (current->grid[k+POS(r+x, c+y)] != '-')
                            {
                                continue;
                            }

                            tipo_mov = 's';
                        }
                        move_r[1] = r+x;
                        move_c[1] = c+y;

                        if (pos_valida(move_r[1], move_c[1]))
                        {
                            // printf("pos: %d %d\n", move_r[1], move_c[1]);
                            states[s_pos++] = create_new_state(current->grid, atual_player, tipo_mov, move_r, move_c);
                        }
                    }
                }
            }

            int min = states[0]->min;
            int p = 0;
            // printf("mv: %c val: %d\n", states[0]->move_type, states[0]->min);
            //se for salto, prefere ele
            if (states[0]->move_type != 's')
            {
                for (int i = 1; i < 4; ++i)
                {
                    // printf("mv: %c val: %d\n", states[i]->move_type, states[i]->min);
                    if (states[i] && states[i]->min >= min)
                    {
                        min = states[i]->min;
                        p = i;
                        //se for salto, prefere ele
                        if (states[i]->move_type == 's')
                        {
                            break;
                        }
                    }
                }
            }

            //se for profundidade 0, eh o primeiro movimento. entao salva no result.
            if (depth == 0)
            {
                if (states[p]->move_type == 'm')
                {
                    result[1] = ' ';
                    result[2] = states[p]->move_type;
                    result[3] = ' ';
                    result[4] = r+'0';
                    result[5] = ' ';
                    result[6] = c+'0';
                }
                else
                {
                    result[1] = ' ';
                    result[2] = states[p]->move_type;
                    result[3] = ' ';
                    result[4] = '2';
                    result[5] = ' ';
                    result[6] = r+'0';
                    result[7] = ' ';
                    result[8] = c+'0';

                }
            }

            if (atual_player == save_player)
            {
                sprintf(result + strlen(result), " %d %d", states[p]->new_pos[0], states[p]->new_pos[1]);
            }

            push(queue, states[p]);
            
            atual_player = 'g';
        }
        //o movimento dos gansos.
        else
        {
            //verifica os gansos que estao proximos da raposa.
            int *gooses_pos = gooses_to_move(&(current->grid[k]), r, c);

            int goose_idx = 0;
            State *optimal_gooses[26];
            for (int s = 0; s < 26; ++s)
            {
                optimal_gooses[s] = NULL;
            }

            //para cada posicao dos gansos, realiza os 4 movimentos possiveis para ele
            for (int g = 0; g < 26; g+=2)
            {
                State *states[4];
                for (int s = 0; s < 4; ++s)
                {
                    states[s] = NULL;
                }
                int s_pos = 0;
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

                            // printf("pos %c\n", current->grid[k+POS(r+p_x, c+p_y)]);
                            if (current->grid[k+POS(r+p_x, c+p_y)] != '-')
                            {
                                continue;
                            }

                            if(pos_valida(move_r[1], move_c[1]))
                            {
                                states[s_pos++] = create_new_state(current->grid, atual_player, tipo_mov, move_r, move_c);
                            }
                        }
                    }
                }
                if (!states[0])
                {
                    continue;
                }

                int min = states[0]->min;
                int p = 0;
                for (int i = 1; i < 4; ++i)
                {
                    // printf("mv: %c val: %d\n", states[i]->move_type, states[i]->min);
                    if (states[i] && states[i]->min > min)
                    {
                        min = states[i]->min;
                        p = i;
                    }
                }

                if (min == AVANCA)
                {
                    while (1)
                    {
                        p = rand() % 4;
                        if (states[p])
                        {
                            break;
                        }
                    }
                }

                optimal_gooses[goose_idx] = (State *)malloc(sizeof(State));
                strcpy(optimal_gooses[goose_idx]->grid, states[p]->grid);
                optimal_gooses[goose_idx]->max = MAX;
                optimal_gooses[goose_idx]->prev = NULL;
                optimal_gooses[goose_idx]->init_pos[0] = states[p]->init_pos[0];
                optimal_gooses[goose_idx]->init_pos[1] = states[p]->init_pos[1];
                optimal_gooses[goose_idx]->new_pos[0] = states[p]->new_pos[0];
                optimal_gooses[goose_idx]->new_pos[1] = states[p]->new_pos[1];
                optimal_gooses[goose_idx]->move_type = states[p]->move_type;
                optimal_gooses[goose_idx]->min = states[p]->min;
                goose_idx++;
            }

            int min = optimal_gooses[0]->min;
            int p = 0;
            for (int i = 1; i < 26; ++i)
            {
                if (optimal_gooses[i] && optimal_gooses[i]->min > min)
                {
                    min = optimal_gooses[i]->min;
                    p = i;
                }
            }

            if (min == AVANCA)
            {
                while (1)
                {
                    p = rand() % 26;
                    if (optimal_gooses[p])
                    {
                        break;
                    }
                }
            }
            //se for profundidade 0, eh o primeiro movimento. entao salva no result.
            if (depth == 0)
            {
                result[1] = ' ';
                result[2] = optimal_gooses[p]->move_type;
                result[3] = ' ';
                result[4] = optimal_gooses[p]->init_pos[0]+'0';
                result[5] = ' ';
                result[6] = optimal_gooses[p]->init_pos[1]+'0';
            }

            if (atual_player == save_player)
            {
                sprintf(result + strlen(result), " %d %d", optimal_gooses[p]->new_pos[0], optimal_gooses[p]->new_pos[1]);
            }

            push(queue, optimal_gooses[p]);

            atual_player = 'r';
        }

        depth++;

        free(current);   
    }
    
    sprintf(result + strlen(result), "\n");

    printf("result: %s", result);
    return result;
}

char *make_move(State *state)
{

    srand(time(NULL));   // Initialization, should only be called once.

    char *line = (char *)malloc(MAXSTR*sizeof(char));
    line = minmax(state, state->role);

    return line;
}