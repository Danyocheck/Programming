#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>

enum { BUF_SIZE = 80 };

int
main(int argc, char *argv[])
{
    char buf[BUF_SIZE];
    char template[] = "helpXXXXXX";
    int help_file = mkstemp(template);
    unlink(template);
    int fd = open(argv[1], O_RDONLY);
    int check_sec_enter = 1;
    char *start_of_prev_str = buf;
    char *start_of_new_str;
    int size;

    while ((size = read(fd, buf, sizeof buf - 1)) > 0) {
        buf[size] = '\0';
        while ((start_of_new_str = strchr(start_of_prev_str, '\n')) > 0) { // while there is a newline
            int count = (start_of_new_str - start_of_prev_str + 1); // count of bytes to write
            size -= count; // size of the rest of the string
            if (check_sec_enter != 2) { // if this is not the second newline
                write(help_file, start_of_prev_str, count);
            }
            ++check_sec_enter;
            start_of_prev_str = start_of_new_str + 1; // remember start of the next string
        }
        if (size != 0 && check_sec_enter != 2) { // if there is a rest of the string and it is not the second newline
            write(help_file, start_of_prev_str, size);
        }
        start_of_prev_str = buf;
    }
    close(fd);
    lseek(help_file, 0, SEEK_SET);
    fd = open(argv[1], O_WRONLY | O_TRUNC);
    while ((size = read(help_file, buf, sizeof buf)) > 0) {
        write(fd, buf, size);
    }
    close(help_file);
    close(fd);
    return 0;
}