#include <stdio.h>
#include <string.h>
#include <stdlib.h>

FILE *
new_file(FILE *f1, FILE *f2)
{
    FILE *f = tmpfile();
    int value1, value2;
    while (fscanf(f1, "%d", &value1) != 1 && fscanf(f2, "%d", &value2) != 1) {
        if (value1 < value2) {
            fprintf(f, "%d ", value1);
        } else {
            fprintf(f, "%d ", value2);
        }
    }
    return f;
}

void
min(FILE **mas, int n)
{
    FILE *f1 = mas[0];
    FILE *f2 = mas[1];
    FILE *f = new_file(f1, f2);
    for (int i = 2; i < n; ++i) {
        f1 = mas[i];
        f = new_file(f1, f);
    }
    fseek(f, 0, SEEK_SET);
    while (fscanf(f, "%d", &n) == 1) {
        printf("%d ", n);
    }
    fclose(f);
}

int
main(int argc, char *argv[])
{
    ++argv;
    --argc;
    FILE **f = malloc((argc - 1) * sizeof(FILE *));
    for (int i = 0; i < argc; ++i) {
        f[i] = fopen(argv[i], "r");
    }
    new_file(f, argc);
}