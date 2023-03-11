#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <stdlib.h>

int
main(void)
{
    setbuf(stdin, 0);
    
    char first_symbol;
    scanf("%c", &first_symbol);
    long pid;
    char next_symbol;
    while (scanf("%c", &next_symbol) == 1) {
        if ((pid = fork()) == 0) {
            if (first_symbol == next_symbol) {
                printf("%c%c", next_symbol, next_symbol);
            }
            exit(0);
        }
        if (pid == -1) {
            wait(0);
            if ((pid = fork()) == 0) {
                if (first_symbol == next_symbol) {
                    printf("%c%c", next_symbol, next_symbol);
                }
                exit(0);
            }
            waitpid(-1, NULL, WNOHANG);
        } else {
            waitpid(-1, NULL, WNOHANG);
        }
    }
    while (wait(NULL) > 0) {}
    printf("\n");
    
    return 0;
}