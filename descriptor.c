#include <stdio.h>
#include <stdlib.h>

#include "descriptor.h"

enum
{
    stop_char = '&'
};

struct Trail
{
    struct Trail *next;
    struct Leader *id;
};

struct Leader
{
    char key;
    int count;
    struct Leader *next;
    struct Trail *trl;
};

struct Answer
{
    char *buff;
    int size;
};

void print(struct Leader *head)
{
    while (head->key != stop_char) {
        printf("(%c, %d) ", head->key, head->count);
        head = head->next;
    }
    puts("");
}

void print_answer(struct Answer *ans) {
    for (int i = 0; i < ans->size; ++i) {
        printf("%c ", ans->buff[i]);
    }
    puts("");
}

struct Leader *new_leader(void)
{
    struct Leader *p = malloc(sizeof(struct Leader));
    p->key = stop_char;
    p->count = 0;
    p->next = NULL;
    p->trl = NULL;
    return p;
};

void free_leader(struct Leader *leader)
{
    if (leader == NULL) return;

    free(leader->trl);
    free_leader(leader->next);
    free(leader);
}

void push(struct Leader *head, char x, char y)
{
    struct Leader *xx = NULL, *yy = NULL, *sv = NULL;

    while (head->key != stop_char && (!xx || !yy)) {
        if (head->key == x) {
            xx = head;
        }
        if (head->key == y) {
            yy = head;
        }
        head = head->next;
    }

    if (!xx) {
        head->key = x;
        head->count = 0;
        head->trl = NULL;
        head->next = new_leader();
        xx = head;
        head = head->next;
    }
    if (!yy) {
        head->key = y;
        head->count = 0;
        head->trl = NULL;
        head->next = new_leader();
        yy = head;
        head = head->next;
    }

    struct Trail *p = xx->trl;
    while (p) {
        if (p->id == yy)
            break;
    }

    if (!p) {
        p = xx->trl;
        xx->trl = malloc(sizeof(struct Trail));
        xx->trl->id = yy;
        xx->trl->next = p;

        yy->count++;
    }

    //print(sv);
}

struct Answer *new_answer(void)
{
    struct Answer *ans = malloc(sizeof(struct Answer));
    return ans;
}

void free_answer(struct Answer* ans)
{
    free(ans->buff);
    free(ans);
}

struct Answer *sort(struct Leader* head)
{
    int size = 1;
    int filled = 0;
    char *buff = malloc(size * sizeof(char));

    while (head->key != stop_char) {
        //printf("New global while: %c count %d\n", head->key, head->count);

        if (filled == size) {
            char *q = realloc(buff, size * 2 + 1);
            if (q) {
                buff = q;
                size = 2 * size + 1;
            }
        }

        struct Leader *p = head, *prev = NULL;
        while (p->key != stop_char) {
            //printf("New Inner While: %c count %d\n", p->key, p->count);

            if (p->count == 0) {
                buff[filled++] = p->key;

                struct Trail *t = p->trl;
                while (t) {
                    t->id->count--;
                    t = t->next;
                }

                if (!prev) head = p->next;
                else {
                    prev->next = p->next;
                    free_leader(p);
                }
                break;

            }
            prev = p;
            p = p->next;
        }

    }

    struct Answer *ans = malloc(sizeof(struct Answer));
    ans->size = filled;
    ans->buff = buff;
    return ans;
}
