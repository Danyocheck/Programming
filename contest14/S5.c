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
    // printf("hi2\n");
    return pid;
}

int
run_redirect_command(Command *cmd)
{
    printf("hi3\n");
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
                printf("hi1\n");
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

// int
// run_pipeline_command(Command *cmd)
// {
//     int fd[2];
//     int std_in = dup(0);
//     int std_out = dup(1);
//     for (int i = 0; i < cmd->pipeline_size; ++i) {
//         pipe(fd);
//         if (i != 0) {
//             dup2(std_in, 0);
//             close(std_out);
//         }
//         if (i == cmd->pipeline_size - 1) {
//             dup2(std_out, 1);
//             dup2(std_in, 0);
//         } else {
//             dup2(fd[1], 1);
//         }
//         close(fd[1]);
//         run_command(&cmd->pipeline_commands[i]);
//         std_in = fd[0];
//     }
//     close(std_in);
//     return 0;
// }

int
run_pipeline_command(Command *command)
{int status;
    int fd[2];
    // save stdin and stdout to restore them later
    int in = dup(0);
    int out = dup(1);
    int pid = 0;
    for (int i = 0; i < command->pipeline_size; i++) {
        if (i != command->pipeline_size - 1) { //if not last command
            pipe(fd);
        }
        pipe(fd);
        if (i != 0) {
            dup2(in, 0);
            close(in);
        }
        if (i != command->pipeline_size - 1) {
            dup2(fd[1], 1);
        } else {
            dup2(out, 1);
            dup2(in, 0);
        }
        close(fd[1]);

        pid = run_command(&command->pipeline_commands[i]);
        in = fd[0];
        printf("%d\n", i);
    }
    close(in);
    return pid;}

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
    Command c4_1_1_1 = {
        .kind = KIND_SIMPLE,
        .argc = 2,
        .argv = (char *[]){"echo", "1", 0},
    };
    Command c4_1_1 = {
        .kind = KIND_REDIRECT,
        .rd_mode = RD_APPEND,
        .rd_path = "file",
        .rd_command = &c4_1_1_1,
    };
    Command c4_1 = {
        .kind = KIND_PIPELINE,
        .pipeline_size = 1,
        .pipeline_commands = &c4_1_1,
    };
    Command c4_2_1_1 = {
        .kind = KIND_SIMPLE,
        .argc = 2,
        .argv = (char *[]){"echo", "2", 0},
    };
    Command c4_2_1 = {
        .kind = KIND_REDIRECT,
        .rd_mode = RD_APPEND,
        .rd_path = "file",
        .rd_command = &c4_2_1_1,
    };
    Command c4_2 = {
        .kind = KIND_PIPELINE,
        .pipeline_size = 1,
        .pipeline_commands = &c4_2_1,
    };
    Command c4_3_1 = {
        .kind = KIND_SIMPLE,
        .argc = 2,
        .argv = (char *[]){"cat", "file", 0},
    };
    Command c4_3 = {
        .kind = KIND_PIPELINE,
        .pipeline_size = 1,
        .pipeline_commands = &c4_3_1,
    };
    Command c4 = {
        .kind = KIND_SEQ,
        .seq_size = 3,
        .seq_commands = (Command []) {c4_1, c4_2, c4_3},
        .seq_operations = (int []){OP_DISJUNCT, OP_CONJUNCT, OP_SEQ},
    };
    run_command(&c4);
    return 0;
}