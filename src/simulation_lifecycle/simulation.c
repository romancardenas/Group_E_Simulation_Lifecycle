#include <stdlib.h>
#include <sys/stat.h>
#include <string.h>
#include <stdio.h>
#include <cJSON.h>
#include <limits.h>
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
#define SIM_RESULTS_DEFAULT_PATH "../logs/"
#define SIM_RESULTS_END_FILENAME "_outputs.txt"
#define OUTPUT_PATH "simulation/"
#define OUTPUT_SCENARIO "build_simulation_output.json"
#define OUTPUT_RESULT "simulation_output_result.txt"

#define SIM_DATA_SOURCE "source"
#define SIM_MODEL_CELLS "cells"
#define SIM_MODEL_DEFAULT "default"
#define SIM_MODEL_VICINITY "vicinity"
#define SIM_MODEL_VICINITIES "vicinities"
#define SIM_MODEL_VICINITIES_FROM "from"
#define SIM_MODEL_VICINITIES_TO "to"
#define SIM_MODEL_NEIGHBORHOOD "neighborhood"

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
 * @brief checks that all the cells of the model have at least one neighboring cell.
 * @param[in] target cJSON struct with all the cells of the scenario that needs to be checked.
 * @return 0 if the function ran successfully. Otherwiese, it returns an error code.
 */
int check_valid_vicinities(const cJSON *target);

/**
 * @brief writes JSON configuration to the desired output path.
 * @param[in] output_folder a string containing the path to the output folder.
 * @param[in] config_json string containing the content of the resulting simulation configuration file.
 *            It cannot be a NULL pointer. Otherwise, the function will raise an exception.
 * @return 0 if the function ran successfully. Otherwise, it returns an error code.
 */
int write_sim_config(char *output_folder, char *config_json);

int build_simulation_scenario(const cJSON *const simulation_config, node_t ** data_sources, char * output_folder) {
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
    res = write_sim_config(output_folder, cJSON_Print(root));
    cJSON_Delete(root);
    return res;
}

