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

int init_empty_command(Command *c);
int init_sequence_command(Command *c);
int append_command_to_sequence(Command *c, Command *cmd);
int append_operation_to_sequence(Command *c, int op);
int init_pipeline_command(Command *c);
int append_to_pipeline(Command *c, Command *cmd);
int init_redirect_command(Command *c);
int set_rd_command(Command *c, Command *cmd);
int init_simple_command(Command *c);
int append_word_simple_command(Command *c, char *arg);
void free_command(Command *c);