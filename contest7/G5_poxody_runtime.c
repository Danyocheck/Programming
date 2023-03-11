#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int
add_str_in_new_file(FILE *filename, FILE *new_filename, int start_position)
{
    fseek(filename, start_position, SEEK_SET);
    int symbol = fgetc(filename);
    long i = 1;
    while ((symbol != '\n') && (symbol != EOF)) {
        putc(symbol, new_filename);
        symbol = fgetc(filename);
        ++i;
    }
    if (symbol == '\n') {
        putc(symbol, new_filename);
    }
    return start_position + i - 1;
}

int
main(int args, char *argv[])
{
    FILE *f = fopen(argv[1], "r");
    FILE *new_file = tmpfile();
    long len = strlen(argv[2]) + 1;
    char *str_in_file = malloc(sizeof(char) * len);

    long start_position = 0;
    long i = 0;
    while (fgets(str_in_file, len, f) != NULL) {
        if (strstr(str_in_file, argv[2])) {
            i = add_str_in_new_file(f, new_file, start_position);
            start_position = i + 1;
        } else if (strchr(str_in_file, '\n')) {
            start_position = i + strlen(str_in_file);
            i = start_position - 1;
        }
        ++i;
        fseek(f, i, SEEK_SET);
    }
    fclose(f);

    f = fopen(argv[1], "w");
    fseek(new_file, 0, SEEK_SET);
    int symbol = fgetc(new_file);;
    while (symbol != EOF) {
        putc(symbol, f);
        symbol = fgetc(new_file);
    }
    free(str_in_file);
    fclose(f);
    fclose(new_file);
    return 0;
}