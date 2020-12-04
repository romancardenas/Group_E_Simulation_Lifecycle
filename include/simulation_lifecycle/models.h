#ifndef __SIMULATION_LIFECYCLE_MODELS_H__
#define __SIMULATION_LIFECYCLE_MODELS_H__

#include "cJSON.h"
#include "simulation_lifecycle/structures.h"

/**
 * @brief parses default configuration parameters common to all models.
 * @param[in] default_config pointer to JSON configuration provided by the user.
 * @param[out] target JSON structure that will contain configuration parameters.
 * @return 0 if it works successfully. Otherwise, it returns the corresponding error code.
 */
int parse_common_default_fields(const cJSON *default_config, cJSON *target);

/**
 * @brief adds cells to configuration from a data source.
 * @param[in] map JSON file that maps state and configuration parameters with fields of the data source.
 * @param[in] data_source pointer to data source that contains information regarding cells configuration.
 * @param[in] default_cell default cell state and configuration. It is used to select default values when needed.
 * @param[out] target pointer to cJSON struct that will contain the cells configuration.
 * @return 0 if the function ran successfully. Otherwise, it returns an error code.
 */
int parse_cells_from_data_source(cJSON *map, data_source_t *data_source, cJSON *default_cell, cJSON *target);

/**
 * @brief adds vicinities to cells of the configuration from a data source.
 * @param[in] data_source pointer to data source that contains information regarding cells vicinities.
 * param[in] from_map property of the data source used to get the source cell of the vicinity.
 * param[in] to_map property of the data source used to get the destination cell of the vicinity.
 * param[in] vicinity_map data-source-to-vicinity map.
 * @param[out] target pointer to cJSON struct that will contain the cells vicinities.
 * @return 0 if the function ran successfully. Otherwise, it returns an error code.
 */
int parse_vicinities_from_data_source(data_source_t *data_source, char *from_map, char *to_map, cJSON *vicinity_map, cJSON *target);

#endif //__SIMULATION_LIFECYCLE_MODELS_H__
