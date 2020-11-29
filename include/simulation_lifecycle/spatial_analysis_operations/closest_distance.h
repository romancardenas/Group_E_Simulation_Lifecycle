#ifndef __SIMULATION_LIFECYCLE_CLOSEST_DISTANCE_H__
#define __SIMULATION_LIFECYCLE_CLOSEST_DISTANCE_H__

#include "cJSON.h"
#include "simulation_lifecycle/error.h"
#include "simulation_lifecycle/utils/linked_list.h"

/**
 * Validates the parameters for the closest distance spatial analysis operation
 * @param parameters a cJSON object containing the parameters for the operation
 * @return 0 if the operation was successful, an error code otherwise
 */
int closest_distance_validate(cJSON * parameters);

/**
 * Executes the closest distance spatial analysis operation and stores the results in the data list
 * @param data a list containing all the potential data sources for the operation
 * @param parameters a cJSON object containing the parameters for the operation
 * @return 0 if the operation was successful, an error code otherwise
 */
int closest_distance_execute(node_t ** data, cJSON * parameters);

#endif //__SIMULATION_LIFECYCLE_CLOSEST_DISTANCE_H__
