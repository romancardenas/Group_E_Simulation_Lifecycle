#ifndef __SIMULATION_LIFECYCLE_SIMULATION_H__
#define __SIMULATION_LIFECYCLE_SIMULATION_H__

#include "cJSON.h"
#include "simulation_lifecycle/structures.h"

/**
 * Creates JSON configuration file for running a Cadmium Cell-DEVS simulation. Pointers must not be null.
 * @param[in] simulation_config pointer to cJSON object that describes simulation configuration.
 * @param[in] p_features pointer to set containing the features of all the agents in the scenario.
 * @param[in] p_relations pointer to set containing the relations between agents in the scenario.
 * @return 0 if function was executed successfully. Otherwise, it returns an error code.
 */
int build_simulation_scenario(cJSON *simulation_config, feature_set_t *p_features, relation_set_t *p_relations);

#endif //__SIMULATION_LIFECYCLE_SIMULATION_H__
