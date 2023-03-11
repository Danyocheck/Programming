#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <stdio.h>

void
proc_A(int *fd1, int *fd2)
{
    close(fd1[1]);
    close(fd2[0]);
    int buf[2];
    while (read(fd1[0], buf, 2 * sizeof(int)) > 0) {
        int sum = buf[0] + buf[1];
        int sub = buf[0] - buf[1];
        buf[0] = sum;
        buf[1] = sub;
        write(fd2[1], buf, 2 * sizeof(int));
    }

    close(fd1[0]);
    close(fd2[1]);
    return;
}

void
proc_B(int number1, int number2, int *fd1, int *fd2, int *fd3)
{
    close(fd2[1]);
    close(fd1[0]);
    int buf[2] = {number1, number2};
    int run = 0;
    int condition;
    read(fd3[0], &condition, sizeof(int)); // wait while proc_A will be ready
    write(fd1[1], buf, 2 * sizeof(int)); // send numbers to proc_A
    printf("%d %d ", number1, number2); 
    read(fd2[0], buf, 2 * sizeof(int)); // read result from proc_A
    printf("%d %d\n", buf[0], buf[1]);
    write(fd3[1], &run, sizeof(int)); // notify that proc_A can continue

    close(fd1[1]);
    close(fd2[0]);
    close(fd3[0]);
    close(fd3[1]);
    return;
}

int
main(void)
{
    setbuf(stdin, 0);
    int number1, number2;
    int fd1[2];
    int fd2[2];
    int fd3[2];
    pipe(fd1);
    pipe(fd2);
    if (fork() == 0) {
        proc_A(fd1, fd2);
        exit(0);
    }
    pipe(fd3);
    int run = 0;
    pid_t pid;
    write(fd3[1], &run, sizeof(int));
    while (scanf("%d%d", &number1, &number2) == 2) {
        if ((pid = fork()) == 0) {
            proc_B(number1, number2, fd1, fd2, fd3);
            exit(0);
        } else if (pid == -1) {
            wait(NULL);
            if (fork() == 0) {
                proc_B(number1, number2, fd1, fd2, fd3);
                exit(0);
            }
        }
    }
    close(fd1[0]);
    close(fd1[1]);
    close(fd2[0]);
    close(fd2[1]);
    close(fd3[0]);
    close(fd3[1]);
    while (wait(NULL) > 0) {}
    return 0;
}