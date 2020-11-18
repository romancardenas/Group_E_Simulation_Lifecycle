#include <unity.h>
#include "simulation_lifecycle/convert.h"
#include "simulation_lifecycle/error.h"

void setUp(void) {
    // set stuff up here
}

void tearDown(void) {
    // clean stuff up here
}

/**
 *  Function convert_results - Test case #1
 *  Description: Call the function with path being null.
 *  Expected output: The function returns 340.
 */
void test_function_convert_results_test_case_1(void) {
    char *results_folder_path = NULL;
    int error_code = convert_results(results_folder_path);
    TEST_ASSERT_EQUAL(error_code, INPUT_PATH_EMPTY);
}

/* Function convert_results - Test case #2
 * Description: Call the function with path being empty.
 * Expected output: The function returns 340.
 */
void test_function_convert_results_test_case_2(void) {
    char *results_folder_path = "\0";
    int error_code = convert_results(results_folder_path);
    TEST_ASSERT_EQUAL(error_code, INPUT_PATH_EMPTY);
}

/* Function convert_results - Test case #3
 * Description: Call the function with path pointing to a file.
 * Expected output: The function returns 341.
 */
void test_function_convert_results_test_case_3(void) {
    char *results_folder_path = "../test/data/convert_tests/case_3_and_5/co2.json";
    int error_code = convert_results(results_folder_path);
    TEST_ASSERT_EQUAL(error_code, INPUT_PATH_FOLDER_ERROR);
}

/* Function convert_results - Test case #4
 * Description: Call the function with path that does not exist.
 * Expected output: The function returns 341.
 */
void test_function_convert_results_test_case_4(void) {
    char *results_folder_path = "../test/data/convert_tests/fake/";
    int error_code = convert_results(results_folder_path);
    TEST_ASSERT_EQUAL(error_code, INPUT_PATH_FOLDER_ERROR);
}

/* Function convert_results - Test case #5
 * Description: Call the function with path that does not only contain
 * one .json and one .txt file.
 * Expected output: The function returns 352.
 */
void test_function_convert_results_test_case_5(void) {
    char *results_folder_path = "../test/data/convert_tests/case_3_and_5/";
    int error_code = convert_results(results_folder_path);
    TEST_ASSERT_EQUAL(error_code, CONVERSION_PATH_FILES_INCORRECT);
}

/* Function convert_results - Test case #6
 * Description: Call the function with path containing only one .json and
 * one .txt file, but .txt file does not have proper format.
 * Exemple used: .txt file contains Cadmium DEVS format instead of
 * Cadmium Cell-DEVS format.
 * Expected output: The function returns 350.
 */
void test_function_convert_results_test_case_6(void) {
    char *results_folder_path = "../test/data/convert_tests/case_6/";
    int error_code = convert_results(results_folder_path);
    TEST_ASSERT_EQUAL(error_code, FILE_FORMAT_INCORRECT);
}

/* Function convert_results - Test case #7
 * Description: Call the function with path containing only one .json and
 * one .txt file, and .txt file has proper format.
 * Expected output: The function returns 0 and path contains the
 * converted files.
 */
void test_function_convert_results_test_case_7(void) {
    char *results_folder_path = "../test/data/convert_tests/case_7/";
    int error_code = convert_results(results_folder_path);
    TEST_ASSERT_EQUAL(error_code, SUCCESS);
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