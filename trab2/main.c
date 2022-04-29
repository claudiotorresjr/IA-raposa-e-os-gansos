#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <readline/readline.h>
#include <readline/history.h>

#include "tabuleiro.h"
#include "shared_funcs.h"

char *make_move(char grid[MAXSTR], Move *move)
{
    char *line = (char *)malloc(MAXSTR*sizeof(char));
    line = "r m 5 4 5 3\n";

    return line;
}

int main(int argc, char **argv)
{
    char buf[MAXSTR];
    char grid[MAXSTR]; 
    char *line;

    tabuleiro_conecta(argc, argv);

    while (1)
    {
        tabuleiro_recebe(buf);
        printf("%s", buf);

        Move move;
        get_enemy_move(buf, grid, &move);

        // show_received_moves(&move);
        // printf("%s", grid);

        // line = readline(NULL);

        if (strcmp(buf, grid) == 0)
        {
            continue;
        }

        Move move2;
        line = make_move(grid, &move2);

        if (line[0] == '0')
        {
            break;
        }

        sprintf(buf, "%s\n", line);

        tabuleiro_envia(buf);
    }

    return 0;
}
