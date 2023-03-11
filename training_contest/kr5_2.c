#include <stdio.h>
#include <stdlib.h>

enum { MIN_ARG = 2, START = 0 };

FILE *
merge (FILE * f1, FILE * f2)
{
    FILE * f3 = tmpfile();
    int n1, n2, end1 = 0, end2 = 0;
    if (fscanf(f1, "%d", &n1) != 1) {
        end1 = 1;
    }
    if (fscanf(f2, "%d", &n2) != 1) {
        end2 = 1;
    }
    while ((end1 == 0) && (end2 == 0)) {
        if (n1 < n2) {
            fprintf(f3, "%d ", n1);
            if (fscanf(f1, "%d", &n1) != 1) {
                end1 = 1;
            }
        }
        else {
            fprintf(f3, "%d ", n2);
            if (fscanf(f2, "%d", &n2) != 1) {
                end2 = 1;
            }
        }
    }
    while (end1 == 0) {
        fprintf(f3, "%d ", n1);
        if (fscanf(f1, "%d", &n1) != 1) {
            end1 = 1;
        }
    }
    while (end2 == 0) {
        fprintf(f3, "%d ", n2);
        if (fscanf(f2, "%d", &n2) != 1) {
            end2 = 1;
        }
    }
    fclose(f1);
    fclose(f2);
    fseek(f3, 0, SEEK_SET);
    return f3;
}

int
main(int argc, char *argv[])
{
    if (argc < MIN_ARG) {
        printf("\n");
        return 0;
    }
    int thinning, i, size = argc - 1;
    long long n;
    FILE *f, **pq_file = calloc(size, sizeof *f);
    for (int i = 0; i < size; ++i) {
        pq_file[i] = fopen(argv[i + 1], "r");
    }
    while (size > 1) {
        thinning = size / 2;
        for (i = 0; i < thinning; ++i) {
            pq_file[i] = merge(pq_file[2 * i], pq_file[2 * i + 1]);
        }
        if (size & 1) {
            pq_file[size / 2] = pq_file[size - 1];
            size = size / 2 + 1;
        }
        else {
            size = size / 2;
        }
    }
    f = pq_file[START];
    fseek(f, 0, SEEK_SET);
    if (fscanf(f, "%lld", &n) == 1) {
        printf("%lld", n);
    }
    while (fscanf(f, "%lld", &n) == 1) {
        printf(" %lld", n);
    }
    printf("\n");
    free(pq_file);
    fclose(f);
    return 0;
}
