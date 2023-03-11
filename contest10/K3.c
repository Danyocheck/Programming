#include <fcntl.h>
#include <sys/wait.h>
#include <unistd.h>
#include <stdlib.h>

int
main(int argc, char *argv[])
{
    char *p1 = argv[1];
    char *p2 = argv[2];
    char *p3 = argv[4];
    int status, main_status;

    int fd = open(argv[3], O_APPEND | O_WRONLY | O_CREAT, 0666);

    if (fork() == 0) {
        dup2(fd, 1);
        close(fd);
        if (fork() == 0) {
            execlp(p1, p1, NULL); // run first program
            exit(1);
        }
        wait(&status);
        if (WIFEXITED(status) && WEXITSTATUS(status) == 0) { // if first program ended successfully
            exit(0);
        } else {
            if (fork() == 0) { // if first program ended unsuccessfully run second program
                execlp(p2, p2, NULL);
                exit(1);
            }
            wait(&status);
            if (WIFEXITED(status) == 0 || WEXITSTATUS(status)) {
                exit(1); // if second program ended unsuccessfully
            } else {
                exit(0);
            }
        }
    }
    wait(&main_status);
    if (WIFEXITED(main_status) && WEXITSTATUS(main_status) == 0) { // if first or second program ended successfully
        if (fork() == 0) {
            execlp(p3, p3, NULL);
            exit(0);
        }
    }
    
    while (wait(NULL) > 0) {}
    return 0;
}