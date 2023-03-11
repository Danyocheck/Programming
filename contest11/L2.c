#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <stdlib.h>

int
main(int argc, char *argv[])
{
    int fd[2];
    pipe(fd);
    if (fork() == 0) {
        dup2(fd[1], 1);
        close(fd[0]);
        close(fd[1]);
        int status;
        if (fork() == 0) {
            execlp(argv[1], argv[1], NULL);
        }
        wait(&status);
        if (WIFEXITED(status) && WEXITSTATUS(status) == 0) {
            exit(0);
        }

        if (fork() == 0) {
            execlp(argv[2], argv[2], NULL);
        }
        wait(NULL);
        exit(0);
    }

    if (fork() == 0) {
        dup2(fd[0], 0);
        close(fd[0]);
        close(fd[1]);
        execvp(argv[3], argv + 3);
    }

    close(fd[0]);
    close(fd[1]);
    while (wait(NULL) > 0) {}
    return 0;
}