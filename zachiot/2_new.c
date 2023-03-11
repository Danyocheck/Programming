#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <stdio.h>

enum { BUF_SIZE = 7 };

char *
add_symb_to_str(char *str, char symb, int *j, int *size_for_str)
{
    str[*j] = symb;
    ++(*j);
    if (*j >= *size_for_str) {
        *size_for_str *= 2;
        str = realloc(str, *size_for_str);
    }
    return str;
}

int
main(void)
{
    char *str = malloc(BUF_SIZE);
    str[0] = '\0';
    char **argv = malloc(2 * sizeof(char *));
    int size_for_str = BUF_SIZE;
    int size_for_argv = 2;
    int i = 0, j = 0;
    int if_space = 0;
    char symb;
    int exit_status = 0;
    int status;
    while (scanf("%c", &symb) == 1) {
        if (symb == ' ') {
            if_space = 1;
            continue;
        }
        if (if_space) {
            if_space = 0;
            if (strlen(str) == 0) {
                // str = add_symb_to_str(str, symb, &j, &size_for_str);
                str[j] = symb;
                ++j;
                if (j >= size_for_str) {
                    size_for_str *= 2;
                    str = realloc(str, size_for_str);
                }
                continue;
            }
            str[j] = '\0';
            j = 0;
            argv[i] = str;
            ++i;
            str = malloc(size_for_str);
            str[j] = '\0';
            if (i >= size_for_argv) {
                size_for_argv *= 2;
                argv = realloc(argv, size_for_argv * sizeof(char *));
            }
        }
        if (symb == '\n' || symb == ';') {
            if (strlen(str) != 0) {
                str[j] = '\0';
                j = 0;
                argv[i] = str;
                ++i;
                if (i >= size_for_argv) {
                    size_for_argv *= 2;
                    argv = realloc(argv, size_for_argv * sizeof(char *));
                }
            } else {
                free(str);
            }
            argv[i] = NULL;
            j = 0;
            for (int k = 0; k < i; ++k) {
                printf("%s\n", argv[k]);
            }
            if (fork() == 0) {
                execvp(argv[0], argv);
                _exit(127);
            }
            wait(&status);
            for (int k = 0; k < i; ++k) {
                free(argv[k]);
            }
            i = 0;
            str = malloc(size_for_str);
            str[j] = '\0';
            continue;
        }
        str[j] = symb;
        ++j;
        if (j >= size_for_str) {
            size_for_str *= 2;
            str = realloc(str, size_for_str);
        }
    }
    free(str);
    free(argv);
    if (WIFEXITED(status)) {
        exit(WEXITSTATUS(status));
    }
    if (WIFSIGNALED(status)) {
        exit(WTERMSIG(status) + 128);
    }
    return exit_status;
}