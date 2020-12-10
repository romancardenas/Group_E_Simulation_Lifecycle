#include <unity.h>
#include <cJSON.h>
#include "simulation_lifecycle/utils/file.h"
#include "simulation_lifecycle/error.h"
#include "simulation_lifecycle/utils/feature.h"

cJSON * geojson_good = NULL;
cJSON * geojson_error = NULL;
cJSON * feature_good = NULL;

void setUp(void) {
    char * path_good = "../test/data/DB_Ottawa_Simple.geojson";
    char * path_error = "../test/data/geojson-error.geojson";

    char * data_feature_good = "{\"type\":\"Feature\",\"properties\":{\"integer\":1,\"string\":\"potato\",\"double\":1.1},\"geometry\":{\"type\":\"MultiPolygon\",\"coordinates\":[[[[0,0],[0,10],[10,10],[10,0],[0,0]],[[2,2],[2,8],[8,8],[8,2],[2,2]]],[[[20,20],[25,20],[25,25],[20,25],[20,20]]]]}}";

    feature_good = cJSON_Parse(data_feature_good);

    int res = read_json_file(path_good, &geojson_good);

    if (res != SUCCESS) {
        printf("Unable to load data for test_feature.");
    }

    res = read_json_file(path_error, &geojson_error);

    if (res != SUCCESS) {
        printf("Unable to load data for test_feature.");
    }
}

void tearDown(void) {
    // clean stuff up here
}

void test_geojson_get_features(void) {
    cJSON * good_features = geojson_get_features(geojson_good);
    cJSON * bad_features = geojson_get_features(geojson_error);
    cJSON * null_geojson = geojson_get_features(NULL);

    // TODO: a geojson file can be wrong in many ways, do we need to test more?

    TEST_ASSERT_TRUE(good_features != NULL);
    TEST_ASSERT_TRUE(cJSON_IsArray(good_features) == 1);
    TEST_ASSERT_TRUE(cJSON_GetArraySize(good_features) == 12);
    TEST_ASSERT_TRUE(bad_features == NULL);
    TEST_ASSERT_TRUE(null_geojson == NULL);
}

void test_feature_get_properties(void) {
    char * data_properties_bad = "{\"type\":\"Feature\",\"properties-bad\":{\"integer\":1,\"string\":\"potato\",\"double\":1.1},\"geometry\":{\"type\":\"MultiPolygon\",\"coordinates\":[[[[0,0],[0,10],[10,10],[10,0],[0,0]],[[2,2],[2,8],[8,8],[8,2],[2,2]]],[[[20,20],[25,20],[25,25],[20,25],[20,20]]]]}}";

    cJSON * feature_bad = cJSON_Parse(data_properties_bad);

    cJSON * good_props = feature_get_properties(feature_good);
    cJSON * bad_props = feature_get_properties(feature_bad);
    cJSON * null_props = feature_get_properties(NULL);

    TEST_ASSERT_TRUE(good_props != NULL);
    TEST_ASSERT_TRUE(cJSON_IsObject(good_props) == 1);
    TEST_ASSERT_TRUE(cJSON_GetArraySize(good_props) == 3);
    TEST_ASSERT_TRUE(bad_props == NULL);
    TEST_ASSERT_TRUE(null_props == NULL);
}

void test_feature_get_property(void) {
    cJSON * good_prop = feature_get_property(feature_good, "integer");
    cJSON * bad_prop = feature_get_property(feature_good, "bad-property");
    cJSON * null_prop = feature_get_property(NULL, "integer");

    TEST_ASSERT_TRUE(good_prop != NULL);
    TEST_ASSERT_TRUE(cJSON_IsNumber(good_prop));
    TEST_ASSERT_TRUE(good_prop->valueint == 1);
    TEST_ASSERT_TRUE(bad_prop == NULL);
    TEST_ASSERT_TRUE(null_prop == NULL);
}

