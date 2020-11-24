#include <unity.h>
#include <stdio.h>
#include "cJSON.h"
#include "simulation_lifecycle/error.h"
#include "simulation_lifecycle/utils/file.h"
#include "simulation_lifecycle/simulation.h"

void setUp(void) {
    remove("../test/data/simulation/output_config.json");
}

void tearDown(void) {
    // clean stuff up here
}

void test_null_cjson(void) {
    int res = build_simulation_scenario(NULL, NULL, NULL);
    TEST_ASSERT_EQUAL(SIM_CONFIG_EMPTY, res);
}

void test_invalid_model_selection(void) {
    cJSON *json = NULL;

    read_json_file("../test/data/simulation/2_1_empty_config.json", &json);
    TEST_ASSERT_EQUAL(SIM_MODEL_SELECTION_INVALID, build_simulation_scenario(json, NULL, NULL));

    read_json_file("../test/data/simulation/2_2_nonstring_model.json", &json);
    TEST_ASSERT_EQUAL(SIM_MODEL_SELECTION_INVALID, build_simulation_scenario(json, NULL, NULL));

    read_json_file("../test/data/simulation/2_3_nonexisting_model.json", &json);
    TEST_ASSERT_EQUAL(SIM_MODEL_SELECTION_INVALID, build_simulation_scenario(json, NULL, NULL));
}

void test_invalid_default_model_common_config(void) {
    cJSON *json = NULL;

    read_json_file("../test/data/simulation/3_1_no_default_config_model.json", &json);
    TEST_ASSERT_EQUAL(SIM_MODEL_COMMON_CONFIG_INVALID, build_simulation_scenario(json, NULL, NULL));

    read_json_file("../test/data/simulation/3_2_nonobject_default_config_model.json", &json);
    TEST_ASSERT_EQUAL(SIM_MODEL_COMMON_CONFIG_INVALID, build_simulation_scenario(json, NULL, NULL));

    read_json_file("../test/data/simulation/3_3_empty_default_config_model.json", &json);
    TEST_ASSERT_EQUAL(SIM_MODEL_COMMON_CONFIG_INVALID, build_simulation_scenario(json, NULL, NULL));

    read_json_file("../test/data/simulation/3_4_incomplete_default_config_model.json", &json);
    TEST_ASSERT_EQUAL(SIM_MODEL_COMMON_CONFIG_INVALID, build_simulation_scenario(json, NULL, NULL));

    read_json_file("../test/data/simulation/3_5_invalid_delay_default_config_model.json", &json);
    TEST_ASSERT_EQUAL(SIM_MODEL_COMMON_CONFIG_INVALID, build_simulation_scenario(json, NULL, NULL));
}

void test_invalid_default_model_specific_config(void) {
    cJSON *json = NULL;

    read_json_file("../test/data/simulation/4_1_empty_specific_config_model.json", &json);
    TEST_ASSERT_EQUAL(SIM_MODEL_SPECIFIC_CONFIG_INVALID, build_simulation_scenario(json, NULL, NULL));

    read_json_file("../test/data/simulation/4_2_invalid_specific_config_params_model.json", &json);
    TEST_ASSERT_EQUAL(SIM_MODEL_SPECIFIC_CONFIG_INVALID, build_simulation_scenario(json, NULL, NULL));

    read_json_file("../test/data/simulation/4_3_missing_specific_config_params_model.json", &json);
    TEST_ASSERT_EQUAL(SIM_MODEL_SPECIFIC_CONFIG_INVALID, build_simulation_scenario(json, NULL, NULL));
}

void test_invalid_output_config_path(void) {
    cJSON *json = NULL;

    read_json_file("../test/data/simulation/5_1_missing_output_config_path.json", &json);
    TEST_ASSERT_EQUAL(SIM_CONFIG_OUTPUT_PATH_INVALID, build_simulation_scenario(json, NULL, NULL));

    read_json_file("../test/data/simulation/5_2_invalid_output_config_path.json", &json);
    TEST_ASSERT_EQUAL(SIM_CONFIG_OUTPUT_PATH_INVALID, build_simulation_scenario(json, NULL, NULL));

    read_json_file("../test/data/simulation/5_3_existing_output_config_path.json", &json);
    TEST_ASSERT_EQUAL(FILE_EXISTS_ERROR, build_simulation_scenario(json, NULL, NULL));

    read_json_file("../test/data/simulation/5_4_valid_output_config_path.json", &json);
    TEST_ASSERT_EQUAL(SUCCESS, build_simulation_scenario(json, NULL, NULL));
}

int main(void) {
    UNITY_BEGIN();
    RUN_TEST(test_null_cjson);
    RUN_TEST(test_invalid_model_selection);
    RUN_TEST(test_invalid_default_model_common_config);
    RUN_TEST(test_invalid_default_model_specific_config);
    RUN_TEST(test_invalid_output_config_path);
    return UNITY_END();
}