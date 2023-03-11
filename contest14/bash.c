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
                OP_SEQ,       // один амперсанд
                OP_CONJUNCT,  // конъюнкция
                OP_DISJUNCT,  // дизъюнкция
                OP_BACKGROUND // точка с запятой
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
                RD_OUTPUT, // перенаправление вывода
                RD_APPEND, // перенаправление вывода с дописыванием
            } rd_mode;
            char *rd_path;
            struct Command *rd_command;
        };
    };
} Command;

int
run_command(Command *cmd)
{
    return 0;
}

int
main(void)
{
    Command c1_1_1 = {
        .kind = KIND_SIMPLE,
        .argc = 2,
        .argv = (char *[]){"uname", "-a", 0}
    };
    Command c1_1 = {
        .kind = KIND_PIPELINE,
        .pipeline_size = 1,
        .pipeline_commands = &c1_1_1,
    };
    Command c1 = {
        .kind = KIND_SEQ,
        .seq_size = 1,
        .seq_commands = &c1_1,
        .seq_operations = (int []){OP_SEQ},
    };
    run_command(&c1);

    Command c2_1_1_1 = {
        .kind = KIND_SIMPLE,
        .argc = 4,
        .argv = (char *[]) {"echo", "1", "2", "3", 0}
    };
    Command c2_1_1 = {
        .kind = KIND_REDIRECT,
        .rd_mode = RD_OUTPUT,
        .rd_path = "file",
        .rd_command = &c2_1_1_1
    };
    Command c2_1 = {
        .kind = KIND_PIPELINE,
        .pipeline_size = 1,
        .pipeline_commands = &c2_1_1,
    };
    Command c2_2_1_1 = {
        .kind = KIND_SIMPLE,
        .argc = 1,
        .argv = (char *[]) {"wc", 0},
    };
    Command c2_2_1 = {
        .kind = KIND_REDIRECT,
        .rd_mode = RD_INPUT,
        .rd_path = "file",
        .rd_command = &c2_2_1_1,
    };
    Command c2_2 = {
        .kind = KIND_PIPELINE,
        .pipeline_size = 1,
        .pipeline_commands = &c2_2_1,
    };
    Command c2 = {
        .kind = KIND_SEQ,
        .seq_size = 2,
        .seq_commands = (Command []){c2_1, c2_2},
        .seq_operations = (int []){OP_CONJUNCT, OP_BACKGROUND},
    };
    run_command(&c2);

    // command "echo 1 2 3 | wc"

    Command c3_1 = {
        .kind = KIND_PIPELINE,
        .pipeline_size = 2,
        .pipeline_commands = (Command []) {c2_1_1_1, c2_2_1_1}
    };
    Command c3 = {
        .kind = KIND_SEQ,
        .seq_size = 1,
        .seq_commands = &c3_1,
        .seq_operations = (int []){OP_SEQ},
    };
    run_command(&c3);

    // command "echo 1 >> file || echo 2 >> file && cat file"
    Command c4_1_1_1 = {
        .kind = KIND_SIMPLE,
        .argc = 1,
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
        .argc = 1,
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
}
