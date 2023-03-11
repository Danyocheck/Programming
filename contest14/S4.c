#include <sys/types.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdio.h>

typedef struct Command {
    enum {
        KIND_SIMPLE,   // простоя команда
        KIND_SEQ,      // последовательность команд
        KIND_PIPELINE, // конвейер
        KIND_REDIRECT, // перенаправление
    } kind;
    union {
        struct { // для простых команд
            int argc;
            char **argv;
        };
        struct { // для последовательностей команд
            int seq_size;
            struct Command *seq_commands;
            enum {
                OP_SEQ,       // точка с запятой
                OP_CONJUNCT,  // конъюнкция
                OP_DISJUNCT,  // дизъюнкция
                OP_BACKGROUND // один амперсанд
            } seq;
            int *seq_operations;
        };
        struct { // для конвейеров
            int pipeline_size;
            struct Command *pipeline_commands;
        };
        struct { // для перенаправлений
            enum {
                RD_INPUT,  // перенаправление ввода
                RD_OUTPUT, // перенаправление вывода с переписыванием
                RD_APPEND, // перенаправление вывода с дописыванием
            } rd_mode;
            char *rd_path;
            struct Command *rd_command;
        };
    };
} Command;

int
run_command(Command *cmd);

int
run_simple_command(Command *cmd)
{
    int pid;
    if ((pid = fork()) == 0) {
        execvp(cmd->argv[0], cmd->argv);
        exit(1);
    }
    return pid;
}

int
run_redirect_command(Command *cmd)
{
    switch(cmd->rd_mode) {
        case RD_INPUT: {
            int fd = open(cmd->rd_path, O_RDONLY);
            dup2(fd, 0);
            close(fd);
            return run_command(cmd->rd_command);
        } case RD_OUTPUT: {
            int fd = open(cmd->rd_path, O_WRONLY | O_TRUNC | O_CREAT, 0666);
            dup2(fd, 1);
            close(fd);
            return run_command(cmd->rd_command);
        } case RD_APPEND:{
            int fd = open(cmd->rd_path, O_WRONLY | O_APPEND | O_CREAT, 0666);
            dup2(fd, 1);
            close(fd);
            return run_command(cmd->rd_command);
        } default:
            return 1;
    }
}

int
run_seq_command(Command *cmd)
{
    int status;
    int return_status = 0;
    for (int i = 0; i < cmd->seq_size; ++i) {
        switch (cmd->seq_operations[i]) {
            case OP_SEQ: {
                if (return_status == 0) {
                    run_command(&cmd->seq_commands[i]);
                    wait(&status);
                }
                return_status = 0;
                break;
            } case OP_CONJUNCT: {
                if (return_status == 0) {
                    run_command(&cmd->seq_commands[i]);
                    wait(&status);
                }
                if (WIFEXITED(status) && WEXITSTATUS(status) == 0) {
                    return_status = 0;
                } else {
                    return_status = 1;
                }
                break;
            } case OP_DISJUNCT: {
                if (return_status == 0) {
                    run_command(&cmd->seq_commands[i]);
                    wait(&status);
                }
                if (WIFEXITED(status) && WEXITSTATUS(status) == 0) {
                    return_status = 1;
                } else {
                    return_status = 0;
                }
                break;
            } case OP_BACKGROUND: {
                if (return_status == 0) {
                    int pid = run_command(&cmd->seq_commands[i]);
                    waitpid(pid, 0, WNOHANG);
                }
                return_status = 0;
                break;
            } default:
                break;
        }
    }
    while (wait(NULL) > 0) {}
    return 0;
}

int
run_pipeline_command(Command *Comm)
{    
    return 0;
}

int
run_command(Command *cmd)
{
    switch(cmd->kind) {
        case KIND_SIMPLE: {
            return run_simple_command(cmd);
        } case KIND_SEQ: {
            return run_seq_command(cmd);
        } case KIND_PIPELINE: {
            return run_pipeline_command(cmd);
        } case KIND_REDIRECT: {
            return run_redirect_command(cmd);
        } default:
            return 1;
    }
}

int
main(int argc, char *argv[])
{
    return 0;
}