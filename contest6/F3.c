#include <stdio.h>
#include <stdlib.h>

typedef struct List
{
    int symbol;
    struct List *next;
    struct List *prev;
} List;

List *
delet_elem(List *root, List *lst)
{
    if (lst == lst->next) {
        root = NULL;
    } else {
        lst->prev->next = lst->next;
        lst->next->prev = lst->prev;
        if (lst == root) {
            root = lst->next;
        }
    }
    return root;
}

void
add_elem(List *root, List *lst)
{
    root->prev->next = lst;
    lst->prev = root->prev;
    root->prev = lst;
    lst->next = root;
}

List *
change_List(List *root)
{
    List *lst = root;
    List *help_lst;
    List *last = root->prev;
    while (lst != last) {
        if (lst->symbol > 100) {
            root = delet_elem(root, lst);
            add_elem(root, lst);
            lst = lst->next;
        } else if (lst->symbol % 2 != 0) {
            root = delet_elem(root, lst);
            help_lst = lst->next;
            free(lst);
            lst = help_lst;
        } else {
            lst = lst->next;
        }
    }
    if ((lst->symbol > 100) && (lst != lst->next)) {
        root = delet_elem(root, lst);
        add_elem(root, lst);
    } else if ((lst->symbol > 100) && (lst == lst->next)) {
        return root;
    } else if (lst->symbol % 2 != 0) {
        root = delet_elem(root, lst);
        free(lst);
    }
    return root;
}


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
        if (help_prev != NULL) {
            help_prev->next = p;
        }
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

    first = change_List(first);

    if (first == NULL) {
        return 0;
    }

    p = first->prev;
    while (p != first) {
        printf("%d ", p->symbol);
        help_prev = p->prev;
        free(p);
        p = help_prev;
    }
    printf("%d ", first->symbol);
    free(first);
}