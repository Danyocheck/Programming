#include <stdio.h>
#include <stdlib.h>

int
main(void)
{
    int *pointer;
    int size = 1;
    int i = 0;
    pointer = malloc(sizeof(int));
    while (scanf("%d", (pointer + i)) == 1) {
        ++i;
        if (i >= size) {
            size *= 2;
            pointer = realloc(pointer, size * sizeof(int));
        }
    }

    for (int j = i - 1; j >= 0; --j) {
        printf("%d ", *(pointer + j));
    }
    free(pointer);
}