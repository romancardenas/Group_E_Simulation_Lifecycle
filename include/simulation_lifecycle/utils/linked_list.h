#ifndef SIMULATION_LIFECYCLE_LINKED_LIST_HPP
#define SIMULATION_LIFECYCLE_LINKED_LIST_HPP

#include<stdlib.h>

typedef struct Node {
    void  *data;        /**< Pointer to linked list value. It can store any data type. */
    struct Node *prev;  /**< Pointer to previous element of the linked list. */
    struct Node *next;  /**< Pointer to next element of the linked list. */
} Node_t;

/**
 * @brief creates a new list node. NOTE: it allocates memory on the heap.
 * @param p_data pointer to data to be stored by list node.
 * @param data_size size of the data to be stored by list node.
 * @param p_prev pointer to previous list node.
 * @param p_next pointer to next list node.
 * @return pointer to new list node.
 */
Node_t * create_node(void *p_data, size_t data_size, Node_t *p_prev, Node_t *p_next);

/**
 * @brief Removes list node. NOTE: it frees memory of the heap.
 * @param p_node pointer to list node.
 */
void remove_node(Node_t *p_node);

/**
 * @brief Returns the number of elements in the list.
 * @param pp_head pointer to the first element of the list.
 * @return number of elements in the list.
 */
int list_length(Node_t **pp_head);

/**
 * @brief Returns pointer to last element of a list.
 * @param pp_head pointer to the list.
 * @return pointer to last element of the list.
 */
Node_t * get_tail(Node_t **pp_head);

/**
 * @brief Returns pointer to nth node of list. If length of list is less than n, then it returns NULL.
 * @param pp_head pointer to the list.
 * @param n item number to be returned.
 * @return pointer to nth node of list.
 */
Node_t * get_node(Node_t **pp_head, int n);

/**
 * @brief returns data of a list node.
 * @param p_node pointer to list node.
 * @return pointer to value of the list node.
 */
void * get_data(Node_t *p_node);

/**
 * @brief Returns data of the first element of list.
 * @param pp_head pointer to the list.
 * @return pointer to the value of the list node.
 */
void * get_head_data(Node_t **pp_head);

/**
 * Returns data of the last element of the list.
 * @param pp_head pointer to the list.
 * @return pointer to the value of the last node of the list.
 */
void * get_tail_data(Node_t **pp_head);

/**
 * @brief returns pointer to data of the nth element of the list. If list has less than n elements, it returns NULL.
 * @param pp_head pointer to list.
 * @param n list node index.
 * @return pointer to data of the nth element of the list.
 */
void * get_node_data(Node_t **pp_head, int n);

/**
 * @brief Adds new element to the end of a list.
 * @param pp_head pointer to the list.
 * @param p_data pointer to value to be added to the list.
 * @param data_size size (in bytes) of the data structure to be added to the list.
 */
void push(Node_t **pp_head, void *p_data, size_t data_size);

/**
 * @brief Adds new element to the beginning of a list.
 * @param pp_head pointer to the list.
 * @param p_data pointer to value to be added to the list.
 * @param data_size size (in bytes) of the data structure to be added to the list.
 */
void push_left(Node_t **pp_head, void *p_data, size_t data_size);

/**
 * @brief Removes last element of the list.
 * @param pp_head pointer to the list.
 */
void pop(Node_t **pp_head);

/**
 * @brief Removes first element of the list.
 * @param pp_head pointer to the list.
 */
void pop_left(Node_t **pp_head);

#endif //SIMULATION_LIFECYCLE_LINKED_LIST_HPP
