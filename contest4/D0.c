#include <stdio.h>

enum {buf_size = 82};

int
main(void)
{
    static char str[buf_size];
    if (fgets(str, sizeof str, stdin) == NULL) {
        printf("EMPTY INPUT\n");
    } else {
        printf("%s", str);
    }
}