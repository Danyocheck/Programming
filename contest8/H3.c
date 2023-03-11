#include <stdio.h>
#include <stdlib.h>
#include <string.h>

enum { BUF_SIZE = 3 };

typedef struct My_switch {
    char **str;
    void (**mas_of_func)(char *);
} My_switch;

void
add_case(char *str)
{
    while (*str != '\0') {
        *str = toupper(*str);
        printf("%c", *str);
        ++str;
    }
    printf("\n");
}

void
sub_case(char *str)
{
    while (*str != '\0') {
        *str = toupper(*str);
        printf("%c", *str);
        ++str;
    }
    printf("\n");
}

void
default_case(char *str)
{
    int res = strtol(str, NULL, 0);
    int len = strlen(str);
    if (res == 0) {
        printf("UNKNOWN\n");
    } else {
        while (len > 0) {
            if (res == 0) {
                printf("UNKNOWN\n");
                return;
            }
            res = res / 10;
            --len;
        }
        printf("NUMBER\n");
    }
}

void
string_switch(My_switch *my_switch, char *string)
{
    char **start_mas_str = my_switch->str;
    void (**start_mas_func)(char *) = my_switch->mas_of_func;
    while ((*my_switch->str != NULL) && (strcmp(*my_switch->str, string) != 0)) {
        ++my_switch->str;
        ++my_switch->mas_of_func;
    }
    if (*my_switch->mas_of_func != NULL) {
        (*my_switch->mas_of_func)(string);
    }
    my_switch->str = start_mas_str;
    my_switch->mas_of_func = start_mas_func;
}

int
main(int args, char *argv[])
{
    ++argv;
    --args;
    char *codes[BUF_SIZE] = {"add", "sub", NULL};
    void (*mas_of_func[BUF_SIZE])(char *) = {add_case, sub_case, default_case};
    My_switch my_switch = {codes, mas_of_func};

    for (int i = 0; i < args; ++i) {
        string_switch(&my_switch, argv[i]);
    }
    return 0; 
}