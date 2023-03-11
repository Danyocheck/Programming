#include <unistd.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/wait.h>

int
main(int argc, char *argv[])
{
    char *sec_prog = argv[argc - 1];
    argv[argc - 1] = NULL;
    if (fork() > 0) {
        int status = 0;
        wait(&status);
        if (WIFEXITED(status) && WEXITSTATUS(status) == 0) {
            if (fork() == 0) {
                execlp(sec_prog, sec_prog, NULL);
            }
        }
    } else {
        execvp(argv[1], argv + 1);
    }

    return 0;
}