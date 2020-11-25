#include <stdlib.h>
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

int build_simulation_scenario(cJSON *simulation_config, feature_set_t *p_features, relation_set_t *p_relations) {
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
