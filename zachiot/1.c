#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>


struct list_item {
    char *str;
    struct list_item *prev, *next;
};

void
process(struct list_item **list, const char *s)
{
    struct list_item *elem = *list, *el, *end = *list, *last = (*list)->prev;
    struct list_item *help_elem, *start_elem = *list, *cur_elem = *list;
    int if_first = 1;
    do {
        if (strstr(cur_elem->str, s)) {
            for (int i = 0; i < strlen(s); ++i) {
                if (if_first) {
                    if_first = 0;
                    *list = cur_elem;
                }
                help_elem = malloc(sizeof(struct list_item));
                help_elem->str = malloc(sizeof(strlen(s) + 1));
                strcpy(help_elem->str, s);
                cur_elem->next->prev = help_elem;
                help_elem->prev = cur_elem;
                help_elem->next = cur_elem->next;
                cur_elem->next = help_elem;
                cur_elem = cur_elem->next;
            }
            cur_elem = cur_elem->next;
        } else {
            if (cur_elem == last) {
                break;
            }
            el = cur_elem->next;
            cur_elem->next->prev = cur_elem->prev;
            cur_elem->prev->next = cur_elem->next;
            if (cur_elem == *list) {
                *list = (*list)->next;
            }
            last->next->prev = cur_elem;
            cur_elem->next = last->next;
            cur_elem->prev = last;
            last->next = cur_elem;
            last = cur_elem;
            cur_elem = el;
        }
    } while (cur_elem != end);
}

int
main(void)
{
    struct list_item *list, *start_elem, *el;
    struct list_item *help_elem = NULL;
    char str[82];
    while (fgets(str, 80, stdin)) {
        list = malloc(sizeof(struct list_item));
        list->str = malloc(strlen(str) + 1);
        strcpy(list->str, str);
        if (help_elem != NULL) {
            help_elem->next = list;
        } else {
            start_elem = list;
        }
        list->prev = help_elem;
        help_elem = list;
    }

    start_elem->prev = help_elem;
    help_elem->next = start_elem;

    process(&start_elem, "axb");

    list = start_elem;
    el = start_elem->prev;

    while (list != el) {
        printf("hi %s\n", list->str);
        start_elem = list->next;
        free(list->str);
        free(list);
        list = start_elem;
    }
    printf("by %s\n", list->str);
    free(list->str);
    free(list);
}