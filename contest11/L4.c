#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <stdlib.h>

int
main(void)
{
    int fd[2];
    pipe(fd);
    if (fork() == 0) {
        close(fd[1]);
        if (fork() == 0) {
            int i = 1;
            char byte;
            while (read(fd[0], &byte, 1) == 1) {
                if (i % 2 == 0) {
                    write(1, &byte, 1);
                }
                ++i;
            }
            close(fd[0]);
            exit(0);
        }
        close(fd[0]);
        wait(NULL);
        exit(0);
    }

    if (fork() == 0) {
        int i = 1;
        char byte;
        close(fd[0]);
        while (read(0, &byte, 1) == 1) {
            if (i % 2 == 0) {
                write(fd[1], &byte, 1);
            }
            ++i;
        }
        close(fd[1]);
        exit(0);
    }

    close(fd[0]);
    close(fd[1]);
    while (wait(NULL) > 0) {}
    return 0;
}