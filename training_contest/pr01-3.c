#include <stdio.h>

int
main(void)
{
    int a, b, n;
    long long number = 0;
    scanf("%d%d%d", &a, &b, &n);
    printf("%*c ", n, ' ');
    int i;
    for (i = a; i < b - 1; ++i) {
        printf("%*d ", n, i);
    }
    printf("%*d", n, i);
    printf("\n");
    int k = a;
    for (i = a; i < b; ++i) {
        printf("%*d ", n, k);
        int j = a;
        for (j = a; j < b - 1; ++j) {
            number = (long long)i * (long long)j;
            printf("%*lld ", n, number);
        }
        number = (long long)i * (long long)j;
        printf("%*lld", n, number);
        ++k;
        printf("\n");
    }
}