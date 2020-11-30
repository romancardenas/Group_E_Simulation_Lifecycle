#include <unity.h>
#include "cJSON.h"
#include "simulation_lifecycle/error.h"
#include "simulation_lifecycle/utils/file.h"
#include "simulation_lifecycle/simulation.h"

void setUp(void) {
    remove("../test/data/run_simulation/2_2_agent_sir_config_outputs.txt");
}

void tearDown(void) {

}

void test_run_sim_invalid_executable(void){
    cJSON *json = NULL;

    read_json_file("../test/data/run_simulation/1_nonexisting_model.json", &json);
    TEST_ASSERT_EQUAL(SIM_RUN_ERROR, run_sim(json));
}

void test_run_sim_valid_executable(void){
    cJSON *json = NULL;

    read_json_file("../test/data/run_simulation/2_existing_model.json", &json);
    TEST_ASSERT_EQUAL(SUCCESS, run_sim(json));
}

int main(void){
    UNITY_BEGIN();
    RUN_TEST(test_run_sim_invalid_executable);
    RUN_TEST(test_run_sim_valid_executable);
    return UNITY_END();
}
