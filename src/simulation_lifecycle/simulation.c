#include <stdlib.h>
#include "cJSON.h"
#include "simulation_lifecycle/error.h"
#include "simulation_lifecycle/utils/linked_list.h"
#include "simulation_lifecycle/utils/file.h"
#include "simulation_lifecycle/utils/strings.h"
#include "simulation_lifecycle/structures.h"
#include "simulation_lifecycle/simulation.h"
#include "simulation_lifecycle/models.h"

#define SIM_MODEL_ID "model_id"
#define SIM_MODEL_LIBRARY "../third_party/CellDEVS_models/tutorial/bin/"
#define SIM_MODEL_DEFAULT_CONFIG "default_config"
#define SIM_CONFIG_OUTPUT_PATH "config_output_path"
#define SIM_RESULT_OUTPUT_PATH "result_output_path"

#define SIM_DATA_SOURCE "source"
#define SIM_MODEL_CELLS "cells"
#define SIM_MODEL_DEFAULT "default"
#define SIM_MODEL_VICINITIES "vicinities"

/**
 * @brief reads simulation configuration and fills the default Cell-DEVS configuration of the simulation scenario.
 * @param[in] simulation_config cJSON structure containing the simulation configuration defined by the user.
 * @param[out] target cJSON object that will hold the default simulation configuration.
 * @return 0 if the function ran successfully. Otherwise, it returns an error code.
 */
int parse_default_sim_config(const cJSON *simulation_config, cJSON *target);

/**
 * @brief reads simulation configuration and fills the Cell-DEVS configuration of all the cells in the scenario.
 * @param[in] simulation_config cJSON structure containing the simulation configuration defined by the user.
 * @param[in] data_sources pointer to linked list containing data sources.
 * @param[out] target cJSON object that will hold the cells simulation configuration.
 * @return 0 if the function ran successfully. Otherwise, it returns an error code.
 */
int parse_cells_config(const cJSON *simulation_config, node_t **data_sources, cJSON *target);

/**
 * @brief reads simulation configuration and fills the vicinity configuration between the cells in the scenario.
 * @param[in] simulation_config cJSON structure containing the simulation configuration defined by the user.
 * @param[in] data_sources pointer to linked list containing data sources.
 * @param[out] target cJSON object that will hold the cells vicinities simulation configuration.
 * @return 0 if the function ran successfully. Otherwise, it returns an error code.
 */
int parse_vicinities(const cJSON *simulation_config, node_t **data_sources, cJSON *target);

/**
 * @brief writes JSON configuration to the desired output path.
 * @param[in] simulation_config cJSON structure containing the simulation configuration defined by the user.
 * @param[in] config_json string containing the content of the resulting simulation configuration file.
 *            It cannot be a NULL pointer. Otherwise, the function will raise an exception.
 * @return 0 if the function ran successfully. Otherwise, it returns an error code.
 */
int write_sim_config(const cJSON *simulation_config, char *config_json);

int build_simulation_scenario(cJSON *simulation_config, node_t **data_sources) {
    if (simulation_config == NULL) {
        return SIM_CONFIG_EMPTY;
    }
    int res;
    /* 1. Parse default configuration */
    cJSON *default_config = cJSON_CreateObject();
    if ((res = parse_default_sim_config(simulation_config, default_config))) {
        cJSON_Delete(default_config);
        return res;
    }
    cJSON *cells_config = cJSON_CreateObject();
    cJSON_AddItemToObject(cells_config, SIM_MODEL_DEFAULT, default_config); // default_config is now inside cells_config
    /* 2. We only keep filling configuration parameters if result is SUCCESS */
    res = parse_cells_config(simulation_config, data_sources, cells_config);
    if (res || (res = parse_vicinities(simulation_config, data_sources, cells_config))) {
        cJSON_Delete(cells_config);
        return res;
    }

    /* 3. Try to write simulation config file, remove cJSON structures, and return the corresponding error code. */
    cJSON *root = cJSON_CreateObject();
    cJSON_AddItemToObject(root, SIM_MODEL_CELLS, cells_config);  // cells_config is now inside root_config
    res = write_sim_config(simulation_config, cJSON_Print(root));
    cJSON_Delete(root);
    return res;
}

