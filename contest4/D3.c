#include <stdio.h>

enum {buf_size = 10000};

int main(void)
{
    static double mas1[buf_size], mas2[buf_size];
    double *dp1, *dp2; //вспомогательные указатели
    double *first_negative = NULL, *last_positive = NULL;
    dp1 = mas1;
    dp2 = mas2;
    double a;
    
    int amount1;
    int flag = 0;
    scanf("%d", &amount1);
    for (int k = 1; k <= amount1; ++k) {
        // считываем и сразу проверяем является ли элемент отрицательным
        scanf("%lf", &a);
        *dp1 = a;
        if ((*dp1 < 0) & (flag == 0)) {
            first_negative = dp1;
            ++flag;
        }
        ++dp1;
    }

    int amount2;
    scanf("%d", &amount2);
    for (int k = 1; k <= amount2; ++k) {
        // считываем и сразу проверяем является ли элемент положительным
        scanf("%lf", &a);
        *dp2 = a;
        if (*dp2 > 0) {
            last_positive = dp2;
        }
        ++dp2;
    }

    if ((first_negative != NULL) & (last_positive != NULL)) {
        // если такие элементы есть то меняем их местами
        a = *first_negative;
        *first_negative = *last_positive;
        *last_positive = a;  
    }

    dp1 = mas1;
    dp2 = mas2;
    for (int k = 1; k <= amount1; ++k) {
        printf("%.1lf ", *dp1);
        ++dp1;
    }
    printf("\n");
    for (int k = 1; k <= amount2; ++k) {
        printf("%.1lf ", *dp2);
        ++dp2;
    }
    printf("\n");
}