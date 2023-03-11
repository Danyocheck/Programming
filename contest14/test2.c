#include <stdio.h>

int
main(int argc, char *argv[])
{
    char c;
    while (scanf("%c", &c) == 1) {
        printf("%c", c);
    }
    printf("%s\n", argv[0]);
    return 0;
}