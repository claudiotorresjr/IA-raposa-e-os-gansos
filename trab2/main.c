#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <readline/readline.h>
#include <readline/history.h>

#include "tabuleiro.h"
#include "shared_funcs.h"

int main(int argc, char **argv)
{
    char buf[MAXSTR];
    char grid[MAXSTR]; 
    char *linha;

    tabuleiro_conecta(argc, argv);

    while (1)
    {
        tabuleiro_recebe(buf);
        printf("%s", buf);

        Move move;
        get_enemy_move(buf, grid, &move);

        // show_received_moves(&move);
        // printf("%s", grid);

        linha = readline(NULL);
        if (linha[0] == '0')
        {
            break;
        }
        sprintf(buf, "%s\n", linha);
        free(linha);
        tabuleiro_envia(buf);
    }

    return 0;
}
