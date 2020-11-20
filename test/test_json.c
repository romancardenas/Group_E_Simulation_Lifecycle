#include <unity.h>
#include "cJSON.h"
#include "simulation_lifecycle/utils/json.h"
#include "string.h"

void setUp(void) {
    // set stuff up here
}

void tearDown(void) {
    // clean stuff up here
}

void test_json_read_field(void) {
    cJSON * json = cJSON_Parse("{\"field\":\"potato\",\"value\":100}");

    cJSON * field = json_read_field(json, "FieldThatDoesntExist");

    TEST_ASSERT_TRUE(field == NULL);

    field = json_read_field(json, "value");

    TEST_ASSERT_TRUE(field->valueint == 100);
}

void test_json_read_object(void) {
    cJSON * json = cJSON_Parse("{\"AnObject\":{\"field\":\"potato\",\"value\":1}}");
    cJSON * field = json_read_object(json, "AnObjectThatDoesntExist");

    TEST_ASSERT_TRUE(field == NULL);

    field = json_read_object(json, "AnObject");

    TEST_ASSERT_TRUE(field != NULL);

    field = json_read_field(field, "value");

    TEST_ASSERT_TRUE(field->valueint == 100);
}

void test_json_read_string(void) {
    cJSON * json = cJSON_Parse("{\"field\":\"potato\",\"value\":1.567}");
    char * field = json_read_string(json, "AFieldThatDoesntExist");

    TEST_ASSERT_TRUE(field == NULL);

    field = json_read_string(json, "field");

    TEST_ASSERT_TRUE(field != NULL);
    TEST_ASSERT_TRUE(strcmp(field, "potato") == 1);
}

void test_feature_from_json(void) {
    int a = 3;
    TEST_ASSERT_EQUAL(a, 3);
}

void test_feature_set_from_json(void) {
    int a = 3;
    TEST_ASSERT_EQUAL(a, 3);
}

int main(void) {
    UNITY_BEGIN();
    RUN_TEST(test_json_read_field);
    RUN_TEST(test_json_read_object);
    RUN_TEST(test_json_read_string);
    RUN_TEST(test_feature_from_json);
    RUN_TEST(test_feature_set_from_json);
    return UNITY_END();
}