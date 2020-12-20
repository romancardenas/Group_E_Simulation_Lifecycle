#include <unity.h>
#include "simulation_lifecycle/utils/strings.h"
#include "simulation_lifecycle/utils/file.h"
#include "simulation_lifecycle/convert.h"
#include "simulation_lifecycle/error.h"
#include "simulation_lifecycle/utils/workflow.h"

void setUp(void) {
    char *log_filename = CONVERTED_RESULTS_LOG_FILENAME;
    char *json_filename = CONVERTED_RESULTS_JSON_FILENAME;
    char *path = "../test/data/convert_tests/case_3/";

    log_filename = concat(path,log_filename);
    json_filename = concat(path,json_filename);

    remove(log_filename);
    remove(json_filename);
}

void tearDown(void) {
    /*
    char *log_filename = CONVERTED_RESULTS_LOG_FILENAME;
    char *json_filename = CONVERTED_RESULTS_JSON_FILENAME;
    char *path = "../test/data/convert_tests/case_3/";

    log_filename = concat(path,log_filename);
    json_filename = concat(path,json_filename);

    remove(log_filename);
    remove(json_filename);
     */
}

/* Test description: Call the function with path being null. */
void test_function_convert_results_null_path(void) {
    char workflow_path[] = "../test/data/convert_tests/valid_viz_fields_workflow.json";
    char * results_folder_path = NULL;

    cJSON * workflow = NULL;
    read_json_file(workflow_path,&workflow);
    cJSON *visualization = read_visualization(workflow);

    int error_code = convert_results(results_folder_path, visualization);
    TEST_ASSERT_EQUAL(CONVERT_OUTPUT_PATH_INCORRECT,error_code);
}

/* Test description: Call the function with path being empty.*/
void test_function_convert_results_empty_path(void) {
    char workflow_path[]  = "../test/data/convert_tests/valid_viz_fields_workflow.json";
    char results_folder_path[]  = "";

    cJSON * workflow = NULL;
    read_json_file(workflow_path,&workflow);
    cJSON *visualization = read_visualization(workflow);

    int error_code = convert_results(results_folder_path, visualization);
    TEST_ASSERT_EQUAL(CONVERT_OUTPUT_PATH_INCORRECT,error_code);
}

/* Test description: Call the function with path pointing to a file.*/
void test_function_convert_results_not_directory(void) {
    char workflow_path[] = "../test/data/convert_tests/valid_viz_fields_workflow.json";
    char results_folder_path[]  = "../test/data/convert_tests/case_1/config.json";;

    cJSON * workflow = NULL;
    read_json_file(workflow_path,&workflow);
    cJSON *visualization = read_visualization(workflow);

    int error_code = convert_results(results_folder_path, visualization);
    TEST_ASSERT_EQUAL(CONVERT_INPUT_PATH_INCORRECT,error_code);
}

/* Test description: Call the function with path that does not exist.*/
void test_function_convert_results_nonexistent_path(void) {
    char workflow_path[] = "../test/data/convert_tests/valid_viz_fields_workflow.json";
    char results_folder_path[] =  "../test/data/convert_tests/invalid/";

    cJSON * workflow = NULL;
    read_json_file(workflow_path,&workflow);
    cJSON *visualization = read_visualization(workflow);

    int error_code = convert_results(results_folder_path, visualization);
    TEST_ASSERT_EQUAL(CONVERT_INPUT_PATH_INCORRECT,error_code);
}

/* Test description: Call the function with path that does not only contain
 * one .json and one .txt file. */
void test_function_convert_results_incorrect_number_files(void) {
    char workflow_path[] = "../test/data/convert_tests/valid_viz_fields_workflow.json";
    char results_folder_path[] = "../test/data/convert_tests/case_1/";

    cJSON * workflow = NULL;
    read_json_file(workflow_path,&workflow);
    cJSON *visualization = read_visualization(workflow);

    int error_code = convert_results(results_folder_path, visualization);
    TEST_ASSERT_EQUAL(CONVERT_PATH_FILES_INCORRECT,error_code);
}

/* Test description: Call the function with empty visualization fields
 * in the workflow.json. */
void test_function_convert_results_empty_viz_fields(void) {
    char workflow_path[] = "../test/data/convert_tests/empty_viz_fields_workflow.json";
    char results_folder_path[] = "../test/data/convert_tests/case_3/";

    cJSON * workflow = NULL;
    read_json_file(workflow_path,&workflow);
    cJSON *visualization = read_visualization(workflow);

    int error_code = convert_results(results_folder_path, visualization);
    TEST_ASSERT_EQUAL(CONVERT_VIZ_FIELDS_INVALID,error_code);
}

/* Test description: Call the function with missing visualization fields
 * in the workflow.json. */
void test_function_convert_results_missing_viz_fields(void) {
    char workflow_path[] = "../test/data/convert_tests/missing_viz_fields_workflow.json";
    char results_folder_path[] = "../test/data/convert_tests/case_3/";

    cJSON * workflow = NULL;
    read_json_file(workflow_path,&workflow);
    cJSON *visualization = read_visualization(workflow);

    int error_code = convert_results(results_folder_path, visualization);
    TEST_ASSERT_EQUAL(CONVERT_VIZ_FIELDS_INVALID,error_code);
}

/* Test description: Call the function with proper visualization JSON structure and
 * with result path containing only one .json and one .txt file with valid format.
 */
void test_function_convert_results_valid(void) {
    char workflow_path[] = "../test/data/convert_tests/valid_viz_fields_workflow.json";
    char results_folder_path[] = "../test/data/convert_tests/case_3/";

    cJSON * workflow = NULL;
    read_json_file(workflow_path,&workflow);
    cJSON *visualization = read_visualization(workflow);

    int error_code = convert_results(results_folder_path, visualization);
    TEST_ASSERT_EQUAL(SUCCESS,error_code);
}

int main(void) {
    UNITY_BEGIN();
    RUN_TEST(test_function_convert_results_null_path);
    RUN_TEST(test_function_convert_results_empty_path);
    RUN_TEST(test_function_convert_results_not_directory);
    RUN_TEST(test_function_convert_results_nonexistent_path);
    RUN_TEST(test_function_convert_results_incorrect_number_files);
    // RUN_TEST(test_function_convert_results_invalid_format);
    RUN_TEST(test_function_convert_results_empty_viz_fields);
    RUN_TEST(test_function_convert_results_missing_viz_fields);
    RUN_TEST(test_function_convert_results_valid);
    return UNITY_END();
}