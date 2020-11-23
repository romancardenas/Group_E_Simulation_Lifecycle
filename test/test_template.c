#include <unity.h>

void setUp(void) {
    // set stuff up here
}

void tearDown(void) {
    // clean stuff up here
}

void test_null_cjson(void) {
    int a = 0;
    TEST_ASSERT_TRUE(a == 0);
}

void test_function_should_doAlsoDoBlah(void) {
    int a = 3;
    TEST_ASSERT_EQUAL(a, 3);
}

int main(void) {
    UNITY_BEGIN();
    RUN_TEST(test_null_cjson);
    RUN_TEST(test_function_should_doAlsoDoBlah);
    return UNITY_END();
}