#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <stdlib.h>

void
print_number(int number)
{
    FILE *f;
    if (number == 1) {
        while ((f = fopen("file.txt", "r")) == NULL) {
            usleep(100);
        }
        printf("%d\n", number);
        fclose(f);
        remove("file.txt");
    } else if (number == 2) {
        printf("%d\n", number);
        f = fopen("file.txt", "w");
    }
    exit(0);
}

int
main(void)
{
    if (fork() == 0) {
        print_number(1);
    }
    if (fork() == 0) {
        print_number(2);
    }
    while (wait(NULL) > 0) {}
}