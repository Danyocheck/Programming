#include <stdio.h>
#include <string.h>

char *
find_sec_end(char *arr)
{
    char *str = "end";
    char *symb_aft_end = arr;
    int amou_of_end = 0;
    while (*arr != '\0') {
        if (strstr(arr, str)) {
            arr = strstr(arr, str);           // memorizing the end address
            symb_aft_end = arr + strlen(str); // address of the character following the end
            ++amou_of_end;
        }
        if (amou_of_end == 2) {  // if there is a second end, we return the address
            return symb_aft_end; // of the character following it
        }
        ++arr;
    }
    return NULL; // if there was one end, or there were none at all return NULL
}

int
main(int args, char *argv[])
{
    for (int i = 1; i < args; ++i) {
        printf("%s\n", argv[i]);
    }
    for (int i = 1; i < args; ++i) {
        if (find_sec_end(argv[i]) != NULL) {
            printf("%s\n", find_sec_end(argv[i]));
        }
    }
}