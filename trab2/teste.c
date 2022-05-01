#include <stdio.h>

int main(int argc, char const *argv[])
{
    int x = 0;
    int y = -1;

    for (int i = -2; i < 3; i++)
        for (int j = -2; j < 3; j++)
            if (i * j == 0 && i != j)
                printf("%d %d\n", i, j);
}