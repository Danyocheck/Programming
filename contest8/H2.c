#include <stdio.h>
#include <stdlib.h>
#include <string.h>

enum { BUF_SIZE = 257 };

int
sort(const void *a, const void *b)
{
    return strcmp(*(char **)a, *(char **)b);
}

void *
my_bsearch(
        void *key,
        void *base,
        size_t number,
        size_t size,
        int (*compar)(const void *, const void *))
{
    int left = 0;
    int right = number - 1;
    while (left <= right) {
        int mid = (left + right) / 2;
        if (compar(key, (char *)base + mid * size) == 0) {
            return (void *)((char *)base + mid * size);
        } else if (compar(key, (char *)base + mid * size) < 0) {
            right = mid - 1;
        } else {
            left = mid + 1;
        }
    }
    return NULL;
}

int
main(int args, char *argv[])
{
    if (args == 1) {
        printf("0\n");
        return 0;
    }
    ++argv;
    --args;
    static char key[BUF_SIZE];
    if (fgets(key, sizeof key, stdin) == NULL) {
        return 0;
    }
    char *dp = strchr(key, '\n');
    *dp = '\0';

    char *p_key = key;
    char **key_ptr = &p_key;
    qsort(argv, args, sizeof(char *), sort);
    char **res = my_bsearch(key_ptr, argv, args, sizeof(char *), sort);
    if (res != NULL) {
        printf("%ld\n", res - argv + 1);
    } else {
        printf("0\n");
    }
    return 0;
}