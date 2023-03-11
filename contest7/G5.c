#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int
main(int args, char *argv[])
{
    FILE *f = fopen(argv[1], "r");
    FILE *new_file = tmpfile();
    long len = (strlen(argv[2]) + 1) * 2;
    char *str_in_file = malloc(len);

    long start_position = 0;
    long i = 0;
    while (fgets(str_in_file, len, f) != NULL) {
        if (strstr(str_in_file, argv[2])) { // if string in file contains substring from command line
            fseek(f, start_position, SEEK_SET); // go to start position of string in file
            do { // copy string in file to new file
                if (fgets(str_in_file, len, f) == NULL) {
                    break;
                }
                fprintf(new_file, "%s", str_in_file);
            } while (!strchr(str_in_file, '\n'));
            start_position = ftell(f); // save start position of next string in file
            i = start_position;
        } else if (strchr(str_in_file, '\n')) {
            start_position = ftell(f); // if string ends, save start position of next string in file
            i = start_position;
        } else {
            i += len / 2; // if substring in file doesn't doesn't match substring from command line
        }                 // and string in file doesn't end, go to next substring in this string
        fseek(f, i, SEEK_SET);
    }
    fclose(f);

    f = fopen(argv[1], "w");
    fseek(new_file, 0, SEEK_SET);
    while (fgets(str_in_file, len, new_file) != NULL) {
        fprintf(f, "%s", str_in_file);
    }
    free(str_in_file);
    fclose(f);
    fclose(new_file);
    return 0;
}