#include <unistd.h>
#include <signal.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

// Усложнения программы из-за сигналов:
// 1) Нужно следить за тем, чтобы обработчики сигналов были
// атомарными
// 2) Нужно слдеить за неатомарными операциями (например в 
// этой программе
// функция вычитания неатомарнаяя и если приходит сигнал, и
// функция выполнилась не до конца число во время обработки
// будет неправильное, поэтому нужно печатать число, которое
// поступило в функция)

char *number, *prev_number;
int ready_to_print = 1;

void
sig_hndlr(int s)
{
    if (*number != '0') {
        int seconds = alarm(0);
        char print_num[3];
        print_num[0] = seconds + '0';
        print_num[1] = '\n';
        write(1, print_num, sizeof(print_num));
        alarm(seconds);
    } else {
        signal(SIGINT, SIG_DFL);
    }
}

void
alrm(int s)
{   
    if (ready_to_print) {
        //write(1, number, sizeof(number));
        printf("%s\n", number);
    } else {
        //write(1, prev_number, sizeof(prev_number));
        printf("%s\n", prev_number);
    }
    // printf("%s\n", number);
    alarm(5);
}

char *
sub(char *num)
{
    ready_to_print = 0;
    char *last_symb = strchr(num, '\0') - 1;
    int len = strlen(num);
    while (len > 0) {
        if (*last_symb == '0') {
            *last_symb = '9';
            --last_symb;
            --len;
        } else {
            --*last_symb;
            break;
        }
    }
    if (*num == '0' && strlen(num) != 1) {
        ++num;
    }
    ready_to_print = 1;
    return num;
}

int
main(int argc, char *argv[])
{
    number = argv[1];
    prev_number = malloc(sizeof(argv[1]));
    strcpy(prev_number, number);
    signal(SIGALRM, alrm);
    signal(SIGINT, sig_hndlr);
    alarm(5);

    while (*number != '0') {
        number = sub(number);
        strcpy(prev_number, number);
    }
    free(prev_number);
    return 0;
}