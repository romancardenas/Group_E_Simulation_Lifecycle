#ifndef __SIMULATION_LIFECYCLE_CENTROIDS_H__
#define __SIMULATION_LIFECYCLE_CENTROIDS_H__

#include "cJSON.h"
#include "simulation_lifecycle/error.h"
#include "simulation_lifecycle/utils/linked_list.h"

/**
 * Validates the parameters for the centroids spatial analysis operation
 * @param parameters a cJSON object containing the parameters for the operation
 * @return 0 if the operation was successful, an error code otherwise
 */
int centroids_validate(cJSON * parameters);

/**
 * Executes the centroids spatial analysis operation
 * @param data_sources a list containing all the potential data sources for the operation
 * @param parameters a cJSON object containing the parameters for the operation
 * @param results a list of all the results that precede this operation in the workflow
 * @return 0 if the operation was successful, an error code otherwise
 */
int centroids_execute(node_t * data_sources, cJSON * parameters, node_t ** results);

#endif //__SIMULATION_LIFECYCLE_CENTROIDS_H__
