#include <unity.h>
#include "simulation_lifecycle/utils/geometry.h"
#include "simulation_lifecycle/structures.h"
#include "simulation_lifecycle/utils/linked_list.h"

void setUp(void) {
    // set stuff up here
}

void tearDown(void) {
    // clean stuff up here
}

void test_get_first(void) {
    feature_t * f = (feature_t *)malloc(sizeof(feature_t));

    f->properties = cJSON_Parse("{\"id\":17206,\"dauid\":\"35060603\",\"pruid\":\"35\",\"prname\":\"Ontario\",\"cduid\":\"3506\",\"cdname\":\"Ottawa\"}");
    f->coordinates = cJSON_Parse("[[[[7465666, 1178880], [7465452, 1178937], [7465766, 1179144], [7465686, 1178883], [7465666, 1178880]]]]");

    f->type = "Point";
    TEST_ASSERT_TRUE(get_first(f) == NULL);

    f->type = "Line";
    TEST_ASSERT_TRUE(get_first(f) == NULL);

    f->type = "Polygon";
    TEST_ASSERT_TRUE(get_first(f) == NULL);

    f->type = "SomethingThatDoesntExist";
    TEST_ASSERT_TRUE(get_first(f) == NULL);

    cJSON * first = NULL;

    f->type = "MultiPoint";
    f->coordinates = cJSON_Parse("[[10, 10], [11, 11], [12, 12], [13, 13], [14, 14]]");

    first = get_first(f);

    TEST_ASSERT_TRUE(first != NULL);
    TEST_ASSERT_TRUE(first->valueint == 10);
    TEST_ASSERT_TRUE(first->next->valueint == 10);

    f->coordinates = cJSON_Parse("[]");
    first = get_first(f);

    TEST_ASSERT_TRUE(first == NULL);


    f->type = "MultiLine";
    f->coordinates = cJSON_Parse("[[[10, 10], [10, 20], [20, 20]], [[50, 50], [60, 60], [50, 70]]]");

    first = get_first(f);

    TEST_ASSERT_TRUE(first != NULL);
    TEST_ASSERT_TRUE(first->child->valueint == 10);
    TEST_ASSERT_TRUE(first->child->next->valueint == 10);

    f->coordinates = cJSON_Parse("[[]]");
    first = get_first(f);

    TEST_ASSERT_TRUE(first == NULL);

    f->type = "MultiPolygon";
    f->coordinates = cJSON_Parse("[[[[10, 10], [10, 20], [20, 20], [20, 10], [10, 10]],[[12, 12], [12, 18], [18, 18], [18, 12], [12, 12]]]],[[[30, 30], [30, 40], [40, 40], [40, 30], [30, 30]],[[32, 32], [32, 38], [38, 38], [38, 32], [32, 32]]]]");

    first = get_first(f);

    TEST_ASSERT_TRUE(first != NULL);
    TEST_ASSERT_TRUE(first->child->child->valueint == 10);
    TEST_ASSERT_TRUE(first->child->child->next->valueint == 10);

    f->coordinates = cJSON_Parse("[[[[],[[12, 12], [12, 18], [18, 18], [18, 12], [12, 12]]],[[[30, 30], [30, 40], [40, 40], [40, 30], [30, 30]],[[32, 32], [32, 38], [38, 38], [38, 32], [32, 32]]]]");
    first = get_first(f);

    TEST_ASSERT_TRUE(first == NULL);
}

void test_get_point(void) {
    cJSON * coordinates = cJSON_Parse("[10, 11]");

    point_t * point = get_point(coordinates->child);

    TEST_ASSERT_TRUE(point != NULL);
    TEST_ASSERT_TRUE(point->lat == 11);
    TEST_ASSERT_TRUE(point->lon == 10);
}

void test_get_exterior_ring(void) {
    feature_t * f = (feature_t *)malloc(sizeof(feature_t));

    f->type = "MultiPolygon";
    f->properties = NULL;
    f->coordinates = cJSON_Parse("[[[[10, 10], [10, 20], [20, 20], [20, 10], [10, 10]],[[12, 12], [12, 18], [18, 18], [18, 12], [12, 12]]]],[[[30, 30], [30, 40], [40, 40], [40, 30], [30, 30]],[[32, 32], [32, 38], [38, 38], [38, 32], [32, 32]]]]");

    cJSON * first = get_first(f);
    node_t * ring =  get_exterior_ring(first);

    point_t  * point = (point_t *)ring->data;

    TEST_ASSERT_TRUE(point->lat == 10);
    TEST_ASSERT_TRUE(point->lon == 10);

    point = (point_t *)ring->next->next->next->data;

    TEST_ASSERT_TRUE(point->lat == 10);
    TEST_ASSERT_TRUE(point->lon == 20);
}

int main(void) {
    UNITY_BEGIN();
    RUN_TEST(test_get_first);
    RUN_TEST(test_get_point);
    RUN_TEST(test_get_exterior_ring);
    return UNITY_END();
}