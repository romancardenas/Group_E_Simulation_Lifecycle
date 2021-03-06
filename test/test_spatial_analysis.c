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
    return -99;
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

    TEST_ASSERT_EQUAL(n + 2, list_length(&registered_operations));
}

void test_get_operation(void) {
    operation_t * op_1 = get_operation("success");
    operation_t * op_2 = get_operation("non-existent");

    TEST_ASSERT_NOT_NULL(op_1);
    TEST_ASSERT_NULL(op_2);
}

void test_read_data_in(void) {
    // Test that read_data_in receives proper errors from  read_json function used internally
    cJSON * wf = NULL;
    read_json_file("../test/data/workflow/12_bad_path.json", &wf);

    node_t * data_sources = NULL;
    int res = read_data_in(wf, &data_sources);
    TEST_ASSERT_EQUAL(FILE_DOES_NOT_EXIST, res);

    remove_list(&data_sources);
    read_json_file("../test/data/workflow/13_missing_data_id.json", &wf);
    res = read_data_in(wf, &data_sources);
    TEST_ASSERT_EQUAL(DATA_SOURCE_ID_NULL, res);

    remove_list(&data_sources);
    read_json_file("../test/data/workflow/14_missing_data_path.json", &wf);
    res = read_data_in(wf, &data_sources);
    TEST_ASSERT_EQUAL(DATA_SOURCE_PATH_NULL, res);

    remove_list(&data_sources);
    read_json_file("../test/data/workflow/1_valid_workflow.json", &wf);
    res = read_data_in(wf, &data_sources);
    TEST_ASSERT_EQUAL(SUCCESS, res);
}

void test_execute_workflow(void) {
    cJSON * wf = NULL;
    read_json_file("../test/data/workflow/1_valid_workflow.json", &wf);

    node_t * data_sources = NULL;
    read_data_in(wf, &data_sources);

    int res = execute_workflow(wf, &data_sources);
    TEST_ASSERT_EQUAL(SUCCESS, res);

    // Get an operation from the json just so I can modify fields to generate errors.
    cJSON * operation = read_spatial_analysis(wf)->child->next;

    char * name = cJSON_GetStringValue(cJSON_GetObjectItem(operation, "operation"));
    cJSON * params = cJSON_GetObjectItem(operation, "parameters");
    int type = cJSON_GetObjectItem(operation, "parameters")->type;

    remove_list(&data_sources);
    read_data_in(wf, &data_sources);
    cJSON_GetObjectItem(operation, "operation")->valuestring = NULL;
    res = execute_workflow(wf, &data_sources);
    TEST_ASSERT_EQUAL(OPERATION_NAME_NULL, res);
    cJSON_GetObjectItem(operation, "operation")->valuestring = name;

    remove_list(&data_sources);
    read_data_in(wf, &data_sources);
    cJSON_GetObjectItem(operation, "parameters")->child = NULL;
    cJSON_GetObjectItem(operation, "parameters")->type = 4;
    res = execute_workflow(wf, &data_sources);
    TEST_ASSERT_EQUAL(OPERATION_NO_PARAMETERS, res);
    cJSON_GetObjectItem(operation, "parameters")->child = params;
    cJSON_GetObjectItem(operation, "parameters")->type = type;

    remove_list(&data_sources);
    read_data_in(wf, &data_sources);
    cJSON_SetValuestring(cJSON_GetObjectItem(operation, "operation"), "non-existent");
    res = execute_workflow(wf, &data_sources);
    TEST_ASSERT_EQUAL(OPERATION_UNREGISTERED, res);

    remove_list(&data_sources);
    read_data_in(wf, &data_sources);
    cJSON_SetValuestring(cJSON_GetObjectItem(operation, "operation"), "execute_fail");
    res = execute_workflow(wf, &data_sources);
    TEST_ASSERT_EQUAL(-99, res);
}

int main(void) {
    UNITY_BEGIN();
    RUN_TEST(test_register_operations);
    RUN_TEST(test_register_operation);
    RUN_TEST(test_get_operation);
    RUN_TEST(test_read_data_in);
    RUN_TEST(test_execute_workflow);
    return UNITY_END();
}
