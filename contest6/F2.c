#include <stdio.h>
#include <stdlib.h>

typedef struct List
{
    int symbol;
    struct List *next;
    struct List *prev;
} List;

int
main(void)
{
    int number;
    int counter = 0;
    List *p = NULL;
    List *help_prev = NULL;
    List *first = NULL;
    while (scanf("%d", &number) == 1) {
        p = malloc(sizeof(struct List));
        p->symbol = number;
        p->next = NULL;
        p->prev = help_prev;
        help_prev = p;
        if (counter == 0) {
            first = p;
        }
        ++counter;
    }

    if (p == NULL) {
        return 0;
    }

    p->next = first;
    first->prev = p;
    while (p != first) {
        printf("%d ", p->symbol);
        help_prev = p->prev;
        free(p);
        p = help_prev;
    }
    printf("%d ", first->symbol);
    free(first);
}