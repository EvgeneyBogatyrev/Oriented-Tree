#include <stdio.h>
#include <stdlib.h>
#include <string.h>

enum
{
    LENGTH = 100
};

struct Leader
{
    char *key;
    int count;
    struct Leader *next;
    struct Trailer *trail;
};

struct Trailer
{
    struct Leader *id;
    struct Trailer *next;
};

struct Graph
{
    struct Leader *head;
    struct Leader *tail;
    int leader_number;
    int error;
};

void free_stdin(void)
{
    int c;
    while ( (c = getchar()) != '\n' );
}

char *cut_newline(char *line)
{
    int len = strlen(line);
    if (line[len - 1] == '\n')
        line[len - 1] = '\0';
    return line;
}

struct Leader *initialize_tail(void)
{
    struct Leader *tail = malloc(sizeof(struct Leader));
    tail->next = NULL;
    return tail;
}

struct Graph *initialize_graph(void)
{
    struct Graph *g = malloc(sizeof(struct Graph));
    g->leader_number = 0;
    g->error = 0;
    g->tail = initialize_tail();
    g->head = g->tail;
    return g;
}

void free_trailer(struct Trailer *trail)
{
    if (trail) {
        free_trailer(trail->next);
        free(trail);
    }
}

void free_leader(struct Leader *leader, struct Leader *tail)
{
    if (leader != tail) {
        free_leader(leader->next, tail);
        free_trailer(leader->trail);
        free(leader);
    }
}

void free_graph(struct Graph *graph)
{
    free_leader(graph->head, graph->tail);
    free(graph->tail);
    free(graph);
}

int search_trails(struct Leader *head, struct Leader *needle, int deep)
{
    struct Trailer *search_trail = head->trail;
    while (search_trail) {
        if (search_trail->id == needle)
            return 1;

        if (deep && search_trails(search_trail->id, needle, deep))
            return 1;

        search_trail = search_trail->next;
    }
    return 0;
}

struct Leader *find(struct Graph *graph, char *keyword)
{
    struct Leader *head = graph->head;
    struct Leader *tail = graph->tail;
    tail->key = keyword;

    struct Leader *curr_leader = head;
    while (strcmp(curr_leader->key, keyword)) {
        curr_leader = curr_leader->next;
    }

    if (curr_leader == tail) {
        tail = initialize_tail();
        curr_leader->next = tail;
        curr_leader->trail = NULL;
        curr_leader->count = 0;
        graph->tail = tail;
        graph->leader_number++;
    }

    return curr_leader;
}

struct Graph *initialize_list(void)
{
    struct Graph *graph = initialize_graph();

    int amount;
    scanf("%d", &amount);
    free_stdin();

    for (int i = 0; i < amount; ++i) {
        struct Leader *x, *y;
        char *key_x = malloc((LENGTH + 1) * sizeof(char));
        char *key_y = malloc((LENGTH + 1) * sizeof(char));

        fgets(key_x, LENGTH, stdin);
        fgets(key_y, LENGTH, stdin);

        x = find(graph, cut_newline(key_x));
        y = find(graph, cut_newline(key_y));

        if (search_trails(y, x, 1)) {
            graph->error = 1;
            fprintf(stderr, "Error: cycle has been detected\n");
            return graph;
        }

        if (!search_trails(x, y, 0)) {
            struct Trailer *new_trail = malloc(sizeof(struct Trailer));
            new_trail->id = y;
            new_trail->next = x->trail;
            x->trail = new_trail;

            y->count++;
        }
    }
    return graph;
}

char **sort_list(struct Graph *graph)
{
    char **output = malloc((graph->leader_number + 1) * sizeof(char*));
    int counter = 0;

    struct Leader *curr_leader = graph->head;
    struct Leader *tail = graph->tail;
    struct Leader *head = NULL;
    struct Leader *next = NULL;

    while (curr_leader != tail) {
        next = curr_leader->next;
        if (curr_leader->count == 0) {
            curr_leader->next = head;
            head = curr_leader;
        }
        curr_leader = next;
    }

    curr_leader = head;
    while (head) {
        graph->leader_number--;
        *(output + counter++) = head->key;

        for (struct Trailer *t = head->trail; t != NULL; t = t->next) {
            struct Leader *curr = t->id;
            curr->count--;
            if (curr->count == 0) {
                curr->next = head->next;
                head->next = curr;
            }
        }
        head = head->next;
    }
    free(graph);
    free_leader(curr_leader, NULL);

    *(output + counter) = NULL;
    return output;
}

void print_info(struct Graph *graph)
{
    printf("\nNumber of Leaders: %d\n\n", graph->leader_number);
    struct Leader *curr_leader = graph->head;
    while (curr_leader != graph->tail) {
        printf("Leader %s:\n", curr_leader->key);
        printf("Count: %d\n", curr_leader->count);
        printf("Trail: ");
        struct Trailer *t = curr_leader->trail;
        while (t) {
            printf("%s ", t->id->key);
            t = t->next;
        }
        puts("\n");
        curr_leader = curr_leader->next;
    }
}

int
main(void)
{
    struct Graph *graph = initialize_list();
    if (graph->error)
        return 0;

    char **output = sort_list(graph);
    char **p = output;
    printf("\n");
    while(*p) {
        printf("%s ", *p);
        p++;
    }
    return 0;
}
