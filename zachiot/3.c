#include <stdio.h>
#include <unistd.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <string.h>

int conv = 1;
int finish = 0;
int start_func = 0;
int count_conv = 0;

void
hndlr1(int s)
{
    static int count = 0;
    ++count;
    if (count % 3 == 0) {
        ++count_conv;
    }
}

void
hndlr2(int s)
{
    finish = 1;
}

void
run_conv(char **argv, int i)
{
    int fd[2];
    pipe(fd);
    if (fork() == 0) {
        dup2(fd[1], 1);
        close(fd[1]);
        close(fd[0]);
        execvp(argv[0], argv);
        _exit(1);
    }

    close(fd[1]);
    if (fork() == 0) {
        dup2(fd[0], 0);
        close(fd[0]);
        execvp(argv[i], argv + i);
        _exit(1);
    }
    while (wait(NULL) > 0);
}

int
main(int argc, char *argv[])
{
    signal(SIGUSR1, hndlr1);
    signal(SIGTERM, hndlr2);
    ++argv;
    --argc;
    int i = 0;
    while (strcmp(argv[i], "--") != 0) {
        ++i;
    }
    argv[i] = NULL;
    ++i;
    int pid;
    if ((pid = fork()) == 0) {
        while (1) {
            if (count_conv) {
                run_conv(argv, i);
                --count_conv;
            } else {
                if (finish) {
                    break;
                }
                usleep(100);
            }
        }
        exit(0);
    }
    int num;
    while (scanf("%d", &num) == 1) {
        if (num == 0) {
            kill(pid,  SIGUSR1);
        } else if (num > 0) {
            usleep(num);
        }
    }
    kill(pid, SIGTERM);
    wait(NULL);
    return 0;
}