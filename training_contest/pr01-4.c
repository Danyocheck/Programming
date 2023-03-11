#include <stdio.h>
#include <string.h>
#include <stdlib.h>

void process(struct List *pl, const char *str)
{
    struct Node *cur_elem = pl->first;
    struct Node *finish_elem = pl->last;
    while (cur_elem != finish_elem) {
        int res = strcmp(cur_elem->elem, str);
        printf("%d\n", res);
        struct Node *help_elem = cur_elem;
        if (res == 0) {
            if (cur_elem == pl->first) {
                cur_elem->next->prev = NULL;
                cur_elem = cur_elem->next;
                pl->first = cur_elem;
                free(help_elem->elem);
                free(help_elem);
            } else {
                cur_elem->next->prev = cur_elem->prev;
                cur_elem->prev->next = cur_elem->next;
                cur_elem = cur_elem->next;
                free(help_elem->elem);
                free(help_elem);
            }
        } else if (res > 0) {
            if (cur_elem == pl->first) {
                cur_elem->next->prev = NULL;
                cur_elem = cur_elem->next;
                pl->first = cur_elem;
                pl->last->next = help_elem;
                help_elem->prev = pl->last;
                help_elem->next = NULL;
                pl->last = help_elem;
            } else {
                cur_elem->next->prev = cur_elem->prev;
                cur_elem->prev->next = cur_elem->next;
                cur_elem = cur_elem->next;
                pl->last->next = help_elem;
                help_elem->prev = pl->last;
                help_elem->next = NULL;
                pl->last = help_elem;
            } 
        } else {
            cur_elem = cur_elem->next;
        }
    }
    int res = strcmp(cur_elem->elem, str);
    if (res == 0) {
        if (pl->first != pl->last) {
            if (cur_elem != pl->first) {
                cur_elem->prev->next = NULL;
                pl->last = cur_elem->prev;
                free(cur_elem->elem);
                free(cur_elem);
            } else {
                cur_elem->next->prev = NULL;
                pl->first = cur_elem->next;
                free(cur_elem->elem);
                free(cur_elem);
            }
        } else {
            free(cur_elem->elem);
            free(pl->first);
            pl->first = NULL;
        }
    } else if (res > 0 && cur_elem != pl->last) {
        if (cur_elem != pl->first) {
            cur_elem->next->prev = cur_elem->prev;
            cur_elem->prev->next = cur_elem->next;
            pl->last->next = cur_elem;
            cur_elem->prev = pl->last;
            cur_elem->next = NULL;
            pl->last = cur_elem;
        } else {
            cur_elem->next->prev = NULL;
            pl->first = cur_elem->next;
            pl->last->next = cur_elem;
            cur_elem->prev = pl->last;
            cur_elem->next = NULL;
            pl->last = cur_elem;
        }
    }
}