#include <unity.h>
#include "cJSON.h"
#include "simulation_lifecycle/error.h"
#include "simulation_lifecycle/utils/file.h"
#include "simulation_lifecycle/simulation.h"

void setUp(void) {

}

void tearDown(void) {

}

void test_run_sim_invalid_executable(void){
    cJSON *json = NULL;

    read_json_file("../test/data/run_simulation/1_nonexisting_model.json", &json);
    TEST_ASSERT_EQUAL(SIM_RUN_ERROR, run_sim(json));

}

int main(void){
    UNITY_BEGIN();
    RUN_TEST(test_run_sim_invalid_executable);
    return UNITY_END();
}
