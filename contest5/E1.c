#include <stdio.h>

enum { BUF_SIZE = 100 };

void
multiply_matrix(
        double result_matrix[][BUF_SIZE],
        const double matrix1[][BUF_SIZE],
        const double matrix2[][BUF_SIZE],
        int line_matrix1,
        int column_matrix1,
        int column_matrix2)
{
    for (int i = 0; i < line_matrix1; ++i) {
        for (int j = 0; j < column_matrix2; ++j) {
            for (int k = 0; k < column_matrix1; ++k) {
                result_matrix[i][j] += matrix1[i][k] * matrix2[k][j];
            }
        }
    }
}

void
read_matrix(double matrix[][BUF_SIZE], int lines, int columns)
{
    for (int i = 0; i < lines; ++i) {
        for (int j = 0; j < columns; ++j) {
            scanf("%lf", &matrix[i][j]);
        }
    }
}

void
print_matrix(const double matrix[][BUF_SIZE], int lines, int columns)
{
    for (int i = 0; i < lines; ++i) {
        for (int j = 0; j < columns; ++j) {
            printf("%.1lf ", matrix[i][j]);
        }
        printf("\n");
    }
}

int
main(void)
{
    int lines1, lines2, columns1, columns2;

    scanf("%d %d", &lines1, &columns1);
    static double matrix1[BUF_SIZE][BUF_SIZE];
    read_matrix(matrix1, lines1, columns1);

    scanf("%d %d", &lines2, &columns2);
    static double matrix2[BUF_SIZE][BUF_SIZE];
    read_matrix(matrix2, lines2, columns2);

    static double result_matrix[BUF_SIZE][BUF_SIZE];
    multiply_matrix(result_matrix, matrix1, matrix2, lines1, columns1, columns2);
    print_matrix(result_matrix, lines1, columns2);
}