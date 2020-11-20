#include <unity.h>
#include "cJSON.h"
#include "simulation_lifecycle/utils/file.h"
#include "simulation_lifecycle/simulation.h"

void setUp(void) {
    // set stuff up here
}

void tearDown(void) {
    // clean stuff up here
}

void test_simulation(void) {
    char file_path[] = "../../data/simulation_config.json";
    cJSON *json = NULL;
    read_json_file(file_path, &json);
    int res = build_simulation_scenario(json, NULL, NULL);
    TEST_ASSERT_TRUE(res == 0);
}

int main(void) {
    UNITY_BEGIN();
    RUN_TEST(test_simulation);
    return UNITY_END();
}