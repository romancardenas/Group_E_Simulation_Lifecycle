#ifndef __SIMULATION_LIFECYCLE_CLOSEST_DISTANCE_H__
#define __SIMULATION_LIFECYCLE_CLOSEST_DISTANCE_H__

#include <cJSON.h>
#include "simulation_lifecycle/error.h"
#include "simulation_lifecycle/utils/linked_list.h"

/**
 * @brief Executes the closest distance spatial analysis operation and stores the results in the data list.
 * @param[in] id a string containing the identification of the data source.
 * @param[in,out] data a list containing all the potential data sources for the operation.
 * @param[in] parameters a cJSON object containing the parameters for the operation.
 * @return 0 if the operation was successful, an error code otherwise.
 */
int closest_distance_execute(char * id, node_t ** data, cJSON * parameters);

#endif //__SIMULATION_LIFECYCLE_CLOSEST_DISTANCE_H__
