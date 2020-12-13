#include <unity.h>
#include <stdio.h>
#include <cJSON.h>
#include "simulation_lifecycle/error.h"
#include "simulation_lifecycle/utils/file.h"
#include "simulation_lifecycle/simulation.h"

void setUp(void) {
    remove("../test/data/simulation/output/simulation/build_simulation_output.json");
}

void tearDown(void) {
    remove("../test/data/simulation/output/simulation/build_simulation_output.json");
}

void test_null_cjson(void) {
    int res = build_simulation_scenario(NULL, NULL, NULL);

    TEST_ASSERT_EQUAL(SIM_CONFIG_EMPTY, res);
}

void test_invalid_model_selection(void) {
    cJSON *json = NULL;
    char output[] = "../test/data/simulation/output";

    read_json_file("../test/data/simulation/2_1_empty_config.json", &json);
    TEST_ASSERT_EQUAL(SIM_MODEL_SELECTION_INVALID, build_simulation_scenario(json, NULL, output));

    cJSON_Delete(json);
    read_json_file("../test/data/simulation/2_2_nonstring_model.json", &json);
    TEST_ASSERT_EQUAL(SIM_MODEL_SELECTION_INVALID, build_simulation_scenario(json, NULL, output));

    cJSON_Delete(json);
    read_json_file("../test/data/simulation/2_3_nonexisting_model.json", &json);
    TEST_ASSERT_EQUAL(SIM_MODEL_SELECTION_INVALID, build_simulation_scenario(json, NULL, output));
}

void test_invalid_default_model_common_config(void) {
    cJSON *json = NULL;
    char output[] = "../test/data/simulation/output";

    read_json_file("../test/data/simulation/3_1_no_default_config_model.json", &json);
    TEST_ASSERT_EQUAL(SIM_MODEL_COMMON_CONFIG_INVALID, build_simulation_scenario(json, NULL, output));

    cJSON_Delete(json);
    read_json_file("../test/data/simulation/3_2_nonobject_default_config_model.json", &json);
    TEST_ASSERT_EQUAL(SIM_MODEL_COMMON_CONFIG_INVALID, build_simulation_scenario(json, NULL, output));

    cJSON_Delete(json);
    read_json_file("../test/data/simulation/3_3_empty_default_config_model.json", &json);
    TEST_ASSERT_EQUAL(SIM_MODEL_COMMON_CONFIG_INVALID, build_simulation_scenario(json, NULL, output));

    cJSON_Delete(json);
    read_json_file("../test/data/simulation/3_4_incomplete_default_config_model.json", &json);
    TEST_ASSERT_EQUAL(SIM_MODEL_COMMON_CONFIG_INVALID, build_simulation_scenario(json, NULL, output));

    cJSON_Delete(json);
    read_json_file("../test/data/simulation/3_5_invalid_delay_default_config_model.json", &json);
    TEST_ASSERT_EQUAL(SIM_MODEL_COMMON_CONFIG_INVALID, build_simulation_scenario(json, NULL, output));
}

