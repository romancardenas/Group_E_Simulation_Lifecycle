#include <stdlib.h>
#include <stdio.h>
#include "cJSON.h"
#include "simulation_lifecycle/error.h"
#include "simulation_lifecycle/utils/file.h"
#include "simulation_lifecycle/utils/strings.h"
#include "simulation_lifecycle/structures.h"
#include "simulation_lifecycle/simulation.h"
#include "simulation_lifecycle/models.h"

#define MODEL_DEFAULT "default"

#define SIM_MODEL_ID "model_id"
#define SIM_MODEL_LIBRARY "../third_party/CellDEVS_models/tutorial/bin/"
#define SIM_MODEL_DEFAULT_CONFIG "default_config"
#define SIM_CONFIG_OUTPUT_PATH "config_output_path"
#define SIM_RESULT_OUTPUT_PATH "result_output_path"
#define SIM_RESULTS_DEFAULT_PATH "../third_party/CellDEVS_models/tutorial/logs/"
#define SIM_RESULTS_END_FILENAME "_outputs.txt"

/**
 * @brief reads simulation configuration and fills the default Cell-DEVS configuration of the simulation scenario.
 * @param[in] simulation_config cJSON structure containing the simulation configuration defined by the user.
 * @param[out] target cJSON object that will hold the default simulation configuration.
 * @return 0 if the function ran successfully. Otherwise, it returns an error code.
 */
int parse_default_sim_config(const cJSON *simulation_config, cJSON *target);

/**
 * @brief writes JSON configuration to the desired output path.
 * @param[in] simulation_config cJSON structure containing the simulation configuration defined by the user.
 * @param[in] config_json string containing the content of the resulting simulation configuration file.
 *            It cannot be a NULL pointer. Otherwise, the function will raise an exception.
 * @return 0 if the function ran successfully. Otherwise, it returns an error code.
 */
int write_sim_config(const cJSON *simulation_config, char *config_json);

int build_simulation_scenario(cJSON *simulation_config, Node_t  **data_sources) {

    // p_features -> list of data sources

    /*
     *
     * [{"id": "db_ottawa", "data": {...}}}, {"id"}]
     *
     *
     */
    if (simulation_config == NULL) {
        return SIM_CONFIG_EMPTY;
    }
    cJSON *root = cJSON_CreateObject();
    cJSON *default_config = cJSON_CreateObject();
    int res = parse_default_sim_config(simulation_config, default_config);
    if (res) {
        return res;
    }
    cJSON_AddItemToObject(root, MODEL_DEFAULT, default_config);

    /* TODO feature set etc. */

    /* Create string from result and remove cJSON structures */
    char *string = cJSON_Print(root);
    cJSON_Delete(root);
    return write_sim_config(simulation_config, string);
}

int parse_default_sim_config(const cJSON *simulation_config, cJSON *target) {
    /* 1. Check that model ID is provided using a valid format */
    cJSON *model = cJSON_GetObjectItemCaseSensitive(simulation_config, SIM_MODEL_ID);
    if (model == NULL || !cJSON_IsString(model)) {
        return SIM_MODEL_SELECTION_INVALID;
    }
    // Check that the model exists
    char *model_path = concat(SIM_MODEL_LIBRARY, cJSON_GetStringValue(model));
    int model_found = executable_exists(model_path);
    free(model_path);
    if (!model_found) {
        return SIM_MODEL_SELECTION_INVALID;
    }
    /* 3. Parse default configuration of the model. Parsing functions may detect an error and return an error code */
    cJSON *default_config = cJSON_GetObjectItemCaseSensitive(simulation_config, SIM_MODEL_DEFAULT_CONFIG);
    if (default_config == NULL || !cJSON_IsObject(default_config)) {
        return SIM_MODEL_COMMON_CONFIG_INVALID;
    }
    return parse_common_default_fields(default_config, target);
}

int write_sim_config(const cJSON *simulation_config, char *config_json) {
    cJSON *config_output_path = cJSON_GetObjectItemCaseSensitive(simulation_config, SIM_CONFIG_OUTPUT_PATH);
    if (config_output_path == NULL || !cJSON_IsString(config_output_path)) {
        return SIM_CONFIG_OUTPUT_PATH_INVALID;
    }
    return write_data_to_file(cJSON_GetStringValue(config_output_path), config_json);
}

int run_sim(const cJSON *simulation_config){
    cJSON *model = cJSON_GetObjectItemCaseSensitive(simulation_config, SIM_MODEL_ID);
    char *model_executable_path = concat(SIM_MODEL_LIBRARY, cJSON_GetStringValue(model));
    model_executable_path = concat(model_executable_path,".exe");

    cJSON *config = cJSON_GetObjectItemCaseSensitive(simulation_config, SIM_CONFIG_OUTPUT_PATH);
    char *config_path = cJSON_GetStringValue(config);

    char *command = concat(model_executable_path," ");
    command = concat(command,config_path);

    /* Run simulation */
    int sim_status = system(command);

    /* Check if simulation was properly ran. */
    if (sim_status != 0){
        return SIM_RUN_ERROR;
    }else{
        char *results_filename = concat(cJSON_GetStringValue(model), SIM_RESULTS_END_FILENAME);
        results_filename = concat(SIM_RESULTS_DEFAULT_PATH,results_filename);

        /* Check if results file exists */
        if (!file_exists(results_filename)) {
            return SIM_RUN_NO_RESULTS;
        }else{
            /* Moving the results file to predefined result output path. */
            char *results_filename_new = concat(SIM_RESULT_OUTPUT_PATH,results_filename);
            int sim_results_move_status = rename(results_filename,results_filename_new);
            if (sim_results_move_status =! 0) {
                return SIM_RUN_RESULTS_MOVE_FAILED;
            }else{
                return SUCCESS;
            }
        }
    }
}