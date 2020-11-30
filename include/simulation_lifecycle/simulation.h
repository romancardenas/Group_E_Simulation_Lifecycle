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

#endif //__SIMULATION_LIFECYCLE_SIMULATION_H__
