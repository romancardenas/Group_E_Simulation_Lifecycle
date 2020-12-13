#ifndef __SIMULATION_LIFECYCLE_LINKED_LIST_H__
#define __SIMULATION_LIFECYCLE_LINKED_LIST_H__

#include<stdlib.h>

/**
 * @brief a data structure to hold a node for a double chained list
 * @field data, a generic pointer that holds the reference to a data object for the node
 * @field prev, a pointer to the previous node of the list
 * @field next, a pointer to the next node of the list
 */
typedef struct node {
    void  *data;        /**< Pointer to linked list value. It can store any data type. */
    struct node *prev;  /**< Pointer to previous element of the linked list. */
    struct node *next;  /**< Pointer to next element of the linked list. */
} node_t;

/**
 * @brief creates a new list node. NOTE: it allocates memory on the heap.
 * @param p_data pointer to data to be stored by list node.
 * @param data_size size of the data to be stored by list node.
 * @param p_prev pointer to previous list node.
 * @param p_next pointer to next list node.
 * @return pointer to new list node.
 */
node_t * create_node(void *p_data, size_t data_size, node_t *p_prev, node_t *p_next);

/**
 * @brief Removes list node. NOTE: it frees memory of the heap.
 * @param p_node pointer to list node.
 */
void remove_node(node_t *p_node);

/**
 * @brief Returns the number of elements in the list.
 * @param pp_head pointer to the first element of the list.
 * @return number of elements in the list.
 */
int list_length(node_t **pp_head);

/**
 * @brief Returns pointer to last element of a list.
 * @param pp_head pointer to the list.
 * @return pointer to last element of the list.
 */
node_t * get_tail(node_t **pp_head);

/**
 * @brief Returns pointer to nth node of list. If length of list is less than n, then it returns NULL.
 * @param pp_head pointer to the list.
 * @param n item number to be returned.
 * @return pointer to nth node of list.
 */
node_t * get_node(node_t **pp_head, int n);

/**
 * @brief Adds new element to the end of a list.
 * @param pp_head pointer to the list.
 * @param p_data pointer to value to be added to the list.
 * @param data_size size (in bytes) of the data structure to be added to the list.
 */
void push_node(node_t **pp_head, void *p_data, size_t data_size);

/**
 * @brief Adds new element to the beginning of a list.
 * @param pp_head pointer to the list.
 * @param p_data pointer to value to be added to the list.
 * @param data_size size (in bytes) of the data structure to be added to the list.
 */
void push_node_left(node_t **pp_head, void *p_data, size_t data_size);

/**
 * @brief Removes last element of the list.
 * @param pp_head pointer to the list.
 */
void pop_node(node_t **pp_head);

/**
 * @brief Removes first element of the list.
 * @param pp_head pointer to the list.
 */
void pop_node_left(node_t **pp_head);

/**
 * @brief removes all the elements of the list.
 * @param pp_head pointer to the list.
 */
void remove_list(node_t **pp_head);

#endif //__SIMULATION_LIFECYCLE_LINKED_LIST_H__
