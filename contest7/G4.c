#include <stdio.h>
#include <string.h>

enum { BUF_SIZE = 257 };

int
main(int args, char *argv[])
{
    char str1[BUF_SIZE];
    FILE *f, *new_file;
    f = fopen (argv[1], "r");
    new_file = tmpfile();
    while (fgets(str1, BUF_SIZE, f) != NULL) {
        if (strstr(str1, argv[2])) {
            fprintf(new_file, "%s", str1);
        }
    }
    fclose(f);

    f = fopen (argv[1], "w");
    fseek(new_file, 0, SEEK_SET);
    while (fgets(str1, BUF_SIZE, new_file) != NULL) {
        fprintf(f, "%s", str1);
    }
    fclose(f);
    fclose(new_file);
    return 0;
}