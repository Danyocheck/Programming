#include <stdio.h>
#include <string.h>

enum {
    buf_size_1 = 161,
    buf_size_2 = 82
};

void reading(char *str)
{
    fgets(str, sizeof str, stdin);
    char *dp;
    dp = str;
    while ((*dp) != '\n') {
        ++dp;
    } *dp = '\0';
}

char *concatination(char *str1,const char *str2)
{
    while ((*str1) != '\0') {
        ++str1;
    } while ((*str2) != '\0') {
        *str1 = *str2;
        ++str2;
        ++str1;
    } return str1;
}

int main(void)
{
    static char str1[buf_size_1];
    static char str2[buf_size_2];
    reading(str1);
    printf("%s\n", str1);
    reading(str2);
    concatination(str1, str2);
    printf("%s\n", str1);
}