void test_feature_get_string_property(void) {
    char * good_prop = feature_get_string_property(feature_good, "string");
    char * bad_prop = feature_get_string_property(feature_good, "bad-string-property");
    char * null_prop = feature_get_string_property(NULL, "string");

    TEST_ASSERT_TRUE(good_prop != NULL);
    TEST_ASSERT_TRUE(strcmp(good_prop, "potato") == 0);
    TEST_ASSERT_TRUE(bad_prop == NULL);
    TEST_ASSERT_TRUE(null_prop == NULL);
}

void test_feature_get_int_property(void) {
    int * good_prop = feature_get_int_property(feature_good, "integer");
    int * bad_prop = feature_get_int_property(feature_good, "bad-integer-property");
    int * null_prop = feature_get_int_property(NULL, "integer");

    TEST_ASSERT_TRUE(good_prop != NULL);
    TEST_ASSERT_TRUE(*good_prop == 1);
    TEST_ASSERT_TRUE(bad_prop == NULL);
    TEST_ASSERT_TRUE(null_prop == NULL);
}

void test_feature_get_double_property(void) {
    double * good_prop = feature_get_double_property(feature_good, "double");
    double * bad_prop = feature_get_double_property(feature_good, "bad-double-property");
    double * null_prop = feature_get_double_property(NULL, "double");

    TEST_ASSERT_TRUE(good_prop != NULL);
    TEST_ASSERT_TRUE(*good_prop == 1.1);
    TEST_ASSERT_TRUE(bad_prop == NULL);
    TEST_ASSERT_TRUE(null_prop == NULL);
}

void test_feature_get_geometry(void) {
    char * data_geometry_bad = "{\"type\":\"Feature\",\"properties\":{\"integer\":1,\"string\":\"potato\",\"double\":1.1},\"geometry-bad\":{\"type\":\"MultiPolygon\",\"coordinates\":[[[[0,0],[0,10],[10,10],[10,0],[0,0]],[[2,2],[2,8],[8,8],[8,2],[2,2]]],[[[20,20],[25,20],[25,25],[20,25],[20,20]]]]}}";

    cJSON *  feature_bad = cJSON_Parse(data_geometry_bad);

    cJSON * good_geometry = feature_get_geometry(feature_good);
    cJSON * bad_geometry = feature_get_geometry(feature_bad);
    cJSON * null_geometry = feature_get_geometry(NULL);

    TEST_ASSERT_TRUE(good_geometry != NULL);
    TEST_ASSERT_TRUE(cJSON_IsObject(good_geometry) == 1);
    TEST_ASSERT_TRUE(cJSON_GetArraySize(good_geometry) == 2);
    TEST_ASSERT_TRUE(bad_geometry == NULL);
    TEST_ASSERT_TRUE(null_geometry == NULL);
}

void test_feature_get_geometry_coordinates(void) {
    char * data_coordinates_bad = "{\"type\":\"Feature\",\"properties\":{\"integer\":1,\"string\":\"potato\",\"double\":1.1},\"geometry\":{\"type\":\"MultiPolygon\",\"coordinates-bad\":[[[[0,0],[0,10],[10,10],[10,0],[0,0]],[[2,2],[2,8],[8,8],[8,2],[2,2]]],[[[20,20],[25,20],[25,25],[20,25],[20,20]]]]}}";

    cJSON *  feature_bad = cJSON_Parse(data_coordinates_bad);

    cJSON * good_coords = feature_get_geometry_coordinates(feature_good);
    cJSON * bad_coords = feature_get_geometry_coordinates(feature_bad);
    cJSON * null_coords = feature_get_geometry_coordinates(NULL);

    // TODO: a geojson file can be wrong in many ways, do we need to test more?

    TEST_ASSERT_TRUE(good_coords != NULL);
    TEST_ASSERT_TRUE(cJSON_IsArray(good_coords) == 1);
    TEST_ASSERT_TRUE(cJSON_GetArraySize(good_coords) == 2);
    TEST_ASSERT_TRUE(bad_coords == NULL);
    TEST_ASSERT_TRUE(null_coords == NULL);
}