int parse_default_sim_config(const cJSON *const simulation_config, cJSON *target) {
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

int parse_cells_config(const cJSON *const simulation_config, node_t ** data_sources, cJSON *target) {
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
    /* Check that there is at least one cell in the scenario (the default cell doesn't count) */
    return (cJSON_GetArraySize(target) > 1)? SUCCESS : SIM_MODEL_CELLS_CONFIG_INVALID;
}

int parse_vicinities(const cJSON *const simulation_config,  node_t ** data_sources, cJSON *target) {
    /* Get mandatory fields for vicinity mapping. Check that they have valid values. */
    cJSON *vicinities = cJSON_GetObjectItemCaseSensitive(simulation_config, SIM_MODEL_VICINITIES);
    if (vicinities == NULL || !cJSON_IsObject(vicinities)) {
        return SIM_MODEL_VICINITIES_CONFIG_INVALID;
    }
    char *from_map = cJSON_GetStringValue(cJSON_GetObjectItemCaseSensitive(vicinities, SIM_MODEL_VICINITIES_FROM));
    char *to_map = cJSON_GetStringValue(cJSON_GetObjectItemCaseSensitive(vicinities, SIM_MODEL_VICINITIES_TO));
    char * data_source_id = cJSON_GetStringValue(cJSON_GetObjectItemCaseSensitive(vicinities, SIM_DATA_SOURCE));
    cJSON * vicinity_map = cJSON_GetObjectItemCaseSensitive(vicinities, SIM_MODEL_VICINITY);
    if (from_map == NULL || to_map == NULL|| data_source_id == NULL || vicinity_map == NULL) {
        return SIM_MODEL_VICINITIES_CONFIG_INVALID;
    }
    /* The data source for getting the vicinities of the scenario must exist. */
    data_source_t *data_source = get_data_source(data_sources, data_source_id);
    if (data_source == NULL) {
        return SIM_MODEL_VICINITIES_CONFIG_INVALID;
    }
    /* Call auxiliary function to parse the cells one by one. Then, check that all the cells have at least one neighbor */
    int res = parse_vicinities_from_data_source(data_source, from_map, to_map, vicinity_map, target);
    return (res)? res : check_valid_vicinities(target);
}

int check_valid_vicinities(const cJSON *const target) {
    cJSON *cell;
    cJSON_ArrayForEach(cell, target) {
        if (strcmp(cell->string, SIM_MODEL_DEFAULT) != 0) { /* the default cell may not have neighborhood. */
            cJSON *neighborhood = cJSON_GetObjectItemCaseSensitive(cell, SIM_MODEL_NEIGHBORHOOD);
            if (!cJSON_GetArraySize(neighborhood)) {  /* Cells must have at least one neighbor. */
                return SIM_MODEL_VICINITY_MAPPING_INVALID;
            }
        }
    }
    return SUCCESS;
}

int write_sim_config(char *output_folder, char *config_json) {
    if (output_folder == NULL) {
        return SIM_CONFIG_OUTPUT_PATH_INVALID;
    }

    char full_path[PATH_MAX] = "";
    /* First, we create the output_folder/OUTPUT_PATH directory if it does not exist */
    join_paths(full_path, output_folder, OUTPUT_PATH);
    mkdir(full_path, 0777);

    join_paths(full_path, output_folder, OUTPUT_PATH OUTPUT_SCENARIO);
    return write_data_to_file(full_path, config_json);
}

int run_sim(const cJSON *simulation_config, char * output_folder){
    // TODO: I added this so the test can go throuh, don't know if it's really required.
    /* Check that model ID is provided using a valid format. */
    if (output_folder == NULL) {
        return SIM_RESULT_OUTPUT_PATH_INVALID;
    }

    /* Get model name - e.g. 2_2_agent_sir_config */
    char *model_name = cJSON_GetStringValue(cJSON_GetObjectItemCaseSensitive(simulation_config, SIM_MODEL_ID));

    /* Check that model ID is provided using a valid format. */
    if (model_name == NULL) {
        return SIM_MODEL_SELECTION_INVALID;
    }

    /* Check that the model exists. */
    char *model_path = concat(SIM_MODEL_LIBRARY, model_name);
    if (!executable_exists(model_path)) {
        return SIM_MODEL_SELECTION_INVALID;
    }

    /* Get config output path which is relative to SIM_MODEL_LIBRARY. */
    char config_path[PATH_MAX] = "";
    join_paths(config_path, output_folder, OUTPUT_PATH OUTPUT_SCENARIO);

    /* Check that config output path is provided using a valid format. */
    if (output_folder == NULL || !file_exists(config_path)) {
        return SIM_CONFIG_OUTPUT_PATH_INVALID;
    }

    /* TODO Issue: "./" works on Linux. On Windows, it will work with Windows PowerShell, but not CMD */

    char command[PATH_MAX] = "";
    snprintf(command, sizeof(command), "%s %s", model_path, config_path);
    free(model_path);

    /* Check if simulation was properly ran. */
    if (system(command) != 0){
        return SIM_RUN_ERROR;
    }
    /* Define results filename. */
    char *results_filename = concat(model_name, SIM_RESULTS_END_FILENAME);
    char *results_filename_path = concat(SIM_RESULTS_DEFAULT_PATH, results_filename);
    free(results_filename);

    /* Check if results file exists. */
    if (!file_exists(results_filename_path)) {
        return SIM_RUN_NO_RESULTS;
    }

    char result_path[PATH_MAX] = "";
    join_paths(result_path, output_folder, OUTPUT_PATH OUTPUT_RESULT);

    /* Check if config output path already exists. This check will avoid overwriting on previous simulation results. */
    if (file_exists(result_path)){
        return SIM_RESULT_OUTPUT_PATH_ALREADY_EXISTS ;
    }

    int sim_results_move_status = rename(results_filename_path, result_path);
    free(results_filename_path);
    /* Check if rename function was successful and if the new file exists */
    if (sim_results_move_status != 0 || !file_exists(result_path)) {
        return SIM_RUN_RESULTS_MOVE_FAILED;
    }
    return SUCCESS;
}
