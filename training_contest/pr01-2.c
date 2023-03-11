#include <stdio.h>

int
main(void)
{
    long long sum = 0;
    char symbol;
    while (scanf("%c", &symbol) == 1) {
        if (symbol >= '0' && symbol <= '9') {
            sum += symbol - '0';
        } else if (symbol >= 'a' && symbol <= 'f') {
            sum += symbol - 'a' + 10;
        } else if (symbol >= 'A' && symbol <= 'F') {
            sum += symbol - 'A' + 10;
        }
    }
    printf("%lld\n", sum);
}