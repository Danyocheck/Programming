#include <unistd.h>
#include <signal.h>
#include <stdio.h>

unsigned long long num;

void
sig_hndlr(int s)
{
    int seconds = alarm(0);
    char print_num[3];
    print_num[0] = seconds + '0';
    print_num[1] = '\n';
    write(1, print_num, sizeof(print_num));
    alarm(seconds);
}

void
alrm(int s)
{
    // так как сигнал посылается каждые 5 секунд, и таймер
    // устанавливается после вывода, то новый сигнал до
    // вывода не придет
    printf("%llu\n", num);
    alarm(5);
}

int
main(void)
{
    scanf("%llu", &num);
    signal(SIGALRM, alrm);
    signal(SIGINT, sig_hndlr);
    alarm(5);
    while (num != 0) {
        --num;
    }
    return 0;
}