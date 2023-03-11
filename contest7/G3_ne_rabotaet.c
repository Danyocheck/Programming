#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

void
revers_file(char *filename)
{
    FILE *f1;
    int symbol1, symbol2;
    int i = 0;
    f1 = fopen(filename, "r+");
    while ((symbol1 = fgetc(f1)) != EOF) {
        ++i;
    }
    for (int j = 0; j < i / 2; ++j) {
        fseek(f1, j, SEEK_SET);
        symbol1 = fgetc(f1);
        fseek(f1, - j - 1, SEEK_END);
        symbol2 = fgetc(f1);
        fseek(f1, j, SEEK_SET);
        fputc(symbol2, f1);
        fseek(f1, - j - 1, SEEK_END);
        fputc(symbol1, f1);
    }
    fclose(f1);
}

void
revers_numbers_in_file(char *filename)
{
    FILE *f1;
    int symbol, symbol1, symbol2, pointer_start, pointer_end;
    f1 = fopen(filename, "r+");
    while ((symbol = fgetc(f1)) != EOF) {
        if (isdigit(symbol)) {
            pointer_start = ftell(f1) - 1;
            int counter = 0;
            while (isdigit(symbol)) {
                symbol = fgetc(f1);
                ++counter;
            }
            pointer_end = pointer_start + counter;
            fseek(f1, pointer_start, SEEK_SET);
            for (int j = 0; j <= (pointer_end - pointer_start) / 2; ++j) {
                symbol1 = fgetc(f1);
                fseek(f1, pointer_end - j, SEEK_SET);
                symbol2 = fgetc(f1);
                printf("%c ", symbol2);
                fseek(f1, pointer_start + j, SEEK_SET);
                fputc(symbol2, f1);
                fseek(f1, pointer_end - j, SEEK_SET);
                fputc(symbol1, f1);
            }
            fseek(f1, pointer_end + 1, SEEK_SET);
            printf("\n");
        }
    }
    fclose(f1);
}


int
main(int args, char *argv[])
{
    revers_file("text.txt");
    revers_numbers_in_file("text.txt");
}