#include "simulation_lifecycle/utils/linked_list.h"

Node_t * create_node(void *p_data, size_t data_size, Node_t *p_prev, Node_t *p_next) {
    /* Allocate memory for new node */
    Node_t *node = (Node_t *) malloc(sizeof(Node_t));
    node->data = malloc(data_size);
    node->prev = p_prev;
    node->next = p_next;
    /* Copy data to newly allocated memory. */
    int i;
    for (i = 0; i < data_size; i++)
        *(char *)(node->data + i) = *(char *)(p_data + i); /* Assumption: char takes 1 byte. */
    /* return newly created node */
    return node;
}

void remove_node(Node_t *p_node) {
    if (p_node != NULL) {
        /* Resolve next and prv pointers of list elements */
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

int list_length(Node_t **pp_head) {
    int i = 0;
    while (*pp_head != NULL && (*pp_head)->next != NULL) {
        i++;
    }
    return i;
}

Node_t * get_node(Node_t **pp_head, int n) {
    Node_t * current = *pp_head;
    int i;
    for (i = 0; i < n && current != NULL; i++) {
        current = current->next;
    }
    return current;
}

Node_t * get_tail(Node_t **pp_head) {
    Node_t * current;
    for (current = *pp_head; current != NULL && current->next != NULL; current = current->next)
        ;
    return current;
}

void * get_data(Node_t *p_node) {
    return (p_node == NULL) ? NULL : p_node->data;
}

void * get_head_data(Node_t **pp_head) {
    return (*pp_head == NULL) ? NULL : get_data(*pp_head);
}

void * get_tail_data(Node_t **pp_head) {
    Node_t * tail = get_tail(pp_head);
    return (tail == NULL) ? NULL : get_data(tail);
}

void * get_node_data(Node_t **pp_head, int n) {
    Node_t * current = get_node(pp_head, n);
    return (current == NULL) ? NULL : get_data(current);
}

void push(Node_t **pp_head, void *p_data, size_t data_size) {
    Node_t *tail = get_tail(pp_head);
    Node_t *new = create_node(p_data, data_size, tail, NULL);
    if (tail == NULL) {  /* If tail is null (i.e., list is empty), then list's head is the new node */
        *pp_head = new;
    } else {             /* Otherwise, former tail's next node is the new node */
        tail->next = new;
    }
}

void push_left(Node_t **pp_head, void *p_data, size_t data_size) {
    *pp_head = create_node(p_data, data_size, NULL, *pp_head);
}

void pop(Node_t **pp_head) {
    remove_node(get_tail(pp_head));
}

void pop_left(Node_t **pp_head) {
    remove_node(*pp_head);
}
