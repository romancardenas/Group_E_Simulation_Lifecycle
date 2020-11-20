#include <unity.h>

void setUp(void) {
    // set stuff up here
}

void tearDown(void) {
    // clean stuff up here
}

void test_write_data_to_file(void) {
    int a = 0;
    TEST_ASSERT_TRUE(a == 0);
}

void test_read_file(void) {
    int a = 3;
    TEST_ASSERT_EQUAL(a, 3);
}

void test_read_json_file(void) {
    int a = 0;
    TEST_ASSERT_TRUE(a == 0);
}

void test_read_geojson_file(void) {
    int a = 3;
    TEST_ASSERT_EQUAL(a, 3);
}

int main(void) {
    UNITY_BEGIN();
    RUN_TEST(test_write_data_to_file);
    RUN_TEST(test_read_file);
    RUN_TEST(test_read_json_file);
    RUN_TEST(test_read_geojson_file);
    return UNITY_END();
}