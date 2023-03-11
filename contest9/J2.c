#include <unistd.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/wait.h>

int
main(int argc, char *argv[])
{
    if (fork() > 0) {
        int status = 0;
        wait(&status);
        if (WIFEXITED(status)) {
            printf("%d\n", WEXITSTATUS(status));
        } else if (WIFSIGNALED(status)) {
            printf("%d\n", WTERMSIG(status));
        }
    } else {
        execvp(argv[1], argv + 1);
    }

    return 0;
}