void test_feature_get_geometry_type(void) {
    char * data_geometry_type_bad = "{\"type\":\"Feature\",\"properties\":{\"integer\":1,\"string\":\"potato\",\"double\":1.1},\"geometry\":{\"type-bad\":\"MultiPolygon\",\"coordinates\":[[[[0,0],[0,10],[10,10],[10,0],[0,0]],[[2,2],[2,8],[8,8],[8,2],[2,2]]],[[[20,20],[25,20],[25,25],[20,25],[20,20]]]]}}";

    cJSON * feature_bad = cJSON_Parse(data_geometry_type_bad);

    char * good_type = feature_get_geometry_type(feature_good);
    char * bad_type = feature_get_geometry_type(feature_bad);
    char * null_type = feature_get_geometry_type(NULL);

    TEST_ASSERT_TRUE(good_type != NULL);
    TEST_ASSERT_TRUE(strcmp(good_type, "MultiPolygon") == 0);
    TEST_ASSERT_TRUE(bad_type == NULL);
    TEST_ASSERT_TRUE(null_type == NULL);
}

void test_feature_get_first_geometry_in_multi(void) {
    char * data_multipolygon = "{\"type\":\"Feature\",\"properties\":{\"integer\":1,\"string\":\"potato\",\"double\":1.1},\"geometry\":{\"type\":\"MultiPolygon\",\"coordinates\":[[[[0,0],[0,10],[10,10],[10,0],[0,0]],[[2,2],[2,8],[8,8],[8,2],[2,2]]],[[[20,20],[25,20],[25,25],[20,25],[20,20]]]]}}";
    char * data_multiline = "{\"type\":\"Feature\",\"properties\":{\"integer\":1,\"string\":\"potato\",\"double\":1.1},\"geometry\":{\"type\":\"MultiLine\",\"coordinates\":[[[0,0],[0,10],[10,10],[10,0],[0,0]],[[2,2],[2,8],[8,8],[8,2],[2,2]]]}}";
    char * data_multipoint = "{\"type\":\"Feature\",\"properties\":{\"integer\":1,\"string\":\"potato\",\"double\":1.1},\"geometry\":{\"type\":\"MultiPoint\",\"coordinates\":[[0,0],[0,10],[10,10],[10,0],[0,0]]}}";
    char * data_polygon = "{\"type\":\"Feature\",\"properties\":{\"integer\":1,\"string\":\"potato\",\"double\":1.1},\"geometry\":{\"type\":\"Polygon\",\"coordinates\":[[[0,0],[0,10],[10,10],[10,0],[0,0]],[[2,2],[2,8],[8,8],[8,2],[2,2]]]]}}";

    char * data_bad_geometry = "{\"type\":\"Feature\",\"properties\":{\"integer\":1,\"string\":\"potato\",\"double\":1.1},\"geometry-bad\":{\"type\":\"MultiPolygon\",\"coordinates\":[[[[0,0],[0,10],[10,10],[10,0],[0,0]],[[2,2],[2,8],[8,8],[8,2],[2,2]]],[[[20,20],[25,20],[25,25],[20,25],[20,20]]]]}}";
    char * data_bad_coordinates = "{\"type\":\"Feature\",\"properties\":{\"integer\":1,\"string\":\"potato\",\"double\":1.1},\"geometry\":{\"type\":\"MultiPolygon\",\"coordinates-bad\":[[[[0,0],[0,10],[10,10],[10,0],[0,0]],[[2,2],[2,8],[8,8],[8,2],[2,2]]],[[[20,20],[25,20],[25,25],[20,25],[20,20]]]]}}";
    char * data_bad_coordinates_0 = "{\"type\":\"Feature\",\"properties\":{\"integer\":1,\"string\":\"potato\",\"double\":1.1},\"geometry\":{\"type\":\"MultiPolygon\",\"coordinates\":[]}}";

    cJSON * feature_multipolygon = feature_get_first_geometry_coordinates_in_multi(cJSON_Parse(data_multipolygon));
    cJSON * feature_multiline = feature_get_first_geometry_coordinates_in_multi(cJSON_Parse(data_multiline));
    cJSON * feature_multipoint = feature_get_first_geometry_coordinates_in_multi(cJSON_Parse(data_multipoint));
    cJSON * feature_polygon = feature_get_first_geometry_coordinates_in_multi(cJSON_Parse(data_polygon));

    cJSON * feature_bad_geometry = feature_get_first_geometry_coordinates_in_multi(cJSON_Parse(data_bad_geometry));
    cJSON * feature_bad_coordinates = feature_get_first_geometry_coordinates_in_multi(cJSON_Parse(data_bad_coordinates));
    cJSON * feature_bad_coordinates_0 = feature_get_first_geometry_coordinates_in_multi(cJSON_Parse(data_bad_coordinates_0));
    cJSON * feature_null_coordinates = feature_get_first_geometry_coordinates_in_multi(NULL);

    TEST_ASSERT_TRUE(feature_multipolygon != NULL);
    TEST_ASSERT_TRUE(feature_multiline != NULL);
    TEST_ASSERT_TRUE(feature_multipoint != NULL);

    TEST_ASSERT_TRUE(cJSON_GetArraySize(feature_multipolygon) == 2);    // 2 rings in first polygon
    TEST_ASSERT_TRUE(cJSON_GetArraySize(feature_multiline) == 5);       // 5 vertices in first line
    TEST_ASSERT_TRUE(cJSON_GetArraySize(feature_multipoint) == 2);      // 2 coordinates in first point

    TEST_ASSERT_TRUE(feature_polygon == NULL);

    TEST_ASSERT_TRUE(feature_bad_geometry == NULL);
    TEST_ASSERT_TRUE(feature_bad_coordinates == NULL);
    TEST_ASSERT_TRUE(feature_bad_coordinates_0 == NULL);
    TEST_ASSERT_TRUE(feature_null_coordinates == NULL);
}

