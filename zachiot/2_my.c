#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <ctype.h>
#include <unistd.h>
#include <string.h>

char **
transformator(char * com)
{
    int n = 16, argc = 0, skip = 0, len = strlen(com);
    char ** argv = malloc(n * sizeof(char *));
    for (int i = 0; i < len; ++i) {
        if ((com[i] != ' ') && (!skip)) {
            argv[argc++] = com + i;
            skip = 1;
        }
        if ((com[i] == ' ') && (skip == 1)) {
            com[i] = 0;
            skip = 0;
        }
        if (argc + 1 == n) {
            n *= 2;
            argv = realloc(argv, n * sizeof(char *) + 1);
        }
    }
    argv[argc] = NULL;
    return argv;
}

char *
reading(void)
{
    char c;
    if (scanf("%c", &c) != 1) {
        return NULL;
    }
    char *res;
    res = malloc(2 * sizeof*res);
    if (c == '\n') {
        *res = '\0';
        return res;
    }
    int size, offset, control_end;
    char *enter;
    *res = c;
    res++;
    *res = '\0';
    res--;
    do {
        size = strlen(res);
        res = realloc(res, (2 * size + 1) * sizeof*res);
        offset = strlen(res);
        if (fgets(&res[offset], size + 1, stdin) == NULL) {
            return res;
        }
        control_end = strlen(res);
        if ((enter = strchr(res, '\n')) != NULL) {
            *enter = '\0';
            return res;
        }
        if (control_end < size * 2) {
            return res;
        }
    } while (1);
}

int
main(void)
{
    char *s = NULL;
    pid_t pid = 0;
    int status = 0;
    while ((s = reading()) != NULL) {
        char ** argv;
        char * com = strtok(s, ";");
        argv = transformator(com);
        if ((pid = fork()) == 0) {
            execvp(argv[0], argv);
            exit(127);
        } else if (pid == -1) {
            exit(1);
        }
        wait(&status);
        free(argv);
        while ((com = strtok(NULL, ";")) != NULL) {
            argv = transformator(com);
            if ((pid = fork()) == 0) {
                execvp(argv[0], argv);
                exit(127);
            } else if (pid == -1) {
                exit(1);
            }
            wait(&status);
            free(argv);
        }
        free(s);
    }
    if (WIFEXITED(status)) {
        exit(WIFEXITED(status));
    }
    if (WIFSIGNALED(status)) {
        exit(128 + WTERMSIG(status));
    }
    return 0;
}
