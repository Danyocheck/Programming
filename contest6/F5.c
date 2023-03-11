#include <stdio.h>
#include <string.h>
#include <stdlib.h>

enum
{
    BUF_SIZE = 82,
    CONST_TWO = 2
};

char *
read_str(void)
{
    char *str;
    int size = BUF_SIZE;
    str = (char *) malloc (size);
    if (fgets(str, size, stdin) == NULL) {
        free(str);
        return NULL;
    }
    while (strchr(str, '\n') == NULL) {
        str = (char *) realloc (str, size * CONST_TWO);
        if (fgets(str + size - 1, size + 1, stdin) == NULL) {
            return str;
        }
        size *= CONST_TWO;
    }
    return str;
}

int
main(void)
{
    int max_len = 0;
    char *str;
    char *max_str = NULL;
    while (1) {
        str = read_str();
        if (str == NULL) {
            break;
        }
        int len = strlen(str);
        if (len >= max_len) {
            free(max_str);
            max_str = str;
            max_len = len;
        } else {
            free(str);
        }
    }

    if (max_str != NULL) {
        printf("%s", max_str);
        free(max_str);
    }
}