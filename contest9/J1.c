#include <stdio.h>

int
main(void)
{
    int n;
    scanf("%d", &n);

    if (n == 0) {
        printf("A\nB\nB\nC\nC\n");
        printf("1\n2\n2\n");
    } else if (n == 1) {
        printf("A\nB\nC\nB\nC\n");
        printf("2\n1\n2\n");
    } else if (n == 2){
        printf("UNKNOWN\n");
        printf("2\n2\n1\n");
    } else {
        printf("UNKNOWN\n");
        printf("UNKNOWN\n");
    }
}