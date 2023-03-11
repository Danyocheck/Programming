typedef struct Command {
    enum {
        KIND_SIMPLE,
        KIND_SEQ,
        KIND_PIPELINE,
        KIND_REDIRECT,
    } kind;
    union {
        struct {
            int argc;
            char **argv;
        };
        struct {
            int seq_size;
            struct Command *seq_commands;
            enum {
                OP_SEQ,
                OP_CONJUNCT,
                OP_DISJUNCT,
                OP_BACKGROUND
            } seq_operations[4];
        };
        struct {
            int pipeline_size;
            struct Command *pipeline_commands;
        };
        struct {
            enum {
                RD_INPUT,
                RD_OUTPUT,
                RD_APPEND,
            } rd_mode;
            char *rd_path;
            struct Command *rd_command;
        };
    };
} Command;

int
main(void)
{
    return 0;
}