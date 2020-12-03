#ifndef __SIMULATION_LIFECYCLE_SIMULATION_H__
#define __SIMULATION_LIFECYCLE_SIMULATION_H__

#include "cJSON.h"
#include "simulation_lifecycle/structures.h"

/**
 * Creates JSON configuration file for running a Cadmium Cell-DEVS simulation. Pointers must not be null.
 * @param[in] simulation_config pointer to cJSON object that describes simulation configuration.
 * @param[in] data_sources pointer to linked list containing data sources.
 * @return 0 if function was executed successfully. Otherwise, it returns an error code.
 */
int build_simulation_scenario(cJSON *simulation_config, node_t **data_sources);

/**
 * Run the simulation and copy the results in the result output path indicated in the JSON configuration file.
 * @param[in] simulation_config cJSON structure containing the simulation configuration defined by the user.
 * @return 0 if the function ran successfully. Otherwise, it returns an error code.
 */
int run_sim(const cJSON *simulation_config);

#endif //__SIMULATION_LIFECYCLE_SIMULATION_H__
