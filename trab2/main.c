#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <readline/readline.h>
#include <readline/history.h>

#include "stack.h"
#include "minmax.h"
#include "tabuleiro.h"
#include "shared_funcs.h"

int main(int argc, char **argv)
{
    char buf[MAXSTR];
    char grid[MAXSTR];

    tabuleiro_conecta(argc, argv);

    while (1)
    {
        tabuleiro_recebe(buf);
        printf("%s", buf);

        State *state = (State *)malloc(sizeof(State));
        strncpy(state->grid, buf, MAXSTR);
        state->min = MIN;
        state->max = MAX;
        state->prev = NULL;
        Move move;

        get_enemy_move(buf, &move);
        // show_received_moves(&move);

        char *line = make_move(state, buf[0]);

        if (line[0] == '0')
        {
            break;
        }

        sprintf(buf, "%s\n", line);

        tabuleiro_envia(buf);

        free(line);
    }

    return 0;
}
