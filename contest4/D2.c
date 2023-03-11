#include <stdio.h>

enum {buf_size = 82};

int find(const char *str1, const char *str2)
{
    int symbols1 = 0, symbols2 = 0;
    while ((*str1) != '\0') {
        ++str1;
        ++symbols1;
    } while ((*str2) != '\0') {
        ++str2;
        ++symbols2;
    } if (symbols1 > symbols2) {
        return 0;
    } --str1;
    --str2;
    while (symbols1 > 0) {
        if (*str1 != *str2) {
            return 0;
        } --str1;
        --str2;
        --symbols1;
    } return 1;
}

int main(void)
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
    if (find(str1, str2)) {
        printf("YES\n");
    } else {
        printf("NO\n");
    }
}