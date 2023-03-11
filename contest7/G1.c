#include <stdio.h>
#include <stdlib.h>

int
main(int argc, char *argv[])
{
    FILE *f = fopen(argv[1], "r");
    int i = 1;
    int symbol = fgetc(f);

    while (symbol != EOF) {
        if (i % 2 == 0) {
            printf("%c", symbol);
        }
        if (symbol == '\n') {
            ++i;
        }
        symbol = fgetc(f);
    }
    
    fclose(f);
    return 0;
}