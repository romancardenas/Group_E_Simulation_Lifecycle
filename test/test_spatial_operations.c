#include <unity.h>
#include <cJSON.h>
#include "simulation_lifecycle/error.h"
#include "simulation_lifecycle/utils/linked_list.h"
#include "simulation_lifecycle/utils/workflow.h"
#include "simulation_lifecycle/spatial_analysis.h"

void setUp(void) {
    // set stuff up here
}

void tearDown(void) {
    // clean stuff up here
}

void test_centroids_invalid_data_sources(void) {

    cJSON * workflow = NULL;
    node_t * data_sources = NULL;

    read_json_file("../test/data/spatial_operations/1_missing_data_source_workflow.json", &workflow);
    read_data_in(workflow, &data_sources);

    cJSON * operation = read_spatial_analysis(workflow)->child->next;
    cJSON * params = cJSON_GetObjectItem(operation, "parameters");

    TEST_ASSERT_EQUAL(DATA_SOURCE_ID_NULL, centroids_execute("db_ottawa", &data_sources, params));
}

void test_centroids_existing_data_sources(void) {

    cJSON * workflow = NULL;
    node_t * data_sources = NULL;

    read_json_file("../test/data/spatial_operations/2_existing_data_source_workflow.json", &workflow);
    read_data_in(workflow, &data_sources);

    cJSON * operation = read_spatial_analysis(workflow)->child->next;
    cJSON * params = cJSON_GetObjectItem(operation, "parameters");

    TEST_ASSERT_EQUAL(SUCCESS, centroids_execute("db_ottawa", &data_sources, params));
}

int main(void) {
    UNITY_BEGIN();
    RUN_TEST(test_centroids_invalid_data_sources);
    RUN_TEST(test_centroids_existing_data_sources);
    return UNITY_END();
}