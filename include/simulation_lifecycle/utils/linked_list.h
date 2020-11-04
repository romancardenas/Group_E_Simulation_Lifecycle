#ifndef SIMULATION_LIFECYCLE_LINKED_LIST_HPP
#define SIMULATION_LIFECYCLE_LINKED_LIST_HPP

#include<stdlib.h>

typedef struct Node {
    void  *data;        /**< Pointer to linked list value. It can store any data type. */
    struct Node *prev;  /**< Pointer to previous element of the linked list. */
    struct Node *next;  /**< Pointer to next element of the linked list. */
} Node_t;

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
Node_t * list_tail(Node_t **pp_head);

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