int parse_default_sim_config(const cJSON *simulation_config, cJSON *target) {
    /* 1. Check that model ID is provided using a valid format */
    char *model = cJSON_GetStringValue(cJSON_GetObjectItemCaseSensitive(simulation_config, SIM_MODEL_ID));
    if (model == NULL) {
        return SIM_MODEL_SELECTION_INVALID;
    }
    // Check that the model exists
    char *model_path = concat(SIM_MODEL_LIBRARY, model);
    int model_found = executable_exists(model_path);
    free(model_path);
    if (!model_found) {
        return SIM_MODEL_SELECTION_INVALID;
    }
    /* 3. Parse default configuration of the model. Parsing functions may detect an error and return an error code */
    cJSON *default_config = cJSON_GetObjectItemCaseSensitive(simulation_config, SIM_MODEL_DEFAULT_CONFIG);
    if (!cJSON_IsObject(default_config)) {
        return SIM_MODEL_COMMON_CONFIG_INVALID;
    }
    return parse_common_default_fields(default_config, target);
}

int parse_cells_config(const cJSON *simulation_config, node_t **data_sources, cJSON *target) {
    /* Get array that describes which data sources contain information regarding cell in the scenario */
    cJSON *cells = cJSON_GetObjectItemCaseSensitive(simulation_config, SIM_MODEL_CELLS);
    if (!cJSON_IsArray(cells)) {
        return SIM_MODEL_CELLS_CONFIG_INVALID;
    }
    /* Target already has the default cell config. We store this configuration in default_cell. */
    cJSON *default_cell = cJSON_GetObjectItemCaseSensitive(target, SIM_MODEL_DEFAULT);

    /* Iterate over each data-source-to-cells mapper (data may be scatter around more than one data source) */
    int res;
    cJSON *cells_mapper = NULL;
    cJSON_ArrayForEach(cells_mapper, cells) {
        /* Get corresponding data source ID */
        char * data_source_id = cJSON_GetStringValue(cJSON_GetObjectItemCaseSensitive(cells_mapper, SIM_DATA_SOURCE));
        if (data_source_id == NULL) {
            return SIM_MODEL_CELLS_CONFIG_INVALID;
        }
        data_source_t *data_source = get_data_source(data_sources, data_source_id);
        if (data_source == NULL) {
            return SIM_MODEL_CELLS_CONFIG_INVALID;
        }
        if((res = parse_cells_from_data_source(cells_mapper, data_source, default_cell, target))) {
            return res;
        }
    }
    /* Check that there is at least one valid cell */
    return (cJSON_GetArraySize(target) > 1) ? SUCCESS : SIM_MODEL_CELLS_CONFIG_INVALID;
}

int parse_vicinities(const cJSON *simulation_config, node_t **data_sources, cJSON *target) {
    return SUCCESS; // TODO remove this when the cell part is done
    cJSON *vicinities = cJSON_GetObjectItemCaseSensitive(simulation_config, SIM_MODEL_VICINITIES);
    if (vicinities == NULL || !cJSON_IsObject(vicinities)) {
        return SIM_MODEL_VICINITIES_CONFIG_INVALID;
    }
    return SUCCESS; // TODO
}

int write_sim_config(const cJSON *simulation_config, char *config_json) {
    cJSON *config_output_path = cJSON_GetObjectItemCaseSensitive(simulation_config, SIM_CONFIG_OUTPUT_PATH);
    if (config_output_path == NULL || !cJSON_IsString(config_output_path)) {
        return SIM_CONFIG_OUTPUT_PATH_INVALID;
    }
    return write_data_to_file(cJSON_GetStringValue(config_output_path), config_json);
}
