#include <unity.h>
#include <cJSON.h>
#include "simulation_lifecycle/error.h"
#include "simulation_lifecycle/utils/linked_list.h"
#include "simulation_lifecycle/utils/workflow.h"
#include "simulation_lifecycle/spatial_analysis.h"

extern node_t * registered_operations;

void setUp(void) {
    // set stuff up here
}

void tearDown(void) {
    // clean stuff up here
}

int execute_success(char * id, node_t * data_sources, cJSON * parameters, node_t ** results) {
    return SUCCESS;
}

int execute_fail(char * id, node_t * data_sources, cJSON * parameters, node_t ** results) {
    return 1;
}

void test_register_operations(void) {
    // nothing to test, all void. Just run the function to make sure it doesn't crash, validate that
    // there are more than 0 operations registered
    register_operations();

    TEST_ASSERT(list_length(&registered_operations) > 0);
}

void test_register_operation(void) {
    int n = list_length(&registered_operations);

    register_operation("success", execute_success);
    register_operation("execute_fail", execute_fail);

    // Nothing much to test, these functions aren't meant to be called using user provided info so
    // they don't have to return error codes. It's up to to the programmer to make sure it works.
    // We can still test that there are more operations registered though.

    TEST_ASSERT(list_length(&registered_operations) == n + 2);
}

void test_get_operation(void) {
    operation_t * op_1 = get_operation("success");
    operation_t * op_2 = get_operation("non-existent");

    TEST_ASSERT(op_1 != NULL);
    TEST_ASSERT(op_2 == NULL);
}

void test_read_data_in(void) {
    cJSON * wf = NULL;

    // Test that read_data_in receives proper errors from  read_json function used internally
    node_t * data_sources = NULL;
    read_json_file("../test/data/workflow/12_bad_path.json", &wf);
    int res = read_data_in(wf, &data_sources);
    TEST_ASSERT_TRUE(res == FILE_DOES_NOT_EXIST);

    data_sources = NULL;
    read_json_file("../test/data/workflow/13_missing_data_id.json", &wf);
    res = read_data_in(wf, &data_sources);
    TEST_ASSERT_TRUE(res == DATA_SOURCE_ID_NULL);

    data_sources = NULL;
    read_json_file("../test/data/workflow/14_missing_data_path.json", &wf);
    res = read_data_in(wf, &data_sources);
    TEST_ASSERT_TRUE(res == DATA_SOURCE_PATH_NULL);

    data_sources = NULL;
    read_json_file("../test/data/workflow/1_valid_workflow.json", &wf);
    res = read_data_in(wf, &data_sources);
    TEST_ASSERT_TRUE(res == SUCCESS);
}

void test_execute_workflow(void) {
    cJSON * wf = NULL;
    node_t * results = NULL;

    read_json_file("..\\test\\data\\workflow\\1_valid_workflow.json", &wf);
    int res = execute_workflow(wf, &results);
    TEST_ASSERT_TRUE(res == SUCCESS);

    // Get an operation from the json just so I can modify fields to generate errors.
    cJSON * operation = read_spatial_analysis(wf)->child->next;

    char * name = cJSON_GetStringValue(cJSON_GetObjectItem(operation, "operation"));
    cJSON * params = cJSON_GetObjectItem(operation, "parameters");
    int type = cJSON_GetObjectItem(operation, "parameters")->type;

    results = NULL;
    cJSON_GetObjectItem(operation, "operation")->valuestring = NULL;
    res = execute_workflow(wf, &results);
    TEST_ASSERT_TRUE(res == OPERATION_NAME_NULL);
    cJSON_GetObjectItem(operation, "operation")->valuestring = name;

    results = NULL;
    cJSON_GetObjectItem(operation, "parameters")->child = NULL;
    cJSON_GetObjectItem(operation, "parameters")->type = 4;
    res = execute_workflow(wf, &results);

    TEST_ASSERT_TRUE(res == OPERATION_NO_PARAMETERS);
    cJSON_GetObjectItem(operation, "parameters")->child = params;
    cJSON_GetObjectItem(operation, "parameters")->type = type;

    results = NULL;
    cJSON_SetValuestring(cJSON_GetObjectItem(operation, "operation"), "non-existent");
    res = execute_workflow(wf, &results);
    TEST_ASSERT_TRUE(res == OPERATION_UNREGISTERED);

    results = NULL;
    cJSON_SetValuestring(cJSON_GetObjectItem(operation, "operation"), "execute_fail");
    res = execute_workflow(wf, &results);
    TEST_ASSERT_TRUE(res != SUCCESS);
}

int main(void) {
    UNITY_BEGIN();
    RUN_TEST(test_register_operations);
    RUN_TEST(test_register_operation);
    RUN_TEST(test_get_operation);
    RUN_TEST(test_read_data_in); // TODO this test fails on Ubuntu
    RUN_TEST(test_execute_workflow); // TODO this test fails on Ubuntu
    return UNITY_END();
}