#include <fcntl.h>
#include <sys/wait.h>
#include <unistd.h>
#include <stdlib.h>

enum { FILE_SIZE = 32 };

void
write_byte(char *symbol, int *write_bytes, int fd_writer, int fd_reader)
{
    // we stop the writer if he has written 31 characters more than the reader has read
    while (lseek(fd_writer, 0, SEEK_CUR) == lseek(fd_reader, 0, SEEK_CUR) - 1 ||
        lseek(fd_reader, 0, SEEK_CUR) == lseek(fd_writer, 0, SEEK_CUR) - 31) {
        usleep(100);
    }
    write(fd_writer, symbol, 1);
    if (*write_bytes == FILE_SIZE) {
        lseek(fd_writer, 0, SEEK_SET);
        *write_bytes = 0;
    }
}

void
read_byte(char *symbol, int *read_bytes, int fd_reader)
{
    read(fd_reader, symbol, 1);
    write(STDOUT_FILENO, symbol, 1);
    if (*read_bytes == FILE_SIZE) {
        lseek(fd_reader, 0, SEEK_SET);
        *read_bytes = 0;
    }
}

int
main(void)
{
    char template[] = "helpXXXXXX";
    int fd_writer = mkstemp(template);
    int fd_reader = open(template, O_RDONLY);

    unlink(template);

    if (fork() == 0) { // writer
        int write_bytes = 0;
        char symbol1;
        while (read(STDIN_FILENO, &symbol1, 1) == 1) {
            ++write_bytes;
            write_byte(&symbol1, &write_bytes, fd_writer, fd_reader);
        }
        close(fd_writer);
        close(fd_reader);
        _exit(0);
    }
    int read_bytes = 0;
    char symbol2;
    while (1) { // reader
        if (lseek(fd_writer, 0, SEEK_CUR) != lseek(fd_reader, 0, SEEK_CUR)) {
            ++read_bytes;
            read_byte(&symbol2, &read_bytes, fd_reader);
        } else if (waitpid(-1, NULL, WNOHANG) < 0) {
            break;
        } else {
            usleep(100); // if the writer has not written anything, we wait
        }
    }
    close(fd_writer);
    close(fd_reader);
    return 0;
}