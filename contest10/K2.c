#include <fcntl.h>
#include <sys/wait.h>
#include <unistd.h>

int
main(int argc, char *argv[])
{
    char *p = argv[1];

    if (fork() == 0) {
        int fd = open(argv[2], O_WRONLY | O_CREAT | O_TRUNC, 0666);
        dup2(fd, 1);
        execlp(p, p, NULL);
    }
    wait(NULL);
    return 0;
}