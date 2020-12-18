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

void test_SBA_missing_data(void) {

    cJSON * workflow = NULL;
    node_t * data_sources = NULL;

    read_json_file("../test/data/spatial_operations/3_missing_data_SBA.json", &workflow);
    read_data_in(workflow, &data_sources);

    cJSON * operation = read_spatial_analysis(workflow)->child;
    cJSON * params = cJSON_GetObjectItem(operation, "parameters");

    TEST_ASSERT_EQUAL(SBA_MISSING_DATA_SOURCE, select_by_attributes_execute("odhf_facil", &data_sources, params));
}

void test_SBA_missing_data_source(void) {

    cJSON * workflow = NULL;
    node_t * data_sources = NULL;

    read_json_file("../test/data/spatial_operations/4_missing_data_source_SBA.json", &workflow);
    read_data_in(workflow, &data_sources);

    cJSON * operation = read_spatial_analysis(workflow)->child;
    cJSON * params = cJSON_GetObjectItem(operation, "parameters");

    TEST_ASSERT_EQUAL(SBA_MISSING_DATA_SOURCE, select_by_attributes_execute("odhf_facil", &data_sources, params));
}

void test_SBA_missing_field(void) {

    cJSON * workflow = NULL;
    node_t * data_sources = NULL;

    read_json_file("../test/data/spatial_operations/5_missing_field_SBA.json", &workflow);
    read_data_in(workflow, &data_sources);

    cJSON * operation = read_spatial_analysis(workflow)->child;
    cJSON * params = cJSON_GetObjectItem(operation, "parameters");

    TEST_ASSERT_EQUAL(SBA_MISSING_FIELD, select_by_attributes_execute("odhf_facil", &data_sources, params));
}

void test_SBA_missing_value(void) {

    cJSON * workflow = NULL;
    node_t * data_sources = NULL;

    read_json_file("../test/data/spatial_operations/6_missing_value_SBA.json", &workflow);
    read_data_in(workflow, &data_sources);

    cJSON * operation = read_spatial_analysis(workflow)->child;
    cJSON * params = cJSON_GetObjectItem(operation, "parameters");

    TEST_ASSERT_EQUAL(SBA_MISSING_VALUE, select_by_attributes_execute("odhf_facil", &data_sources, params));
}

void test_CD_missing_data_source(void) {

    cJSON * workflow = NULL;
    node_t * data_sources_origin = NULL;
    node_t * data_sources_dest = NULL;

    read_json_file("../test/data/spatial_operations/7_missing_data_source_CD.json", &workflow);
    read_data_in(workflow, &data_sources_origin);
    cJSON * operation_origin = read_spatial_analysis(workflow)->child->next->next;
    cJSON * params_origin = cJSON_GetObjectItem(operation_origin, "parameters");
    TEST_ASSERT_EQUAL(CD_MISSING_DATA_SOURCE, closest_distance_execute("db_centroids", &data_sources_origin, params_origin));

    read_json_file("../test/data/spatial_operations/8_missing_destination_data_source_CD.json", &workflow);
    read_data_in(workflow, &data_sources_dest);
    cJSON * operation_dest = read_spatial_analysis(workflow)->child->next->next;
    cJSON * params_dest = cJSON_GetObjectItem(operation_dest, "parameters");
    TEST_ASSERT_EQUAL(CD_MISSING_DATA_SOURCE, closest_distance_execute("db_centroids", &data_sources_dest, params_dest));
}

void test_CD_missing_parameter(void) {

    cJSON * workflow = NULL;
    node_t * data_sources_n = NULL;
    node_t * data_sources_fromid = NULL;
    node_t * data_sources_toid = NULL;

    read_json_file("../test/data/spatial_operations/9_missing_n_param_parameter_CD.json", &workflow);
    read_data_in(workflow, &data_sources_n);
    cJSON * operation_n = read_spatial_analysis(workflow)->child->next->next;
    cJSON * params_n = cJSON_GetObjectItem(operation_n, "parameters");
    TEST_ASSERT_EQUAL(CD_MISSING_PARAMETER, closest_distance_execute("db_centroids", &data_sources_n, params_n));

    cJSON_Delete(workflow);
    read_json_file("../test/data/spatial_operations/10_missing_from_id_parameter_CD.json", &workflow);
    read_data_in(workflow, &data_sources_fromid);
    cJSON * operation_fromid = read_spatial_analysis(workflow)->child->next->next;
    cJSON * params_fromid = cJSON_GetObjectItem(operation_fromid, "parameters");
    TEST_ASSERT_EQUAL(CD_MISSING_PARAMETER, closest_distance_execute("db_centroids", &data_sources_fromid, params_fromid));

    cJSON_Delete(workflow);
    read_json_file("../test/data/spatial_operations/10_missing_from_id_parameter_CD.json", &workflow);
    read_data_in(workflow, &data_sources_toid);
    cJSON * operation_toid = read_spatial_analysis(workflow)->child->next->next;
    cJSON * params_toid = cJSON_GetObjectItem(operation_toid, "parameters");
    TEST_ASSERT_EQUAL(CD_MISSING_PARAMETER, closest_distance_execute("db_centroids", &data_sources_toid, params_toid));

}

int main(void) {
    UNITY_BEGIN();
    RUN_TEST(test_centroids_invalid_data_sources);
    RUN_TEST(test_centroids_existing_data_sources);
    RUN_TEST(test_SBA_missing_data);
    RUN_TEST(test_SBA_missing_data_source);
    RUN_TEST(test_SBA_missing_field);
    RUN_TEST(test_SBA_missing_value);
    RUN_TEST(test_CD_missing_data_source);
    RUN_TEST(test_CD_missing_parameter);
    return UNITY_END();
}
