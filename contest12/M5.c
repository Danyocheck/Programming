#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <signal.h>
#include <stdio.h>

enum { BUF_SIZE = 1000 };

static int mas_pid[BUF_SIZE];
int son_need_wait = 1;
int send_signal = 1;
int cur_son = 0;
int father_change_flg = 1;

void
hndlr1(int s)
{
    son_need_wait = 0;
}

void
hndlr2(int s)
{
    send_signal = 1;
}

void
hndlr3(int s)
{
    father_change_flg = 0;
}

void
proc(int fd)
{
    // printf("wait = %d\n", son_need_wait);
    int number;
    while (son_need_wait) {
        //printf("hi\n");
        usleep(1);
    }
    lseek(fd, 0, SEEK_SET);
    read(fd, &number, sizeof(int));
    ++number;
    //printf("number = %d\n", number);
    lseek(fd, 0, SEEK_SET);
    write(fd, &number, sizeof(int));

    close(fd);
    // while (father_change_flg) {
    //     //printf("hi\n");
    //     usleep(1);
    // }
    kill(getppid(), SIGUSR2);
    // printf("son = %d\n", cur_son);
    return;
}

int
main(void)
{
    int number = 0;
    int pid, i = 0;
    char file_name[] = "helpXXXXXX";
    int fd = mkstemp(file_name);
    unlink(file_name);

    write(fd, &number, sizeof(int));
    scanf("%d", &number);

    signal(SIGUSR1, hndlr1);
    signal(SIGUSR2, hndlr2);
    signal(SIGINT, hndlr3);
    while (1) {
    //for (int i = 0; i < number; ++i) {
        if (i < number) {
            if ((pid = fork()) == 0) {
                proc(fd);
                exit(0);
            } else if (pid == -1) {
                wait(NULL);
                if ((pid = fork()) == 0) {
                    proc(fd);
                    exit(0);
                }
            }
            mas_pid[i] = pid;
            ++i;
            //printf("%d\n", i);
        }
        // printf("send = %d\n", send_signal);
        // printf("cur %d\n",  cur_son);
        //usleep(100);
        if (send_signal) {
            send_signal = 0;
            kill(mas_pid[cur_son], SIGUSR1);
            //printf("send %d\n",  cur_son);
            //kill(mas_pid[cur_son], SIGINT);
            ++cur_son;
        }
        if (cur_son == number) {
            break;
        }
        waitpid(-1, NULL, WNOHANG);
    }

    // printf("finish");
    while(wait(NULL) > 0) {}
    lseek(fd, 0, SEEK_SET);
    read(fd, &number, sizeof(int));
    printf("%d\n", number);
    close(fd);

    return 0;
}