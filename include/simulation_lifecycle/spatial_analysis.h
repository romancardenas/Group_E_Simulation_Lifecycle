#ifndef __SIMULATION_LIFECYCLE_SPATIAL_ANALYSIS_H__
#define __SIMULATION_LIFECYCLE_SPATIAL_ANALYSIS_H__

#include <stdlib.h>
#include <string.h>
#include <cJSON.h>
#include "simulation_lifecycle/structures.h"
#include "simulation_lifecycle/utils/file.h"
#include "simulation_lifecycle/spatial_analysis_operations/centroids.h"
#include "simulation_lifecycle/spatial_analysis_operations/select_by_atttributes.h"
#include "simulation_lifecycle/spatial_analysis_operations/closest_distance.h"

/**
 * @brief Reads data sources specified in the workflow file and stores their content in a list of data_source_t.
 * @param[in] workflow a cJSON object containing the complete workflow content.
 * @param[out] data_sources a pointer to a NULL list of node_t objects.
 * @return 0 if the operation was successful, an error code otherwise.
 */
int read_data_in(cJSON * workflow, node_t **data_sources);

/**
 * @brief Executes the entire spatial analysis workflow and stores results in the list of results_t.
 * @param[in] workflow a cJSON object containing the complete workflow content.
 * @param[in,out] data_sources a pointer to a NULL list of result_t objects.
 * @return 0 if the operation was successful, an error code otherwise.
 */
int execute_workflow(cJSON * workflow, node_t **data_sources);

/**
 * @brief Registers an operation to a global list of available operations to be used by the execute_workflow function.
 * @param[in] name a string containing the name of the operation to register.
 * @param[in] execute a function that executes the registered operation.
 */
void register_operation(char * name, int (* execute)(char * id, node_t * data_sources, cJSON * parameters, node_t ** results));

/**
 * @brief Registers all available operations to a global list of available operations to be used by the
 * execute_workflow function.
 */
void register_operations(void);

/**
 * @brief Retrieves an operation from the global list of available operations by name.
 * @param[in] name a string containing the name of the operation to retrieve.
 * @return an operation_t object that contains the operation retrieved.
 */
operation_t * get_operation(char * name);

#endif //__SIMULATION_LIFECYCLE_SPATIAL_ANALYSIS_H__
