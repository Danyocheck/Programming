#include <stdio.h>

int
main(int args, char *argv[])
{
    FILE *f = fopen(argv[1], "r+");
    char buf[2];
    size_t return_condition = fread(buf, 1, 2, f);
    if ((return_condition == 0) || (return_condition == 1)) {
        fclose(f);
        return 0;
    }
    fseek(f, 1, SEEK_SET);
    fwrite(buf, 1, 1, f);
    fclose(f);
    return 0;
}