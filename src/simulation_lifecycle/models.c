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

int parse_common_default_fields(const cJSON *default_config, cJSON *target) {
    /* 1. Detect default cell type */
    char *type = cJSON_GetStringValue(cJSON_GetObjectItemCaseSensitive(default_config, MODEL_CELL_TYPE));
    if (type == NULL) {
        return SIM_MODEL_COMMON_CONFIG_INVALID;
    }
    cJSON_AddStringToObject(target, MODEL_CELL_TYPE, type);
    /* 2. Detect output delay buffer */
    char *delay = cJSON_GetStringValue(cJSON_GetObjectItemCaseSensitive(default_config, MODEL_CELL_DELAY));
    if (delay == NULL) {
        return SIM_MODEL_COMMON_CONFIG_INVALID;
    }
    if (strcmp(delay, DELAY_BUFFER_TRANSPORT) != 0) {
        if (strcmp(delay, DELAY_BUFFER_INERTIAL) != 0) {
            if (strcmp(delay, DELAY_BUFFER_HYBRID) != 0) {
                return SIM_MODEL_COMMON_CONFIG_INVALID;
            }
        }
    }
    cJSON_AddStringToObject(target, MODEL_CELL_DELAY, delay);
    /* 3. Detect default state */
    cJSON *state = cJSON_GetObjectItemCaseSensitive(default_config, MODEL_CELL_STATE);
    if(state == NULL) {
        return SIM_MODEL_COMMON_CONFIG_INVALID;
    }
    cJSON_AddItemToObject(target, MODEL_CELL_STATE, cJSON_Parse(cJSON_Print(state)));
    /* 4. Detect default config (if any) */
    cJSON *config = cJSON_GetObjectItemCaseSensitive(default_config, MODEL_CELL_CONFIG);
    if(config != NULL) {
        cJSON_AddItemToObject(target, MODEL_CELL_CONFIG, cJSON_Parse(cJSON_Print(config)));
    }
    /* 5. Add default neighborhood (so far, it is always empty for default configurations) */
    cJSON_AddItemToObject(target, MODEL_NEIGHBORHOOD, cJSON_CreateObject());
    return SUCCESS;
}
