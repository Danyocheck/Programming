#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

int
main(int argc, char *argv[])
{
    FILE *f = fopen(argv[1], "r");
    FILE *new_file = tmpfile();
    long numbers_read = 0;
    int num;
    while (fscanf(f, "%d", &num) == 1) { // read numbers from file
        fwrite(&num, sizeof(int), 1, new_file); // write numbers to new file
        ++numbers_read;
    }
    fclose(f);

    f = fopen(argv[1], "w");
    for (long i = numbers_read - 1; i >= 0; --i) { // read numbers from new file in reverse order
        fseek(new_file, i * sizeof(int), SEEK_SET); // set position to read
        fread(&num, sizeof(int), 1, new_file); // read number
        fprintf(f, "%d ", num); // write number to file
    }
    fclose(f);
    fclose(new_file);
    return 0;
}