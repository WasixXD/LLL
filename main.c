#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <stdbool.h>
#include <stdatomic.h>

struct Node {
    int value;
    struct Node *next;
};

struct Master {
    struct Node *main;
    struct Node *to_insert; 
};

bool cmp_exchange(struct Node *p, struct Node *old, struct Node *new)  {
    usleep(1);
    if (p != old->next) {
        return false;
    }
    old->next = new;
    return true;
}

void *insert_in(void *data) {
    struct Master *n = (struct Master*)data;
    while(n->main->next == NULL) {
        if(cmp_exchange(n->main->next, n->main, n->to_insert)) {
            break;
        }
        n->main = n->main->next;
    }
    return NULL;
}

void print_nodes(struct Node *master) {
    while(master->next) {
        printf("%d -> ", master->value);
        master = master->next;
    }
    printf("%d\n", master->value);
}

int main() {
    struct Node master = { .value = 0, .next = NULL };

    struct Node n1 = {.value = 1, .next = NULL};
    struct Master add1 = {.main = &master, .to_insert = &n1};

    struct Node n2 = {.value = 2, .next = NULL};
    struct Master add2= {.main = &master, .to_insert = &n2};

    struct Node n3 = {.value = 3, .next = NULL};
    struct Master add3= {.main = &master, .to_insert = &n3};
   
    pthread_t th1, th2;
    pthread_create(&th1, NULL, insert_in, &add1);
    pthread_create(&th2, NULL, insert_in, &add3);
    pthread_create(&th2, NULL, insert_in, &add2);
    sleep(1);
    print_nodes(&master);
    // printf("%d\t", master.value);
    // printf("%d\t", master.next->value);
}