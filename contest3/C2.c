#include <stdio.h>
#include <stdarg.h>

enum Type {INT, DOUBLE};

void vmax (enum Type a, size_t b, ...);

int
main(void)
{
    vmax(INT, 4, 1, 2, 10, 5);

    vmax(INT, 1, 10);

    vmax(INT, 2, 10, -100);

    vmax(INT, 6, -25, 9, 8, 9, 10, 10);

    vmax(DOUBLE, 1, 100.);

    vmax(DOUBLE, 2, 45.56, 100.);

    vmax(DOUBLE, 3, -10.93, -90.78, 100.0);

    vmax(DOUBLE, 4, -123., 9.76, 99.99, 100.);
}

void vmax (enum Type a, size_t b, ...)
{
    va_list ap;
    if (a == INT) {
        int now, max;
        va_start(ap, b);
        max = va_arg(ap, int);
        for (int i = 1; i < b; i++) {
            if (max < (now = va_arg(ap, int))) {
                max = now;
            }
        } printf("%d\n", max);
    }
    else if (a == DOUBLE) {
        double now, max;
        va_start(ap, b);
        max = va_arg(ap, double);
        for (int i = 1; i < b; i++) {
            if (max < (now = va_arg(ap, double))) {
                max = now;
                
            }
        } printf("%g\n", max);
    }
    va_end(ap);
}