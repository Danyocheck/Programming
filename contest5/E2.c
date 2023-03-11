#include <stdio.h>
#include <string.h>
#include <ctype.h>

enum { BUF_SIZE = 82 };

void
print_three_sym(char *arr[])
{
    char *str;
    int i = 0;
    while (arr[i] != NULL) {
        str = arr[i];
        if (strlen(str) >= 3) { // if len of word >= 3 print first 3 symbols
            for (int k = 0; k < 3; ++k) {
                printf("%c", *str);
                ++str;
            }
            printf("\n");
        }
        ++i;
    }
}

void
select_lines(char *arr[], char *str)
{
    char *st_of_new_str = str;
    int counter = 0;
    int i = 0;
    while (*str != '\0') {
        while (isalpha(*str)) { // if we get a symbol ++counter
            ++counter;
            ++str;
        }
        if (counter > 0) {        // if we have sumbols before counter > 0
            arr[i] = st_of_new_str; // and so there was a line
            *str = '\0';
            ++i;
            counter = 0;
        } else {
            *str = '\0';
        }
        ++str;
        st_of_new_str = str;
    }
    arr[i] = NULL;
}

int
main(void)
{
    static char *arr[BUF_SIZE];
    static char str[BUF_SIZE];
    char *dp = str;
    
    fgets(str, sizeof str, stdin);
    dp = strstr(str, "\n");
    *dp = '\0';

    select_lines(arr, str);
    print_three_sym(arr);
}