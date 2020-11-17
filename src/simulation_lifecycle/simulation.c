#include <string.h>
#include "cJSON.h"
#include "simulation_lifecycle/error.h"
#include "simulation_lifecycle/utils/file.h"
#include "simulation_lifecycle/structures.h"
#include "simulation_lifecycle/simulation.h"

#define MODEL_SIR "sir"
#define MODEL_SIRS "sirs"
#define MODEL_SIRD "sird"
#define MODEL_SIRDS "sirds"

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
 * @brief returns pointer to a default configuration parser function.
 * @param[in] model_id string containing the Cell-DEVS model ID.
 * @return pointer to a default configuration function. If model ID couldn't be identified, it returns a NULL pointer.
 */
int (*get_model_default_config_parser(char *model_id))(const cJSON *, cJSON *);

/**
 * @brief
 * @param[in] simulation_config cJSON structure containing the simulation configuration defined by the user.
 * @param[in] config_json string containing the content of the resulting simulation configuration file.
 * @return 0 if the function ran successfully. Otherwise, it returns an error code.
 */
int write_sim_config(const cJSON *simulation_config, char *config_json);

/* TODO */
int parse_default_sir_model(const cJSON *default_config, cJSON *target);
int parse_default_sirs_model(const cJSON *default_config, cJSON *target);
int parse_default_sird_model(const cJSON *default_config, cJSON *target);
int parse_default_sirds_model(const cJSON *default_config, cJSON *target);


int build_simulation_scenario(cJSON *simulation_config, feature_set_t *p_features, relation_set_t *p_relations) {
    if (simulation_config == NULL) {
        return SIM_CONFIG_EMPTY;
    }

    cJSON *root = cJSON_CreateObject();
    int res = load_default_sim_config(simulation_config, root);

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
    /* 3. Parse default configuration of the model. The parsing function is in charge of determining the error code */
    return p_parser(simulation_config, target);
}

int (*get_model_default_config_parser(char *model_id))(const cJSON *, cJSON *) {
    if (strcmp(MODEL_SIR, model_id) == 0) {
        return parse_default_sir_model;
    } else if (strcmp(MODEL_SIRS, model_id) == 0) {
        return parse_default_sirs_model;
    } else if (strcmp(MODEL_SIRD, model_id) == 0) {
        return parse_default_sird_model;
    } else if (strcmp(MODEL_SIRDS, model_id) == 0) {
        return parse_default_sirds_model;
    } else {
        return NULL;
    }
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
