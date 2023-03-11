#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int
sort1(const void *a, const void *b)
{
    return strcmp(*(char **)b, *(char **)a);
}

int
sort2(const void *a, const void *b)
{
    int len1= strlen(*(char **)a);
    int len2= strlen(*(char **)b);

    if (len1 % 2 == 0 && len2 % 2 == 0) {
        return sort1(b, a);
    } else if (len1 != 0 && len2 % 2 != 0) {
        return sort1(a, b);
    } else if (len1 == 0 && len2 % 2 != 0) {
        return -1;
    } else {
        return 1;
    }
}

void
print_strings(char *mas_of_strings[], int number_of_elements)
{
    for (int i = 0; i < number_of_elements; ++i) {
        printf("%s\n", mas_of_strings[i]);
    }
}

int
main(int args, char *argv[])
{
    ++argv;
    --args;
    qsort(argv, args, sizeof(char *), sort1);
    print_strings(argv, args);
    qsort(argv, args, sizeof(char *), sort2);
    print_strings(argv, args);
}