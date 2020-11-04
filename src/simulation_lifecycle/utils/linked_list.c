#include "simulation_lifecycle/utils/linked_list.h"

/**
 * @brief creates a new list node.
 * @param p_data pointer to data to be stored in node.
 * @param data_size size (in bits) of the data of the node. It is required to allocating the correct amount of memory.
 * @param p_prev pointer to previous node of the list.
 * @param p_next pointer to next node of the list.
 * @return pointer to new node.
 */
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

/**
 * @brief removes a list node.
 * @param p_node pointer to node.
 */
void remove_node(Node_t *p_node) {
    if (p_node != NULL) {
        if (p_node->prev != NULL) {
            p_node->prev->next = p_node->next;
        }
        if (p_node->next != NULL) {
            p_node->next->prev = p_node->prev;
        }
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

Node_t * list_tail(Node_t **pp_head) {
    Node_t * current;
    for (current = *pp_head; current != NULL && current->next != NULL; current = current->next)
        ;
    return current;
}

void push(Node_t **pp_head, void *p_data, size_t data_size) {
    Node_t *tail = list_tail(pp_head);
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
    remove_node(list_tail(pp_head));
}

void pop_left(Node_t **pp_head) {
    remove_node(*pp_head);
}
