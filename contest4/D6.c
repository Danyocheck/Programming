#include <stdio.h>
#include <ctype.h>
#include <string.h>

enum {size_buf = 82};

void numbers_forward(char *str)
{
    int length = strlen(str);
    char prev, *dp = str;
    int count = 0, c = 0;
    // пробегаем по массиву до конца, переставляем цифры вперед
    // пузырьком и считаем количество букв
    for (int i = 0; i < length; ++i) {
        if (isdigit(*str)) {
            while (str != dp) {
                prev = *(str - 1);
                *(str - 1) = *str;
                *str = prev;
                --str;
                ++c;
            }
            ++dp;
            str += c;
        } else {
            ++count;
        }
        c = 0;
        ++str;
    }
    --str;
    // ставим буквы в обратном порядке пузырьком
    for (int i = count; i > 0; --i) {
        if (!isdigit(*str)) {
            while (str != dp) {
                prev = *str;
                *str = *(str - 1);
                *(str - 1) = prev;
                --str;
                ++c;
            }
            ++dp;
            str += c;
        c = 0;
        }
    }
}

int
main(void)
{
    static char str[size_buf];
    char *dp;
    fgets(str, sizeof str, stdin);
    dp = str;
    while ((*dp) != '\n') {
        ++dp;
    } *dp = '\0';
    numbers_forward(str);
    printf("%s\n", str);
}