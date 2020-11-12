#ifndef __SIMULATION_LIFECYCLE_SIMULATION_H__
#define __SIMULATION_LIFECYCLE_SIMULATION_H__

#include "simulation_lifecycle/structures.h"

/**
 * Creates JSON configuration file for running a Cadmium Cell-DEVS simulation. Pointers must not be null.
 * @param[in] model name of the Cadmium Cell-DEVS model for the scenario.
 * @param[in] p_features pointer to set containing the features of all the agents in the scenario.
 * @param[in] p_relations pointer to set containing the relations between agents in the scenario.
 * @param[in] output_path path to location of the simulation JSON configuration file.
 * @return 0 if function was executed successfully. Otherwise, it returns an error code.
 */
int build_simulation_scenario(char *model, feature_set_t *p_features, relation_set_t *p_relations, char *output_path);

#endif //__SIMULATION_LIFECYCLE_SIMULATION_H__
