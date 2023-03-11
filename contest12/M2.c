#include <signal.h>
#include <unistd.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <stdio.h>

void
sig_hndlr(int s)
{
    static int counter = 0;
    ++counter;
    char a = counter + '0';
    char enter = '\n';
    write(1, &a, 1);
    write(1, &enter, 1);
    if (counter == 5) {
        signal(SIGINT, SIG_DFL);
        exit(0);
    }
}

int
main(void)
{
    int pid;
    signal(SIGINT, sig_hndlr);
    if ((pid = fork()) == 0) {
        while(1) {
            pause();
        }
    }
    for (int i = 0; i < 5; ++i) {
        kill(pid, SIGINT);
        usleep(50);
    }
    wait(0);
    return 0;
}