void test_feature_get_exterior_ring(void) {
    char * data_multipolygon = "{\"type\":\"Feature\",\"properties\":{\"integer\":1,\"string\":\"potato\",\"double\":1.1},\"geometry\":{\"type\":\"MultiPolygon\",\"coordinates\":[[[[0,0],[0,10],[10,10],[10,0],[0,0]],[[2,2],[2,8],[8,8],[8,2],[2,2]]],[[[20,20],[25,20],[25,25],[20,25],[20,20]]]]}}";
    char * data_polygon = "{\"type\":\"Feature\",\"properties\":{\"integer\":1,\"string\":\"potato\",\"double\":1.1},\"geometry\":{\"type\":\"Polygon\",\"coordinates\":[[[0,0],[0,10],[10,10],[10,0],[0,0]],[[2,2],[2,8],[8,8],[8,2],[2,2]]]}}";
    char * data_multipoint = "{\"type\":\"Feature\",\"properties\":{\"integer\":1,\"string\":\"potato\",\"double\":1.1},\"geometry\":{\"type\":\"MultiPoint\",\"coordinates\":[[0,0],[0,10],[10,10],[10,0],[0,0]]}}";

    char * data_bad_geometry = "{\"type\":\"Feature\",\"properties\":{\"integer\":1,\"string\":\"potato\",\"double\":1.1},\"geometry-bad\":{\"type\":\"MultiPolygon\",\"coordinates\":[[[[0,0],[0,10],[10,10],[10,0],[0,0]],[[2,2],[2,8],[8,8],[8,2],[2,2]]],[[[20,20],[25,20],[25,25],[20,25],[20,20]]]]}}";
    char * data_bad_coordinates = "{\"type\":\"Feature\",\"properties\":{\"integer\":1,\"string\":\"potato\",\"double\":1.1},\"geometry\":{\"type\":\"MultiPolygon\",\"coordinates-bad\":[[[[0,0],[0,10],[10,10],[10,0],[0,0]],[[2,2],[2,8],[8,8],[8,2],[2,2]]],[[[20,20],[25,20],[25,25],[20,25],[20,20]]]]}}";
    char * data_bad_coordinates_0 = "{\"type\":\"Feature\",\"properties\":{\"integer\":1,\"string\":\"potato\",\"double\":1.1},\"geometry\":{\"type\":\"MultiPolygon\",\"coordinates\":[]}}";

    cJSON * feature_multipolygon = feature_get_exterior_ring(cJSON_Parse(data_multipolygon));
    cJSON * feature_polygon = feature_get_exterior_ring(cJSON_Parse(data_polygon));
    cJSON * feature_multipoint = feature_get_exterior_ring(cJSON_Parse(data_multipoint));

    cJSON * feature_bad_geometry = feature_get_exterior_ring(cJSON_Parse(data_bad_geometry));
    cJSON * feature_bad_coordinates = feature_get_exterior_ring(cJSON_Parse(data_bad_coordinates));
    cJSON * feature_bad_coordinates_0 = feature_get_exterior_ring(cJSON_Parse(data_bad_coordinates_0));
    cJSON * feature_null_coordinates = feature_get_exterior_ring(NULL);

    TEST_ASSERT_TRUE(feature_multipolygon != NULL);
    TEST_ASSERT_TRUE(feature_polygon != NULL);

    TEST_ASSERT_TRUE(cJSON_GetArraySize(feature_multipolygon) == 5);       // 5 vertices in first line
    TEST_ASSERT_TRUE(cJSON_GetArraySize(feature_polygon) == 5);      // 2 coordinates in first point

    TEST_ASSERT_TRUE(feature_multipoint == NULL);
    TEST_ASSERT_TRUE(feature_bad_geometry == NULL);
    TEST_ASSERT_TRUE(feature_bad_coordinates == NULL);
    TEST_ASSERT_TRUE(feature_bad_coordinates_0 == NULL);
    TEST_ASSERT_TRUE(feature_null_coordinates == NULL);
}

