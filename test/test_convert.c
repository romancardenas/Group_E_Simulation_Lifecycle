#include <unity.h>
#include "simulation_lifecycle/utils/strings.h"
#include "simulation_lifecycle/convert.h"
#include "simulation_lifecycle/error.h"

void setUp(void) {
    // set stuff up here

    char *log_filename = CONVERTED_RESULTS_LOG_FILENAME;
    char *json_filename = CONVERTED_RESULTS_JSON_FILENAME;
    char *path = "../test/data/convert_tests/case_7/";

    log_filename = concat(path,log_filename);
    json_filename = concat(path,json_filename);

    remove(log_filename);
    remove(json_filename);

}

void tearDown(void) {
    // clean stuff up here

}

/**
 *  Function convert_results - Test case #1
 *  Description: Call the function with path being null.
 */
void test_function_convert_results_test_case_1(void) {
    char *results_folder_path = NULL;
    int error_code = convert_results(results_folder_path);
    TEST_ASSERT_EQUAL(CONVERT_INPUT_PATH_INCORRECT,error_code);
}

/* Function convert_results - Test case #2
 * Description: Call the function with path being empty.
 */
void test_function_convert_results_test_case_2(void) {
    char *results_folder_path = "";
    int error_code = convert_results(results_folder_path);
    TEST_ASSERT_EQUAL(CONVERT_INPUT_PATH_INCORRECT,error_code);
}

/* Function convert_results - Test case #3
 * Description: Call the function with path pointing to a file.
 */
void test_function_convert_results_test_case_3(void) {
    char *results_folder_path = "../test/data/convert_tests/case_3_and_5/co2.json";
    int error_code = convert_results(results_folder_path);
    TEST_ASSERT_EQUAL(CONVERT_INPUT_PATH_FOLDER_ERROR,error_code);
}

/* Function convert_results - Test case #4
 * Description: Call the function with path that does not exist.
 */
void test_function_convert_results_test_case_4(void) {
    char *results_folder_path = "../test/data/convert_tests/fake/";
    int error_code = convert_results(results_folder_path);
    TEST_ASSERT_EQUAL(CONVERT_INPUT_PATH_FOLDER_ERROR, error_code);
}

/* Function convert_results - Test case #5
 * Description: Call the function with path that does not only contain
 * one .json and one .txt file.
 */
void test_function_convert_results_test_case_5(void) {
    char *results_folder_path = "../test/data/convert_tests/case_3_and_5/";
    int error_code = convert_results(results_folder_path);
    TEST_ASSERT_EQUAL(CONVERT_PATH_FILES_INCORRECT,error_code);
}

/* Function convert_results - Test case #6
 * Description: Call the function with path containing only one .json and
 * one .txt file, but .txt file does not have proper format.
 * Example used: .txt file contains Cadmium DEVS format instead of
 * Cadmium Cell-DEVS format.
 */
void test_function_convert_results_test_case_6(void) {
    char *results_folder_path = "../test/data/convert_tests/case_6/";
    int error_code = convert_results(results_folder_path);
    TEST_ASSERT_EQUAL(CONVERT_FILE_FORMAT_INCORRECT,error_code);
}

/* Function convert_results - Test case #7
 * Description: Call the function with path containing only one .json and
 * one .txt file, and .txt file has proper format.
 */
void test_function_convert_results_test_case_7(void) {
    char *results_folder_path = "../test/data/convert_tests/case_7/";
    int error_code = convert_results(results_folder_path);
    TEST_ASSERT_EQUAL(SUCCESS,error_code);
}

int main(void) {
    UNITY_BEGIN();
    RUN_TEST(test_function_convert_results_test_case_1);
    RUN_TEST(test_function_convert_results_test_case_2);
    RUN_TEST(test_function_convert_results_test_case_3);
    RUN_TEST(test_function_convert_results_test_case_4);
    RUN_TEST(test_function_convert_results_test_case_5);
    RUN_TEST(test_function_convert_results_test_case_6);
    RUN_TEST(test_function_convert_results_test_case_7);
    return UNITY_END();
}