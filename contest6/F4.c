#include <stdio.h>

enum
{
    BUF_SIZE = 10000,
    GETVAL = 0,
    SETVAL = 1
};

typedef union Argument
{
    int number;
    int *pointer;
} Argument;

typedef struct Masiv
{
    int id;
    int value;
} Masiv;

static Masiv data[BUF_SIZE];

int
command(int id, int cmd, Argument arg)
{
    if (cmd != GETVAL && cmd != SETVAL) {
        return 2;
    }

    int find_id = 0;
    int i = 0;
    for (; i < BUF_SIZE; ++i) {
        if (data[i].id == id) {
            find_id = 1;
            break;
        }
    }

    if (find_id == 0) {
        return 1;
    }

    if (cmd == GETVAL) {
        *arg.pointer = data[i].value;
        return 0;
    } else {
        data[i].value = arg.number;
        return 0;
    }
}

void
creat_mas_elem(void)
{
    int i, id, value;
    scanf("%d%d%d", &i, &id, &value);
    data[i].id = id;
    data[i].value = value;
}

int
main(void)
{
    int d;
    Argument arg;
    arg.pointer = &d;
    creat_mas_elem();
    creat_mas_elem();

    scanf("%d", &d);
    if (command(d, GETVAL, arg)) {
        printf("NONE\n"); 
        return 0;
    } else {
        arg.number = *arg.pointer + 1;
        command(d, SETVAL, arg);
        printf("%d\n", arg.number);
        return 0;
    }
}