void test_json_to_point(void) {
    cJSON * ring = feature_get_exterior_ring(feature_good);
    cJSON * json_point = cJSON_GetArrayItem(ring, 1);

    point_t * point = json_to_point(json_point);

    point_t * bad_point_1 = json_to_point(cJSON_Parse("[]"));
    point_t * bad_point_2 = json_to_point(cJSON_Parse("[3]"));

    TEST_ASSERT_TRUE(point != NULL);
    TEST_ASSERT_TRUE(point->lat == 10);
    TEST_ASSERT_TRUE(point->lon == 0);

    TEST_ASSERT_TRUE(bad_point_1 == NULL);
    TEST_ASSERT_TRUE(bad_point_2 == NULL);
}

void test_json_ring_to_points(void) {
    cJSON * ring = feature_get_exterior_ring(feature_good);

    node_t * points = json_ring_to_points(ring);

    TEST_ASSERT_TRUE(points != NULL);
    TEST_ASSERT_TRUE(list_length(&points) == 5);

    point_t * point = (point_t *)points->next->next->next->data;

    TEST_ASSERT_TRUE(point->lat == 0);
    TEST_ASSERT_TRUE(point->lon == 10);

    cJSON * bad_ring = cJSON_Parse("[[3,3],[3,4],[5],[4,4],[3,3]]");

    node_t * bad_points = json_ring_to_points(bad_ring);

    TEST_ASSERT_TRUE(bad_points == NULL);
}

int main(void) {
    setUp();

    UNITY_BEGIN();

    if (geojson_good == NULL) return 1;
    if (geojson_error == NULL) return 1;

    UNITY_BEGIN();
    RUN_TEST(test_geojson_get_features);
    RUN_TEST(test_feature_get_properties);
    RUN_TEST(test_feature_get_property);
    RUN_TEST(test_feature_get_string_property);
    RUN_TEST(test_feature_get_int_property);
    RUN_TEST(test_feature_get_double_property);
    RUN_TEST(test_feature_get_geometry);
    RUN_TEST(test_feature_get_geometry_coordinates);
    RUN_TEST(test_feature_get_geometry_type);
    RUN_TEST(test_feature_get_first_geometry_in_multi);
    RUN_TEST(test_feature_get_exterior_ring);
    RUN_TEST(test_json_to_point);
    RUN_TEST(test_json_ring_to_points);
    return UNITY_END();
}