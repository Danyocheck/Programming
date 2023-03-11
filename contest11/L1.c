#include <unistd.h>
#include <sys/types.h>
#include <string.h>
#include <sys/wait.h>

int
main(int argc, char *argv[])
{
    ++argv;
    --argc;
    int i = 0;
    for (; i < argc - 1; ++i) {
        if (strcmp(argv[i], "--") == 0) {
            argv[i] = NULL;
            break;
        }
    }

    int fd[2];
    pipe(fd);
    if (fork() == 0) {
        dup2(fd[1], 1);
        close(fd[0]);
        close(fd[1]);
        execvp(argv[0], argv);
    }

    if (fork() == 0) {
        dup2(fd[0], 0);
        close(fd[0]);
        close(fd[1]);
        execvp(argv[i + 1], argv + i + 1);
    }

    close(fd[0]);
    close(fd[1]);
    while (wait(NULL) > 0) {}
    return 0;
}