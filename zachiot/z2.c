#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <stdio.h>
#include <string.h>

enum
{
    ARG_SIZE = 4,
    ARGV_SIZE = 4,
    STR_SIZE = 32
};

struct pair {
    int ret; // 0 - good, 1 - ';', '\n', 2 - EOF
    char *str;
};

char ***arg = (char ***) NULL;

void
free_memory(void)
{
    if (arg == (char ***) NULL) {
        return;
    }
    for (int i = 0; arg[i] != (char **) NULL; ++i) {
        for (int j = 0; arg[i][j] != (char *) NULL; ++j) {
            free(arg[i][j]);
        }
        free(arg[i]);
    }
    free(arg);
}

struct pair
get_string()
{
    char c;
    int i = 0;
    int str_size = STR_SIZE;
    struct pair res = {0, NULL};
    while ((c=getchar()) != EOF) {
        if ( c == ' ' ) {
            if (res.str != NULL) {
                // string is found
                break;
            }
            // continue;
        } else
        if ( c == ';' || c == '\n' ) {
            res.ret = 1;
            break;
        } else {
            // memorize char
            if (res.str == NULL) {
                // create new string if needed
                res.str = malloc(str_size * sizeof(char));
            }
            if (i + 1 >= str_size) {
                // give more memory
                str_size *= 2;
                res.str = realloc(res.str, str_size * sizeof(char));
            }
            res.str[i] = c;
            res.str[i+1] = '\0';
            ++i;
        }
    }
    if (c == EOF) {
        res.ret = 2;
    }
    return res;
}

int
main(void)
{
    // construct ***arg first, then start all programs
    char **argv = (char **) NULL;
    int j = 0;
    int argv_size = ARGV_SIZE;
    int k = 0;
    int arg_size = ARG_SIZE;

    struct pair res = get_string();
    while (1) {
        if (res.str != NULL) {
            // create argv if needed
            if (argv == (char **) NULL) {
                argv = malloc(argv_size * sizeof(char **));
            }
            // memorize string
            if (j + 1 >= argv_size) {
                argv_size *= 2;
                argv = realloc(argv, argv_size * sizeof(char *));
            }
            argv[j] = res.str;
            argv[j+1] = (char *) NULL;
            ++j;
        }
        if ((res.ret > 0) && (argv != (char **) NULL)) {
            // create arg if needed
            if (arg == (char ***) NULL) {
                arg = malloc(arg_size * sizeof(char **));
            }
            // memorize argv
            if (k + 1 >= arg_size) {
                arg_size *= 2;
                arg = realloc(arg, arg_size * sizeof(char **));
            }
            arg[k] = argv;
            arg[k+1] = (char **) NULL;
            ++k;
            // clear current argv
            argv = (char **) NULL;
            j = 0;
            argv_size = ARGV_SIZE;
        }
        if (res.ret == 2) {
            // EOF found
            break;
        }
        res = get_string();
    }

    atexit(free_memory);

    // start programs
    char ***p = arg;
    int pid, status;
    for (int f = 0; arg[f] != (char **) NULL; ++f) {
        if ((pid=fork()) == 0) {
            execvp(arg[f][0], arg[f]);
            exit(127);
        } else if (pid == -1) {
            exit(1);
        }
        wait(&status);
    }

    // return
    if (WIFEXITED(status)) {
        return WEXITSTATUS(status);
    }
    if (WIFSIGNALED(status)) {
        return 128 + WTERMSIG(status);
    }
    return 1;
}