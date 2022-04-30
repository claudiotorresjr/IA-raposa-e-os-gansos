#include <stdio.h>

int main(int argc, char const *argv[])
{
    int x = 0;
    int y = -1;

    for (int i = -1; i < 2; i++)
        for (int j = -1; j < 2; j++)
            if (i * j == 0 && i != j)
                printf("%d %d\n", i, j);
}