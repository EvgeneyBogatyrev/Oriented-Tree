#include <stdio.h>
#include <stdlib.h>

#include "descriptor.h"

int main(void)
{
    struct Leader *head = new_leader();

    push(head, 'a', 'b');
    push(head, 'b', 'd');
    push(head, 'd', 'f');
    push(head, 'b', 'l');
    push(head, 'd', 'h');
    push(head, 'f', 'c');
    push(head, 'a', 'c');
    push(head, 'c', 'e');
    push(head, 'e', 'h');
    push(head, 'g', 'e');
    push(head, 'g', 'k');
    push(head, 'k', 'd');
    push(head, 'k', 'l');

    struct Answer *ans = sort(head);
    print_answer(ans);

    free_answer(ans);
    free_leader(head);
    return 0;
}
