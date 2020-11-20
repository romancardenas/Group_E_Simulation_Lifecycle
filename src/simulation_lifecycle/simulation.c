#include <string.h>
#include "cJSON.h"
#include "simulation_lifecycle/error.h"
#include "simulation_lifecycle/utils/file.h"
#include "simulation_lifecycle/structures.h"
#include "simulation_lifecycle/simulation.h"
#include "simulation_lifecycle/models.h"

#define MODEL_DEFAULT "default"

#define SIM_MODEL_ID "model_id"
#define SIM_CONFIG_OUTPUT_PATH "config_output_path"
#define SIM_RESULT_OUTPUT_PATH "result_output_path"

/**
 * @brief reads simulation configuration and fills the default Cell-DEVS configuration of the simulation scenario.
 * @param[in] simulation_config cJSON structure containing the simulation configuration defined by the user.
 * @param[out] target cJSON object that will hold the default simulation configuration.
 * @return 0 if the function ran successfully. Otherwise, it returns an error code.
 */
int load_default_sim_config(const cJSON *simulation_config, cJSON *target);

/**
 * @brief
 * @param[in] simulation_config cJSON structure containing the simulation configuration defined by the user.
 * @param[in] config_json string containing the content of the resulting simulation configuration file.
 * @return 0 if the function ran successfully. Otherwise, it returns an error code.
 */
int write_sim_config(const cJSON *simulation_config, char *config_json);

/* TODO */


int build_simulation_scenario(cJSON *simulation_config, feature_set_t *p_features, relation_set_t *p_relations) {
    if (simulation_config == NULL) {
        return SIM_CONFIG_EMPTY;
    }
    int res = SUCCESS;
    cJSON *root = cJSON_CreateObject();

    cJSON *default_config = cJSON_CreateObject();

    if ((res = parse_common_default_fields(simulation_config, default_config))) {
        return res;
    }
    if ((res = load_default_sim_config(simulation_config, default_config))) {
        return res;
    }
    cJSON_AddItemToObject(root, MODEL_DEFAULT, default_config);

    /* TODO feature set etc. */

    /* Create string from result and remove cJSON structures */
    char *string = cJSON_Print(root);
    cJSON_Delete(root);
    return write_sim_config(simulation_config, string);
}

int load_default_sim_config(const cJSON *simulation_config, cJSON *target) {
    /* 1. Check that model ID is provided using a valid format */
    cJSON *model = cJSON_GetObjectItemCaseSensitive(simulation_config, SIM_MODEL_ID);
    if (model == NULL) {
        return SIM_MODEL_EMPTY;
    } else if (!cJSON_IsString(model) || model->valuestring == NULL) {
        return SIM_MODEL_INVALID;
    }
    /* 2. Check that the model exists and get the corresponding default configuration parser */
    int (*p_parser)(const cJSON *, cJSON *) = get_model_default_config_parser(model->valuestring);
    if (p_parser == NULL) {
        return SIM_MODEL_NOT_FOUND;
    }
    /* 3. Parse default configuration of the model. Parsing functions may detect an error and return an error code */
    int res = parse_common_default_fields(simulation_config, target);
    return (res) ? res : p_parser(simulation_config, target);
}

int write_sim_config(const cJSON *simulation_config, char *config_json) {
    cJSON *config_output_path = cJSON_GetObjectItemCaseSensitive(simulation_config, SIM_CONFIG_OUTPUT_PATH);
    if (config_output_path == NULL) {
        return SIM_CONFIG_OUTPUT_PATH_EMPTY;
    } else if (!cJSON_IsString(config_output_path) || config_output_path->valuestring == NULL) {
        return SIM_CONFIG_OUTPUT_PATH_INVALID;
    } else if (config_json == NULL) {
        return SIM_CONFIG_EMPTY;
    }
    return write_data_to_file(config_output_path->valuestring, config_json);
}