void test_invalid_cells_config(void) {
    node_t *data_sources = NULL;
    cJSON *data = NULL;
    read_json_file("../test/data/DB_Ottawa_Simple.geojson", &data);

    data_source_t source = {"data", "../test/DB_Ottawa_Simple.geojson",  data};
    push_node_left(&data_sources, &source, sizeof(data_source_t));

    cJSON *json = NULL;
    char output[] = "../test/data/simulation/output";

    read_json_file("../test/data/simulation/4_1_missing_cells_config.json", &json);
    TEST_ASSERT_EQUAL(SIM_MODEL_CELLS_CONFIG_INVALID, build_simulation_scenario(json, &data_sources, output));

    cJSON_Delete(json);
    read_json_file("../test/data/simulation/4_2_nonarray_cells_config.json", &json);
    TEST_ASSERT_EQUAL(SIM_MODEL_CELLS_CONFIG_INVALID, build_simulation_scenario(json, &data_sources, output));

    cJSON_Delete(json);
    read_json_file("../test/data/simulation/4_3_empty_cells_config.json", &json);
    TEST_ASSERT_EQUAL(SIM_MODEL_CELLS_CONFIG_INVALID, build_simulation_scenario(json, &data_sources, output));

    cJSON_Delete(json);
    read_json_file("../test/data/simulation/4_3_empty_cells_config.json", &json);
    TEST_ASSERT_EQUAL(SIM_MODEL_CELLS_CONFIG_INVALID, build_simulation_scenario(json, &data_sources, output));

    cJSON_Delete(json);
    read_json_file("../test/data/simulation/4_4_cells_config_no_source.json", &json);
    TEST_ASSERT_EQUAL(SIM_MODEL_CELLS_CONFIG_INVALID, build_simulation_scenario(json, &data_sources, output));

    cJSON_Delete(json);
    read_json_file("../test/data/simulation/4_5_cells_config_invalid_source.json", &json);
    TEST_ASSERT_EQUAL(SIM_MODEL_CELLS_CONFIG_INVALID, build_simulation_scenario(json, &data_sources, output));

    cJSON_Delete(json);
    read_json_file("../test/data/simulation/4_6_cells_config_no_id_map.json", &json);
    TEST_ASSERT_EQUAL(SIM_MODEL_CELL_MAPPING_INVALID, build_simulation_scenario(json, &data_sources, output));

    cJSON_Delete(json);
    read_json_file("../test/data/simulation/4_7_cells_config_invalid_id_map.json", &json);
    TEST_ASSERT_EQUAL(SIM_MODEL_CELL_MAPPING_INVALID, build_simulation_scenario(json, &data_sources, output));

    cJSON_Delete(json);
    read_json_file("../test/data/simulation/4_8_cells_config_invalid_pop_mapping.json", &json);
    TEST_ASSERT_EQUAL(SIM_MODEL_CELL_MAPPING_INVALID, build_simulation_scenario(json, &data_sources, output));

    cJSON_Delete(json);
    cJSON_Delete(data);
    remove_list(&data_sources);
}

