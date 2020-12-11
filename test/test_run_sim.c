#include <unity.h>
#include <cJSON.h>
#include "simulation_lifecycle/error.h"
#include "simulation_lifecycle/utils/file.h"
#include "simulation_lifecycle/simulation.h"

void setUp(void) {
    remove("../test/data/run_simulation/simulation_output_messages.txt");
}

void tearDown(void) {
    remove("../test/data/run_simulation/simulation_output_messages.txt");
}

void test_invalid_model_selection(void) {
    cJSON *json = NULL;

    read_json_file("../test/data/run_simulation/1_1_nonexisting_model.json", &json);
    TEST_ASSERT_EQUAL(SIM_MODEL_SELECTION_INVALID, run_sim(json));

    read_json_file("../test/data/run_simulation/1_2_nonstring_model.json", &json);
    TEST_ASSERT_EQUAL(SIM_MODEL_SELECTION_INVALID, run_sim(json));

    read_json_file("../test/data/run_simulation/1_3_missing_model.json", &json);
    TEST_ASSERT_EQUAL(SIM_MODEL_SELECTION_INVALID, run_sim(json));
}

void test_invalid_config_output_path(void) {
    cJSON *json = NULL;

    read_json_file("../test/data/run_simulation/2_1_nonexisting_config_output_path.json", &json);
    TEST_ASSERT_EQUAL(SIM_CONFIG_OUTPUT_PATH_INVALID, run_sim(json));

    read_json_file("../test/data/run_simulation/2_2_invalid_config_output_path.json", &json);
    TEST_ASSERT_EQUAL(SIM_CONFIG_OUTPUT_PATH_INVALID, run_sim(json));

    read_json_file("../test/data/run_simulation/2_3_missing_config_output_path.json", &json);
    TEST_ASSERT_EQUAL(SIM_CONFIG_OUTPUT_PATH_INVALID, run_sim(json));
}

void test_invalid_result_output_path(void) {
    cJSON *json = NULL;

    read_json_file("../test/data/run_simulation/3_1_exist_result_output_path.json", &json);
    TEST_ASSERT_EQUAL(SIM_RESULT_OUTPUT_PATH_ALREADY_EXISTS, run_sim(json));

    read_json_file("../test/data/run_simulation/3_2_invalid_result_output_path.json", &json);
    TEST_ASSERT_EQUAL(SIM_RESULT_OUTPUT_PATH_INVALID, run_sim(json));

    read_json_file("../test/data/run_simulation/3_3_missing_result_output_path.json", &json);
    TEST_ASSERT_EQUAL(SIM_RESULT_OUTPUT_PATH_INVALID, run_sim(json));
}

void test_run_sim_valid(void){
    cJSON *json = NULL;

    printf("\n Test 10 \n");
    read_json_file("../test/data/run_simulation/4_1_valid_model_valid_result_output_path.json", &json);
    TEST_ASSERT_EQUAL(SUCCESS, run_sim(json));
}

int main(void){
    UNITY_BEGIN();
    RUN_TEST(test_invalid_model_selection);
    RUN_TEST(test_invalid_config_output_path);
    RUN_TEST(test_invalid_result_output_path);
    RUN_TEST(test_run_sim_valid);
    return UNITY_END();
}
