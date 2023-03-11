#include <stdio.h>
#include <ctype.h>
#include <string.h>

enum {buf_size = 82};

char *find_left_number(char *str, int *result, int *length)
{
    int flag = 0;
    char *str_start = str;
    while (*str != '\0') {
        if (isdigit(*str)) {
            if ((*str == '0') & (flag == 0)) { // проверяем на 0
                *length = 1;
                *result = 1;
                return str;
            }
            // если цифра не 0, то передвигаем указатель
            // на следующий символ,
            // считаем количество подряд идущих цифр
            ++flag;
            ++str;
        } else if (flag != 0) {
            // если цифры закончились, то запоминаем количество
            // цифр, которое нам встретилось
            // возвращаем указатель на первую цифру в числе
            *length = flag;
            *result = 1;
            str -= flag;
            return str;
        } else {
            ++str; // если цифры еще есть идем дальше
        }
    }
    // если мы вышли из цикла проверяем
    // являются ли последние символы цифрами
    // если нет, то возвращаем указатель на начало строки
    if (flag == 0) {
        *length = 0;
        *result = 0;
        str = str_start;
        return str;
    } else {
        *length = flag;
        *result = 1;
        str -= flag;
        return str;
    }
}

int
main(void)
{
    static char str1[buf_size];
    static char max_number[buf_size];
    char *dp1, *dp2, *start_mas1;
    int result, length, flag1 = 0, flag2 = 0;
    fgets(str1, sizeof str1, stdin);
    dp1 = str1;
    while ((*dp1) != '\n') {
        ++dp1;
    } *dp1 = '\0';

    // указатель на массив, в котором хранится максимальное число
    dp2 = max_number;
    // находим первое число, и если оно есть заносим его в массив
    dp1 = find_left_number(str1, &result, &length);
    if (result != 0) {
        for (int i = 0; i < length; ++i) {
            *dp2 = *(dp1);
            ++dp2;
            ++dp1; 
        }
    }
    dp1 -= length;

    while (result) {
        dp2 = max_number;
        start_mas1 = dp1;
        // если это первый заход в цикл, то переходим
        //к поиску нового числа
        // если нет, сравниваем новое число с максимальным 
        if (flag1 != 0) {
            for (int i = 0; i < length; ++i) {
                // если их длины равны, сравниваем пока новое число больше
                if ((*dp2 <= *dp1) & (strlen(max_number) == length)) {
                    ++dp2;
                    ++dp1;
                // если длина нового числа больше, значит оно точно больше
                } else if (strlen(max_number) < length) {
                    break;
                // если меньше, значит меньше
                } else {
                    flag2 = 1;
                    break;
                }
            }
            // если новое число оказалось больше максимального, то
            // перезаписывем максимальное число
            if (flag2 == 0) {
                dp1 = start_mas1;
                dp2 = max_number;
                for (int i = 0; i < length; ++i) {
                    *dp2 = *(dp1);
                    ++dp2;
                    ++dp1; 
                }
            } else {
                dp1 = start_mas1 + length;
            }
        } else {
            dp1 += length;
        }
        ++flag1;
        flag2 = 0;
        dp1 = find_left_number(dp1, &result, &length);   
    }
    printf("%s\n", max_number);
}