#include <stdio.h>

enum {
    BUF_SIZE = 10000,
    START_ELEM = 5,
    END_ELEM = 18
};

double *
find_max_elem(double *arr, size_t size_arr)
{
    double *pointer, *max;
    pointer = arr;
    max = pointer;
    if (size_arr == 0) {
        return NULL;
    }
    for (size_t i = 0; i < size_arr; ++i) {
        if (*pointer > *max) {
            max = pointer;
        }
        ++pointer;
    }
    return max;
}

void
change_elems(double *pointer1, double *pointer2)
{
    double help_pointer = *pointer1;
    *pointer1 = *pointer2;
    *pointer2 = help_pointer;
}

void
sorting(double *arr, size_t size_arr)
{
    size_t new_size = size_arr;
    double *pointer1, *pointer2;
    pointer1 = arr;
    for (size_t i = 0; i < size_arr; ++i) {
        pointer2 = find_max_elem(pointer1, new_size); // finding max elem
        change_elems(pointer1, pointer2);             // move it forvard
        ++pointer1;
        --new_size;;
    }
}

void
read_arr(double *arr, size_t size_arr)
{
    for (int i = 0; i < size_arr; ++i) {
        scanf("%lf", &arr[i]);
    }
}

void
print_arr(const double *arr, size_t size_arr)
{
    for (size_t i = 0; i < size_arr; ++i) {
        printf("%.1lf ", arr[i]);
    }
}

int
main(void)
{
    static double arr[BUF_SIZE];
    double *pointer;
    size_t size;
    scanf("%zd", &size);
    read_arr(arr, size);

    if (size < END_ELEM) {
        sorting(arr, size);
        print_arr(arr, size);
    } else {
        pointer = arr + START_ELEM - 1;
        pointer = find_max_elem(pointer, END_ELEM - START_ELEM + 1);
        printf("%.1lf\n", *find_max_elem(pointer, END_ELEM - START_ELEM + 1));
        sorting(arr, size);
        print_arr(arr, size);
    }
    printf("\n");
}