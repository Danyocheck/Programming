#include <stdio.h>
#include <stdlib.h>
#include "command.h"

int
init_empty_command(Command *c)
{
    c->kind = KIND_SIMPLE;
    c->argc = 0;
    c->argv = NULL;
    return 0;
}

int
init_sequence_command(Command *c)
{
    c->kind = KIND_SEQ;
    c->seq_size = 0;
    c->seq_commands = NULL;
    c->seq_operations = NULL;
    return 0;
}

int
append_command_to_sequence(Command *c, Command *cmd)
{
    ++c->seq_size;
    if (c->seq_commands != NULL) {
        c->seq_commands = realloc(c->seq_commands, c->seq_size * sizeof(Command));
    } else {
        c->seq_commands = malloc(c->seq_size * sizeof(Command));
    }
    c->seq_commands[c->seq_size - 1] = *cmd;
    return 0;
}

int
append_operation_to_sequence(Command *c, int op)
{
    if (c->seq_operations != NULL) {
        c->seq_operations = realloc(c->seq_operations, c->seq_size * sizeof op);
    } else {
        c->seq_operations = malloc(c->seq_size * sizeof(Command));
    }
    c->seq_operations[c->seq_size - 1] = op;
    return 0;
}

int
init_pipeline_command(Command *c)
{
    c->kind = KIND_PIPELINE;
    c->pipeline_size = 0;
    c->pipeline_commands = NULL;
    return 0;
}

int
append_to_pipeline(Command *c, Command *cmd)
{
    ++c->pipeline_size;
    if (c->pipeline_commands != NULL) {
        c->pipeline_commands = realloc(c->pipeline_commands, c->pipeline_size * sizeof(Command));
    } else {
        c->pipeline_commands = malloc(c->pipeline_size * sizeof(Command));
    }
    c->pipeline_commands[c->pipeline_size - 1] = *cmd;
    return 0;
}

int
init_redirect_command(Command *c)
{
    c->kind = KIND_REDIRECT;
    c->rd_mode = RD_INPUT;
    c->rd_path = NULL;
    return 0;
}

int
set_rd_command(Command *c, Command *cmd)
{
    c->rd_command = malloc(sizeof *cmd);
    *c->rd_command = *cmd;
    return 0;
}

int
init_simple_command(Command *c)
{
    c->kind = KIND_SIMPLE;
    c->argc = 0;
    c->argv = NULL;
    return 0;
}

int
append_word_simple_command(Command *c, char *arg)
{
    ++c->argc;
    c->argv = realloc(c->argv, c->argc * sizeof arg);
    c->argv[c->argc - 1] = arg;
    return 0;
}

void free_command(Command *c)
{
    switch (c->kind) {
        case KIND_SIMPLE: 
            for (int i = 0; i < c->argc; ++i) {
                free(c->argv[i]);
            }
            free(c->argv);
            break;
        case KIND_SEQ:
            for (int i = 0; i < c->seq_size; ++i) {
                free_command(&c->seq_commands[i]);
            }
            free(c->seq_commands);
            free(c->seq_operations);
            break;
        case KIND_PIPELINE:
            for (int i = 0; i < c->pipeline_size; ++i) {
                free_command(&c->pipeline_commands[i]);
            }
            free(c->pipeline_commands);
            break;
        case KIND_REDIRECT:
            free(c->rd_path);
            free_command(c->rd_command);
            free(c->rd_command);
            break;
        default:
            break;
    }
}