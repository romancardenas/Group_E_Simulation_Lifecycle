#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
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
#define MAX_LEN 255

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

int build_simulation_scenario(cJSON *simulation_config, node_t **data_sources) {

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
    int model_found = file_exists(model_path);
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

    /* Get model name - e.g. 2_2_agent_sir_config */
    cJSON *model = cJSON_GetObjectItemCaseSensitive(simulation_config, SIM_MODEL_ID);

    /* Check that model ID is provided using a valid format. */
    if (model == NULL || !cJSON_IsString(model)) {
        return SIM_MODEL_SELECTION_INVALID;
    }

    /* Check that the model exists. */
    char *model_path = concat(SIM_MODEL_LIBRARY, cJSON_GetStringValue(model));
    int model_found = file_exists(model_path);
    if (!model_found) {
        return SIM_MODEL_SELECTION_INVALID;
    }

    char *model_name = cJSON_GetStringValue(model);

    /* Get current directory. */
    char current_dir[MAX_LEN];
    getcwd(current_dir, sizeof(current_dir));

    /* Change to model executable files directory. */
    chdir(SIM_MODEL_LIBRARY);

    /* Get config output path which is relative to SIM_MODEL_LIBRARY. */
    cJSON *config = cJSON_GetObjectItemCaseSensitive(simulation_config, SIM_CONFIG_OUTPUT_PATH);

    /* Check that config output path is provided using a valid format. */
    if (config == NULL || !cJSON_IsString(config)) {
        chdir(current_dir);
        return SIM_CONFIG_OUTPUT_PATH_INVALID;
    }

    /* Check that the config output path exists. */
    char *config_path = cJSON_GetStringValue(config);
    int path_found = file_exists(config_path);
    if (!path_found) {
        chdir(current_dir);
        return SIM_CONFIG_OUTPUT_PATH_INVALID;
    }

    /* Defining a command to run the simulation.
     * The command executes the model executable file with one argument
     * which is the scenario config .json file.
     * Example: ./2_2_agent_sir_config ../../../../test/data/run_simulation/config.json
     */

    /* TODO Issue: "./" works on Linux. On Windows, it will work with Windows PowerShell, but not CMD */
    /* TODO Find a better solution than using a relative config output path from "../third_party/CellDEVS_models/tutorial/bin/" */

    char command[MAX_LEN] = "";
    snprintf(command,sizeof(command),"./%s %s",model_name,config_path);

    int sim_status = system(command);

    chdir(current_dir);

    /* Check if simulation was properly ran. */
    if (sim_status != 0){
        return SIM_RUN_ERROR;
    }else{
        /* Define results filename. */
        char *results_filename = concat(model_name, SIM_RESULTS_END_FILENAME);
        char *results_filename_path = concat(SIM_RESULTS_DEFAULT_PATH,results_filename);

        /* Check if results file exists. */
        if (!file_exists(results_filename_path)) {
            return SIM_RUN_NO_RESULTS;
        }else{
            /* Moving the results file to predefined result output path. */
            /* Get result output path */
            cJSON *result = cJSON_GetObjectItemCaseSensitive(simulation_config, SIM_RESULT_OUTPUT_PATH);

            /* Check that config output path is provided and valid. */
            if (result == NULL || !cJSON_IsString(result)) {
                return SIM_RESULT_OUTPUT_PATH_INVALID;
            }

            /* Check if config output path already exists. */
            /* This check will avoid overwriting on previous simulation results. */
            char *result_path = cJSON_GetStringValue(result);
            int result_path_found = file_exists(result_path);
            if (result_path_found) {
                return SIM_RESULT_OUTPUT_PATH_ALREADY_EXISTS ;
            }

            int sim_results_move_status = rename(results_filename_path,result_path);

            /* Check if rename function was successful and if the new file exists */
            if (sim_results_move_status == 0 && file_exists(result_path)) {
                return SUCCESS;
            }else{
                return SIM_RUN_RESULTS_MOVE_FAILED;
            }
        }
    }
}
