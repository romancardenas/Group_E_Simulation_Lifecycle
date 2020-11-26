#include <unity.h>
#include "cJSON.h"
#include "simulation_lifecycle/error.h"
#include "simulation_lifecycle/utils/workflow.h"

void setUp(void) {

}

void tearDown(void) {

}

void test_null_cJSON(void){
    cJSON *file = NULL;

    file = read_workflow_file("../test/data/workflow/MISSING_WORKFLOW_FILE.json");
    TEST_ASSERT_EQUAL(WORKFLOW_DOES_NOT_EXIST, validate_workflow(file));

    file = read_workflow_file("../test/data/workflow/2_empty_workflow.json");
    TEST_ASSERT_EQUAL(WORKFLOW_CONTAINS_EMPTY_JSON_OBJ, validate_workflow(file));
}

void test_workflow_data_sources(void){
    cJSON *file = NULL;

    file = read_workflow_file("../test/data/workflow/9_null_data_sources.json");
    TEST_ASSERT_EQUAL(NULL_DATA_SOURCES, validate_workflow(file));

    file = read_workflow_file("../test/data/workflow/3_missing_path.json");
    TEST_ASSERT_EQUAL(DATA_SOURCE_PATH_MISSING, validate_workflow(file));

    file = read_workflow_file("../test/data/workflow/4_missing_id.json");
    TEST_ASSERT_EQUAL(DATA_SOURCE_ID_MISSING, validate_workflow(file));

    file = read_workflow_file("../test/data/workflow/5_additional_data_source_vars.json");
    TEST_ASSERT_EQUAL(INVALID_DATA_SOURCE_ARGS, validate_workflow(file));

    file = read_workflow_file("../test/data/workflow/10_duplicate_data_id.json");
    TEST_ASSERT_EQUAL(DUPLICATE_DATA_SOURCE_ID, validate_workflow(file));

    file = read_workflow_file("../test/data/workflow/11_duplicate_data_path.json");
    TEST_ASSERT_EQUAL(DUPLICATE_DATA_SOURCE_PATH, validate_workflow(file));
}

void test_spatial_analysis(void){
    cJSON *file = NULL;

    file = read_workflow_file("../test/data/workflow/6_null_spatial_analysis.json");
    TEST_ASSERT_EQUAL(NULL_SPATIAL_ANALYSIS, validate_workflow(file));
}

void test_visualization(void){
    cJSON *file = NULL;

    file = read_workflow_file("../test/data/workflow/7_null_visualization.json");
    TEST_ASSERT_EQUAL(NULL_VISUALIZATION, validate_workflow(file));
}

void test_simulation(void){
    cJSON *file = NULL;

    file = read_workflow_file("../test/data/workflow/8_null_simulation.json");
    TEST_ASSERT_EQUAL(NULL_SIMULATION, validate_workflow(file));
}

void test_valid_workflow(void){
    cJSON *file = NULL;

    file = read_workflow_file("../test/data/workflow/1_valid_workflow.json");
    TEST_ASSERT_EQUAL(SUCCESS, validate_workflow(file));
}

int main(void){
    UNITY_BEGIN();
    RUN_TEST(test_null_cJSON);
    RUN_TEST(test_workflow_data_sources);
    RUN_TEST(test_spatial_analysis);
    RUN_TEST(test_visualization);
    RUN_TEST(test_simulation);
    RUN_TEST(test_valid_workflow);
    return UNITY_END();
}
