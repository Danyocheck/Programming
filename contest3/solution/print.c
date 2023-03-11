#include <stdio.h>
#include <stdarg.h>
#include "print.h"

static int t = 0;
static char buf[BUF_SIZE];

static void output(const char buf[])
{
    printf("%s", buf);
    fflush(stdout);
}

int print(const char format[], ...)
{
    va_list(ap);
    va_start(ap, format);
    int i = 0;
    while (format[i] != 0) {
        if (t >= BUF_SIZE - 1) {
            buf[t] = '\0';
            output(buf);
            t = 0;
        } if (format[i] == '%') {
            buf[t] = ((char) va_arg(ap, int));
        } else {
            buf[t] = format[i];
        } if (buf[t] == '\n') {
            buf[t + 1] = '\0';
            output(buf);
            t = -1;
        }
        ++i;
        ++t;
    }
    return 0;
}

void flush(void)
{
    output(buf);
    t = 0;
}