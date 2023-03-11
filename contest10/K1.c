#include <fcntl.h>
#include <sys/wait.h>
#include <unistd.h>

int
main(int argc, char *argv[])
{
    char *p = argv[1];

    if (fork() == 0) {
        int fd = open(argv[2], O_RDONLY);
        dup2(fd, 0);
        close(fd);
        execlp(p, p, NULL);
    }
    wait(NULL);
    return 0;
}