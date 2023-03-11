#include <stdio.h>
#include <string.h>
#include <stdlib.h>


void process(struct List *pl, const char *str)
{
    if (pl->first == NULL) {
        return;
    }
    struct Node *pq, *end, *buf1, *buf2, *stop;
    pq = pl->first;
    end = pl->last;
    stop = end;
    int last_check = 0;
    while (last_check == 0) {
        if (pq == stop) {
            last_check = 1;
        }
        if (strcmp((pq->elem), str) == 0) {
            buf1 = pq->next;
            buf2 = pq->prev;
            if (pq == pl->first) {
                pl->first = buf1;
            }
            if (pq == pl->last) {
                pl->last = buf2;
            }
            free(pq->elem);
            free(pq);
            pq = NULL;
            if (buf1 != NULL) {
                buf1->prev = buf2;
            }
            if (buf2 != NULL) {
                buf2->next = buf1;
            }
            pq = buf1;
            
        }
        else if (strcmp((pq->elem), str) > 0) {
            buf1 = pq->next;
            buf2 = pq->prev;
            if ((buf1 == NULL) && (buf2 = NULL)) {
                pl->first = pq;
                pl->last = pq;
                return;
            }
            if (pq == pl->last) {
                return;
            }
            if (pq == pl->first) {
                pl->first = buf1;
            }
            if (buf1 != NULL) {
                buf1->prev = buf2;
            }
            if (buf2 != NULL) {
                buf2->next = buf1;
            }
            pq->prev = end;
            end->next = pq;
            end = pq;
            pl->last = end;
            pq->next = NULL;
            pq = buf1;
        }
        else {
            pq = pq->next;
        }
    }
}