void test_invalid_vicinities_config(void) {
    node_t *data_sources = NULL;
    cJSON *data = NULL, *vicinities = NULL, *isolated_vicinities = NULL, *incorrect_vicinities = NULL;

    read_json_file("../test/data/DB_Ottawa_Simple.geojson", &data);
    read_json_file("../test/data/DB_Ottawa_Vicinities_Simple.geojson", &vicinities);
    read_json_file("../test/data/DB_Ottawa_Vicinities_IsolatedCell.geojson", &isolated_vicinities);
    read_json_file("../test/data/DB_Ottawa_Vicinities_IncorrectID.geojson", &incorrect_vicinities);

    data_source_t d_source = {"data", "../test/DB_Ottawa_Simple.geojson", data};
    data_source_t v_source = {"vicinities", "../test/DB_Ottawa_Vicinities_Simple.geojson", vicinities};
    data_source_t iv_source = {"incomplete_vicinities", "../test/DB_Ottawa_Vicinities_Simple.geojson", isolated_vicinities};
    data_source_t nv_source = {"incorrect_vicinities", "../test/DB_Ottawa_Vicinities_Simple.geojson", incorrect_vicinities};
    push_node_left(&data_sources, &d_source, sizeof(data_source_t));
    push_node_left(&data_sources, &v_source, sizeof(data_source_t));
    push_node_left(&data_sources, &iv_source, sizeof(data_source_t));
    push_node_left(&data_sources, &nv_source, sizeof(data_source_t));

    cJSON *json = NULL;
    char output[] = "../test/data/simulation/output";

    cJSON_Delete(json);
    read_json_file("../test/data/simulation/5_1_missing_vicinities_config.json", &json);
    TEST_ASSERT_EQUAL(SIM_MODEL_VICINITIES_CONFIG_INVALID, build_simulation_scenario(json, &data_sources, output));

    cJSON_Delete(json);
    read_json_file("../test/data/simulation/5_2_empty_vicinities_config.json", &json);
    TEST_ASSERT_EQUAL(SIM_MODEL_VICINITIES_CONFIG_INVALID, build_simulation_scenario(json, &data_sources, output));

    cJSON_Delete(json);
    read_json_file("../test/data/simulation/5_3_vicinities_config_invalid_source.json", &json);
    TEST_ASSERT_EQUAL(SIM_MODEL_VICINITIES_CONFIG_INVALID, build_simulation_scenario(json, &data_sources, output));

    cJSON_Delete(json);
    read_json_file("../test/data/simulation/5_4_vicinities_config_invalid_id_map.json", &json);
    TEST_ASSERT_EQUAL(SIM_MODEL_VICINITY_MAPPING_INVALID, build_simulation_scenario(json, &data_sources, output));

    cJSON_Delete(json);
    read_json_file("../test/data/simulation/5_5_vicinities_config_incorrect_source.json", &json);
    TEST_ASSERT_EQUAL(SIM_MODEL_VICINITY_MAPPING_INVALID, build_simulation_scenario(json, &data_sources, output));

    cJSON_Delete(json);
    read_json_file("../test/data/simulation/5_6_vicinities_config_incomplete_source.json", &json);
    TEST_ASSERT_EQUAL(SIM_MODEL_VICINITY_MAPPING_INVALID, build_simulation_scenario(json, &data_sources, output));

    cJSON_Delete(json);
    cJSON_Delete(data);
    cJSON_Delete(vicinities);
    cJSON_Delete(isolated_vicinities);
    cJSON_Delete(incorrect_vicinities);
    remove_list(&data_sources);
}

void test_invalid_output_config_path(void) {
    node_t *data_sources = NULL;
    cJSON *data = NULL, *vicinities = NULL;
    read_json_file("../test/data/DB_Ottawa_Simple.geojson", &data);
    read_json_file("../test/data/DB_Ottawa_Vicinities_Simple.geojson", &vicinities);

    data_source_t d_source = {"data", "../test/DB_Ottawa_Simple.geojson", data};
    data_source_t v_source = {"vicinities", "../test/DB_Ottawa_Vicinities_Simple.geojson", vicinities};
    push_node_left(&data_sources, &d_source, sizeof(data_source_t));
    push_node_left(&data_sources, &v_source, sizeof(data_source_t));

    cJSON *json = NULL;
    char output[] = "../test/data/simulation/output";

    read_json_file("../test/data/simulation/6_4_valid_config.json", &json);
    TEST_ASSERT_EQUAL(SIM_CONFIG_OUTPUT_PATH_INVALID, build_simulation_scenario(json, &data_sources, NULL));

    cJSON_Delete(json);
    read_json_file("../test/data/simulation/6_4_valid_config.json", &json);
    TEST_ASSERT_EQUAL(SUCCESS, build_simulation_scenario(json, &data_sources, output));

    cJSON_Delete(json);
    read_json_file("../test/data/simulation/6_4_valid_config.json", &json);
    TEST_ASSERT_EQUAL(FILE_EXISTS_ERROR, build_simulation_scenario(json, &data_sources, output));

    cJSON_Delete(json);
    cJSON_Delete(data);
    cJSON_Delete(vicinities);
    remove_list(&data_sources);
}

int main(void) {
    UNITY_BEGIN();
    RUN_TEST(test_null_cjson);
    RUN_TEST(test_invalid_model_selection);
    RUN_TEST(test_invalid_default_model_common_config);
    RUN_TEST(test_invalid_cells_config);
    RUN_TEST(test_invalid_vicinities_config);
    RUN_TEST(test_invalid_output_config_path);
    return UNITY_END();
}