#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

int
main(int argc, char *argv[])
{
    int fd1[2];

    pipe(fd1);
    if (fork() == 0) {
        dup2(fd1[1], 1);
        close(fd1[0]);
        close(fd1[1]);
        execlp("grep", "grep", "^[^ ]*(", NULL);
        _exit(1);
    }

    int fd2[2];
    pipe(fd2);
    if (fork() == 0) {
        dup2(fd1[0], 0);
        dup2(fd2[1], 1);
        close(fd1[0]);
        close(fd1[1]);
        close(fd2[0]);
        close(fd2[1]);
        execlp("cut", "cut", "-d", "(", "-f1", NULL);
        _exit(1);
    }

    close(fd1[0]);
    close(fd1[1]);
    if (fork() == 0) {
        dup2(fd2[0], 0);
        close(fd2[0]);
        close(fd2[1]);
        execlp("sort", "sort", NULL);
        _exit(1);
    }
    close(fd2[0]);
    close(fd2[1]);
    while (wait(NULL) > 0) {}
    return 0;
}