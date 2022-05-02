#include <stdio.h>

int main(int argc, char const *argv[])
{
    int x = 0;
    int y = -1;

    for (int s = 1; s < 8; ++s)
    {
        printf("%d\n", s);
        for (int i = 5-s; i <= 5+s; ++i)
        {
            for (int j = 4-s; j <= 4+s; ++j)
            {
                printf("(%d %d) ", i, j);
            }
            printf("\n");
        }
        printf("\n");
    }
}