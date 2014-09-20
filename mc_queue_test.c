#include <stdio.h>
#include "mc_queue.h"

typedef struct {
    u_char *str;
    mc_queue_t q_ele;
    int num;
} test_node;

int cmp_test_node(const mc_queue_t *a, const mc_queue_t *b) {
    test_node *a_node = mc_queue_data(a, test_node, q_ele);
    test_node *b_node = mc_queue_data(b, test_node, q_ele);

    return a_node->num > b_node->num;
}

int
main(void) {
    mc_queue_t queue_container;
    mc_queue_t *q;
    int i = 0;
    test_node node[5];

    mc_queue_init(&queue_container);

    for (i = 0; i < 5; i++)
    {
        node[i].num = i;
    }

    mc_queue_insert_tail(&queue_container, &node[0].q_ele);
    mc_queue_insert_head(&queue_container, &node[1].q_ele);
    mc_queue_insert_tail(&queue_container, &node[2].q_ele);
    mc_queue_insert_after(&queue_container, &node[3].q_ele);
    mc_queue_insert_tail(&queue_container, &node[4].q_ele);

    for (q = mc_queue_head(&queue_container);
            q != mc_queue_sentinel(&queue_container);
            q = mc_queue_next(q))
    {
        test_node *ele_node = mc_queue_data(q, test_node, q_ele);
        printf("%d\n", ele_node->num);
    }

    mc_queue_sort(&queue_container, cmp_test_node);
    printf("==============after sorting=====================\n");
    for (q = mc_queue_head(&queue_container);
            q != mc_queue_sentinel(&queue_container);
            q = mc_queue_next(q))
    {
        test_node *ele_node = mc_queue_data(q, test_node, q_ele);
        printf("%d\n", ele_node->num);
    }

    return 0;
}
