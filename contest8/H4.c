#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

enum { BUF_SIZE = 3 };

typedef struct Pair_func_str {
    char *str;
    void (*func)(char *);
} Pair_func_str;

typedef struct Pair_switch {
    long pair_num; // number of displays
    void (*default_case)(char *);
    Pair_func_str struk_ar[BUF_SIZE];
} Pair_switch; 

void
add_case(char *str)
{
    static int number_add = 0;
    ++number_add;
    if (number_add % 2 != 0) {
        printf("%d\n", number_add);
    }
}

void
sub_case(char *str)
{
    static int number_sub = 0;
    ++number_sub;
    if (number_sub == 2) {
        exit(0);
    }
}

void
default_case(char *str)
{
}

void
string_switch(Pair_switch *my_switch, char *string)
{
    int i = 0;
    while (i < my_switch->pair_num) {
        if (strcmp(my_switch->struk_ar[i].str, string) == 0) {
            my_switch->struk_ar[i].func(string);
            break;
        }
        ++i;
    }
    if (i == my_switch->pair_num) {
        my_switch->default_case(string);
    }
}

int
main(int args, char *argv[])
{
    ++argv;
    --args;
    Pair_func_str pair1 = {"add", add_case};
    Pair_func_str pair2 = {"sub", sub_case};
    Pair_switch pair_switch = {2, default_case, {pair1, pair2}};

    for (int i = 0; i < args; ++i) {
        string_switch(&pair_switch, argv[i]);
    }
    return 0; 
}