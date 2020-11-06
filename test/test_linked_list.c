#include <unity.h>
#include "simulation_lifecycle/utils/linked_list.h"

struct custom_struct {
    int val_1;
    float val_2;
};

void setUp(void) {
    // set stuff up here
}

void tearDown(void) {
    // clean stuff up here
}

void test_int_list(void) {
    Node_t *list = NULL;
    TEST_ASSERT_FALSE(list_length(&list));
    int data = 4;
    push(&list, (void *)&data, sizeof(int));
    TEST_ASSERT_EQUAL(list_length(&list), 1);
    TEST_ASSERT_EQUAL(*(int *)(list->data), data);
}

void test_str_list(void) {
    TEST_ASSERT_TRUE(3);
}

void test_custom_list(void) {
    TEST_ASSERT_TRUE(3);
}

int main(void) {
    UNITY_BEGIN();
    RUN_TEST(test_int_list);
    RUN_TEST(test_str_list);
    RUN_TEST(test_custom_list);
    return UNITY_END();
}
