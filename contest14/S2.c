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
    return 0;
}

int
run_redirect_command(Command *cmd)
{
    return 0;
}

int
run_seq_command(Command *cmd)
{
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
