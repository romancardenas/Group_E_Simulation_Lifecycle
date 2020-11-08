#include <unity.h>
#include "simulation_lifecycle/utils/linked_list.h"

void setUp(void) {
    // set stuff up here
}

void tearDown(void) {
    // clean stuff up here
}

void test_int_list(void) {
    /* First we create an empty list and check that the initial preconditions are met. */
    Node_t *list = NULL;
    TEST_ASSERT_FALSE(list_length(&list));
    pop_node(&list);
    pop_node_left(&list);
    /* Push five values to the end of the list */
    int i, aux;
    for (i = 5; i < 10; i++) {
        aux = i;
        push_node(&list, (void *) &aux, sizeof(int));
        TEST_ASSERT_EQUAL(i - 5 + 1, list_length(&list));
        TEST_ASSERT_NULL(list->prev);
        TEST_ASSERT_NULL(get_tail(&list)->next);
    }
    for (i = 5; i < 10; i++) {
        TEST_ASSERT_EQUAL(i, *(int *)get_node(&list, i - 5)->data);
    }
    /* Push five values to the beginning of the list */
    for (i = 0; i < 5; i++) {
        aux = 4 - i;
        push_node_left(&list, (void *) &aux, sizeof(int));
        TEST_ASSERT_EQUAL(5 + i + 1, list_length(&list));
        TEST_ASSERT_NULL(list->prev);
        TEST_ASSERT_NULL(get_tail(&list)->next);
    }
    for (i = 0; i < 10; i++) {
        TEST_ASSERT_EQUAL(i, *(int *)get_node(&list, i)->data);
    }
    /* Add element at the middle of the list */
    Node_t *current = get_node(&list, 5);
    aux = 20;
    create_node(&aux, sizeof(int), current, current->next);
    TEST_ASSERT_EQUAL(11, list_length(&list));
    TEST_ASSERT_EQUAL(aux, *(int *)get_node(&list, 6)->data);
    TEST_ASSERT_EQUAL(current, get_node(&list, 6)->prev);
    TEST_ASSERT_EQUAL(get_node(&list, 6), current->next);
    /* Remove element at the middle of the list */
    remove_node(current->next);
    TEST_ASSERT_EQUAL(10, list_length(&list));
    TEST_ASSERT_EQUAL(current, get_node(&list, 6)->prev);
    TEST_ASSERT_EQUAL(get_node(&list, 6), current->next);
    /* Pop five values to the end of the list */
    for (i = 9; i >= 5; i--) {
        pop_node(&list);
        TEST_ASSERT_EQUAL(i, list_length(&list));
        TEST_ASSERT_EQUAL(i - 1, *(int *)get_tail(&list)->data);
        TEST_ASSERT_NULL(list->prev);
        TEST_ASSERT_NULL(get_tail(&list)->next);
    }
    for (i = 1; i < 5; i++) {
        TEST_ASSERT_EQUAL(i, *(int *)get_node(&list, i)->data);
    }
    /* Pop five values from the beginning of the list */
    for (i = 0; i < 5; i++) {
        pop_node_left(&list);
        TEST_ASSERT_EQUAL(4 - i, list_length(&list));
        if (list != NULL) {
            TEST_ASSERT_EQUAL(i + 1, *(int *)list->data);
            TEST_ASSERT_NULL(list->prev);
            TEST_ASSERT_NULL(get_tail(&list)->next);
        }
    }
    TEST_ASSERT_NULL(list);
}

int main(void) {
    UNITY_BEGIN();
    RUN_TEST(test_int_list);
    return UNITY_END();
}
