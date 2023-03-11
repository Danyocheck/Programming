#include <stdio.h>
#include <string.h>

enum {
    size_buf = 82,
    size_of_end = 4
};

const char *find_string(const char *str1, const char *str2)
{
    int symbols1 = strlen(str1), symbols2 = strlen(str2);
    int counter = 0;
    const char *dp = str2;
    if (symbols1 < symbols2) {
        return str1;
    }
    int symbols1_help = symbols1;
    int symbols2_help = symbols2;
    while (symbols1 > 0) {
        // если символы совпали уменьшаем счетчик и передвигаемся
        // на один элемент вперед во втором массиве
        if (*str1 == *str2) {
            --symbols2_help;
            ++str2;
            ++counter;
        // если символы не совпали, но данный символ является началом
        // второго массива, то передвигаем указатель str2 на второй
        // элемент второго массива, а указатель str1 на символ 
        // cледующий за первым совпавшим (это нужно чтобы работало для
        // таких строк в которых есть повторяющиеся буквы).
        // Например: "aab", "aaaaab"
        } else if (*str1 == *dp){
            symbols2_help = symbols2 - 1;
            str2 = dp + 1;
            str1 -= counter - 1;
            symbols1 += counter - 1;
            counter = 1;
        // если нет, то возвращаем второй указатель на начало
        // второго масства
        } else {
            symbols2_help = symbols2;
            str2 = dp;
        }
        // если счетчик обнулился, значит нашлась подстрока
        if (symbols2_help == 0) {
            ++str1;
            return str1;
        }
        ++str1;
        --symbols1;
    }
    if (symbols2_help == 0) {
        ++str1;
        return str1;
    } else {
        str1 -= symbols1_help;
        return str1;
    }
}

int
main(void)
{
    static char str1[size_buf];
    static char str2[size_of_end] = "end";
    char *dp1;
    const char *dp, *prev, *now;
    int end = 1;
    fgets(str1, sizeof str1, stdin);
    dp1 = str1;
    while ((*dp1) != '\n') {
        ++dp1;
    } *dp1 = '\0';

   
    now = str1;
    prev = str1;
    while (end) {
        dp = find_string(now, str2);
        if (dp == now) {
            end = 0;
            dp = prev;
        } else {
            prev = dp;
            now = dp;
        }
    }
    printf("%s\n", dp);
}