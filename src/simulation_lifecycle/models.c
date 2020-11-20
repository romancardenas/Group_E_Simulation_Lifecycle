#include <string.h>
#include "cJSON.h"
#include "simulation_lifecycle/error.h"
#include "simulation_lifecycle/models.h"

#define MODEL_CELL_TYPE "cell_type"
#define MODEL_CELL_DELAY "delay"
#define MODEL_CELL_STATE "state"
#define MODEL_CELL_CONFIG "config"
#define MODEL_NEIGHBORHOOD "neighborhood"

#define DELAY_BUFFER_TRANSPORT "transport"
#define DELAY_BUFFER_HYBRID "hybrid"
#define DELAY_BUFFER_INERTIAL "inertial"

#define MODEL_SIR "sir"
#define MODEL_SIRS "sirs"
#define MODEL_SIRD "sird"
#define MODEL_SIRDS "sirds"

#define MODEL_SIRX_POPULATION "population"
#define MODEL_SIRX_SUSCEPTIBLE "susceptible"
#define MODEL_SIRX_INFECTED "infected"
#define MODEL_SIRX_RECOVERED "recovered"
#define MODEL_SIRX_DECEASED "deceased"

#define MODEL_SIRX_VIRULENCE "virulence"
#define MODEL_SIRX_RECOVERY "recovery"
#define MODEL_SIRX_FATALITY "fatality"
#define MODEL_SIRX_IMMUNITY "immunity"

int parse_common_default_fields(const cJSON *default_config, cJSON *target) {
    /* 1. Detect output delay buffer type */
    cJSON *delay = cJSON_GetObjectItemCaseSensitive(default_config, MODEL_CELL_DELAY);
    if (delay == NULL || !cJSON_IsString(delay) || delay->valuestring == NULL) {
        return SIM_MODEL_DEFAULT_CONFIG_INVALID;
    }
    char *delay_buffer = delay->valuestring;
    if (strcmp(delay_buffer, DELAY_BUFFER_TRANSPORT) != 0) {
        if (strcmp(delay_buffer, DELAY_BUFFER_INERTIAL) != 0) {
            if (strcmp(delay_buffer, DELAY_BUFFER_HYBRID) != 0) {
                return SIM_MODEL_DEFAULT_CONFIG_INVALID;
            }
        }
    }
    cJSON_AddStringToObject(target, MODEL_CELL_DELAY, delay_buffer);

    /* 2. Add default neighborhood (so far, it is always empty for default configurations) */
    cJSON_AddItemToObject(target, MODEL_NEIGHBORHOOD, cJSON_CreateObject());
    return SUCCESS;
}

int copy_json_number(const cJSON *from, cJSON *to, char *label) {
    cJSON *val = cJSON_GetObjectItemCaseSensitive(from, label);
    if (val == NULL || !cJSON_IsNumber(val)) {
        return SIM_MODEL_DEFAULT_CONFIG_INVALID;  // TODO
    }
    cJSON_AddNumberToObject(to, label, cJSON_GetNumberValue(val));
    return SUCCESS;
}

int copy_json_string(const cJSON *from, cJSON *to, char *label) {
    cJSON *val = cJSON_GetObjectItemCaseSensitive(from, label);
    if (val == NULL || !cJSON_IsString(val)) {
        return SIM_MODEL_DEFAULT_CONFIG_INVALID;  // TODO
    }
    cJSON_AddStringToObject(to, label, cJSON_GetStringValue(val));
    return SUCCESS;
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

int parse_default_sir_model(const cJSON *from, cJSON *target) {
    cJSON_AddStringToObject(target, MODEL_CELL_TYPE, "sir");

    cJSON *default_state = cJSON_CreateObject();
    cJSON *s = cJSON_GetObjectItemCaseSensitive(from, MODEL_CELL_STATE);
    if (s == NULL || !cJSON_IsObject(s)) {
        return SIM_MODEL_DEFAULT_CONFIG_INVALID;
    }
    int res = copy_json_number(s, default_state, MODEL_SIRX_POPULATION);
    res += copy_json_number(s, default_state, MODEL_SIRX_SUSCEPTIBLE);
    res += copy_json_number(s, default_state, MODEL_SIRX_INFECTED);
    res += copy_json_number(s, default_state, MODEL_SIRX_RECOVERED);
    if (res) {
        return SIM_MODEL_DEFAULT_CONFIG_INVALID;
    }
    cJSON_AddItemToObject(target, MODEL_CELL_STATE, default_state);

    cJSON *default_config = cJSON_CreateObject();
    cJSON *c = cJSON_GetObjectItemCaseSensitive(from, MODEL_CELL_CONFIG);
    res += copy_json_number(c, default_config, MODEL_SIRX_VIRULENCE);
    res += copy_json_number(c, default_config, MODEL_SIRX_RECOVERY);
    if (res) {
        return SIM_MODEL_DEFAULT_CONFIG_INVALID;
    }
    cJSON_AddItemToObject(target, MODEL_CELL_CONFIG, default_state);
    
    return SUCCESS;
}

int parse_default_sirs_model(const cJSON *default_config, cJSON *target) {
    cJSON_AddStringToObject(target, MODEL_CELL_TYPE, "sirs");

    return SUCCESS;
}

int parse_default_sird_model(const cJSON *default_config, cJSON *target) {
    cJSON_AddStringToObject(target, MODEL_CELL_TYPE, "sird");

    return SUCCESS;
}

int parse_default_sirds_model(const cJSON *default_config, cJSON *target) {
    cJSON_AddStringToObject(target, MODEL_CELL_TYPE, "sirds");

    return SUCCESS;
}
