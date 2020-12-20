#include "simulation_lifecycle/utils/linked_list.h"

node_t * create_node(void *p_data, size_t data_size, node_t *p_prev, node_t *p_next) {
    /* Allocate memory for new node */
    node_t *node = (node_t *) malloc(sizeof(node_t));
    node->data = malloc(data_size);
    node->prev = p_prev;
    node->next = p_next;
    /* Copy data to newly allocated memory. */
    int i;
    for (i = 0; i < data_size; i++) {
        *(char *) (node->data + i) = *(char *) (p_data + i); /* Assumption: char takes 1 byte. */
    }
    /* Resolve next and prev pointers of list elements */
    if (p_prev != NULL) {
        p_prev->next = node;
    }
    if (p_next != NULL) {
        p_next->prev = node;
    }
    /* return newly created node */
    return node;
}

void remove_node(node_t *p_node) {
    if (p_node != NULL) {
        /* Resolve next and prev pointers of list elements */
        if (p_node->prev != NULL) {
            p_node->prev->next = p_node->next;
        }
        if (p_node->next != NULL) {
            p_node->next->prev = p_node->prev;
        }
        /* Free memory of the node */
        free(p_node->data);
        free((void *)p_node);
    }
}

int list_length(node_t **pp_head) {
    int i = 0;
    node_t *current = *pp_head;
    while (current != NULL) {
        i++;
        current = current->next;
    }
    return i;
}

node_t * get_node(node_t **pp_head, int n) {
    node_t * current = *pp_head;
    int i;
    for (i = 0; i < n && current != NULL; i++) {
        current = current->next;
    }
    return current;
}

node_t * get_tail(node_t **pp_head) {
    node_t * current;
    for (current = *pp_head; current != NULL && current->next != NULL; current = current->next) {
    }
    return current;
}

void push_node(node_t **pp_head, void *p_data, size_t data_size) {
    node_t *tail = get_tail(pp_head);
    node_t *new = create_node(p_data, data_size, tail, NULL);
    if (tail == NULL) {  /* If tail is null (i.e., list is empty), then list's head is the new node */
        *pp_head = new;
    }
}

void push_node_left(node_t **pp_head, void *p_data, size_t data_size) {
    *pp_head = create_node(p_data, data_size, NULL, *pp_head);
}

void pop_node(node_t **pp_head) {
    remove_node(get_tail(pp_head));
}

void pop_node_left(node_t **pp_head) {
    if (*pp_head != NULL) {
        node_t * prev = *pp_head;
        *pp_head = prev->next;
        remove_node(prev);
    }
}

void remove_list(node_t **pp_head) {
    while(*pp_head != NULL) {
        pop_node_left(pp_head);
    }
}
