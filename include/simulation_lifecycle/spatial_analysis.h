#ifndef __SIMULATION_LIFECYCLE_SPATIAL_ANALYSIS_H__
#define __SIMULATION_LIFECYCLE_SPATIAL_ANALYSIS_H__
#include <stdlib.h>
#include <string.h>
#include "cJSON.h"
#include "simulation_lifecycle/structures.h"
#include "simulation_lifecycle/utils/file.h"
#include "simulation_lifecycle/spatial_analysis_operations/centroids.h"
#include "simulation_lifecycle/spatial_analysis_operations/select_by_atttributes.h"
#include "simulation_lifecycle/spatial_analysis_operations/closest_distance.h"

/**
 * Reads data sources specified in the workflow file and stores their content in a list of data_source_t
 * @param[in] workflow a cJSON object containing the complete workflow content
 * @param[out] data_sources a pointer to a NULL list of node_t objects
 * @return 0 if the operation was successful, an error code otherwise
 * @effect the data_sources list contains data_source_t objects with the content of the data specified in the workflow
 */
int read_data_in(cJSON * workflow, node_t **data_sources);

/**
 * Executes the entire spatial analysis workflow and stores results in the list of results_t
 * @param workflow a cJSON object containing the complete workflow content
 * @param data_sources a pointer to a NULL list of result_t objects
 * @return 0 if the operation was successful, an error code otherwise
 * @effect the results list contains result_t objects with the results of each spatial analysis operation
 */
int execute_workflow(cJSON * workflow, node_t **data_sources);

/**
 * Registers an operation to a global list of available operations to be used by the execute_workflow function
 * @param name a string containing the name of the operation to register
 * @param execute a function that executes the registered operation
 * @return nothing
 */
void register_operation(char * name, int (* execute)(char * id, node_t * data_sources, cJSON * parameters, node_t ** results));

/**
 * Registers all available operations to a global list of available operations to be used by the execute_workflow function
 * @return nothing
 */
void register_operations(void);

/**
 * Retrieves an operation from the global list of available operations by name
 * @param name a string containing the name of the operation to retrieve
 * @return an operation_t object that contains the operation retrieved
 */
operation_t * get_operation(char * name);

#endif //__SIMULATION_LIFECYCLE_SPATIAL_ANALYSIS_H__
