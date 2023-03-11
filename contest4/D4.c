#include <stdio.h>

enum {buf_size = 82};

int func(const char *str1, const char *str2)
{
    while ((*str1 != '\0') | (*str2 != '\0')) {
        if (*str1 == ' ') {
            ++str1;
            continue;
        } else if (*str2 == ' ') {
            ++str2;
            continue;
        } else if (*str1 != *str2) {
            return 0;
        }
        ++str1;
        ++str2;
    }
    return 1;
}

int
main(void)
{
    static char str1[buf_size];
    static char str2[buf_size];
    char *dp1, *dp2;
    fgets(str1, sizeof str1, stdin);
    fgets(str2, sizeof str2, stdin);
    dp1 = str1;
    dp2 = str2;
    while ((*dp1) != '\n') {
        ++dp1;
    } *dp1 = '\0';
    while ((*dp2) != '\n') {
        ++dp2;
    } *dp2 = '\0';
    if (func(str1, str2)) {
        printf("YES\n");
    } else {
        printf("NO\n");
    }
}