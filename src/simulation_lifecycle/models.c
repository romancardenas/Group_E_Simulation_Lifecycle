#include <string.h>
#include "cJSON.h"
#include "simulation_lifecycle/error.h"
#include "simulation_lifecycle/models.h"
#include "simulation_lifecycle/utils/feature.h"

#define MODEL_CELL_TYPE "cell_type"
#define MODEL_CELL_DELAY "delay"
#define MODEL_CELL_ID "cell_id"
#define MODEL_CELL_STATE "state"
#define MODEL_CELL_CONFIG "config"
#define MODEL_NEIGHBORHOOD "neighborhood"
#define MODEL_VICINITY "vicinity"

#define DELAY_BUFFER_TRANSPORT "transport"
#define DELAY_BUFFER_HYBRID "hybrid"
#define DELAY_BUFFER_INERTIAL "inertial"

/**
 * @brief checks that delay buffer is valid.
 * @param delay_id string containing the delay buffer ID.
 * @return 1 if the delay buffer is valid. 0 otherwise.
 */
int valid_delay(char *delay_id);

/**
 * @brief creates state configuration of a cell from a data source.
 * @param[in] feature pointer to the data feature that represents a cell
 * @param[in] default_param default cell parameter. It is used to select default values when needed.
 * @param[in] source_to_param_map JSON file that maps parameters with fields of the data source.
 * @return pointer to cJSON struct containing the cell parameter. If anything went wrong, the function returns NULL.
 */
cJSON * create_param_from_feature(cJSON *feature, cJSON *default_param, cJSON *source_to_param_map);

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
    if (!valid_delay(delay)) {
        return SIM_MODEL_COMMON_CONFIG_INVALID;
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

int parse_cells_from_data_source(cJSON *map, data_source_t *data_source, cJSON *default_cell, cJSON *target) {
    /* parse data source ID and data-source-field-to-cell-id map. They must be strings */
    char *id_key = cJSON_GetStringValue(cJSON_GetObjectItemCaseSensitive(map, MODEL_CELL_ID));
    if (id_key == NULL) {
        return SIM_MODEL_CELL_MAPPING_INVALID;
    }
    /* Get data-source-to-state map. They can be NULL */
    char *data_cell_type = cJSON_GetStringValue(cJSON_GetObjectItemCaseSensitive(map, MODEL_CELL_TYPE));
    char *data_delay_type = cJSON_GetStringValue(cJSON_GetObjectItemCaseSensitive(map, MODEL_CELL_DELAY));
    if (data_delay_type != NULL && !valid_delay(data_delay_type)) {
        return SIM_MODEL_CELL_MAPPING_INVALID;
    }
    cJSON *data_to_state_map = cJSON_GetObjectItemCaseSensitive(map, MODEL_CELL_STATE);
    cJSON *data_to_config_map = cJSON_GetObjectItemCaseSensitive(map, MODEL_CELL_CONFIG);

    /* Iterate over each data source element and create the corresponding cell configuration */
    cJSON *cell_iterator = NULL;
    cJSON_ArrayForEach(cell_iterator, data_source->data) {
        /* Cell ID is a mandatory field. It must exist in the data source */
        char *cell_id = feature_get_string_property(cell_iterator, id_key);
        if(cell_id == NULL) {
            return SIM_MODEL_CELL_MAPPING_INVALID;
        }

        /* Cell config is a JSON object. It may be empty if it inherits all the default values */
        cJSON *cell_config = cJSON_CreateObject();

        /* Check if a special cell type is necessary */
        if (data_cell_type != NULL) {
            cJSON_AddStringToObject(cell_config, MODEL_CELL_TYPE, data_cell_type);
        }
        /* Check if a special delay buffer is necessary */
        if (data_delay_type != NULL) {
            cJSON_AddStringToObject(cell_config, MODEL_CELL_DELAY, data_delay_type);
        }
        /* Check if a state mapping is necessary */
        if (data_to_state_map != NULL) {
            cJSON * default_state = cJSON_GetObjectItemCaseSensitive(default_cell, MODEL_CELL_STATE);
            cJSON *state = create_param_from_feature(cell_iterator, default_state, data_to_state_map);
            if (state == NULL) {
                cJSON_Delete(cell_config);
                return SIM_MODEL_CELL_MAPPING_INVALID;
            }
            cJSON_AddItemToObject(cell_config, MODEL_CELL_STATE, state);
        }

        /* Check if a config mapping is necessary */
        if (data_to_config_map != NULL) {
            cJSON * default_config = cJSON_GetObjectItemCaseSensitive(default_cell, MODEL_CELL_CONFIG);
            cJSON *config = create_param_from_feature(cell_iterator, default_config, data_to_config_map);
            if (config == NULL) {
                cJSON_Delete(cell_config);
                return SIM_MODEL_CELL_MAPPING_INVALID;
            }
            cJSON_AddItemToObject(cell_config, MODEL_CELL_CONFIG, config);
        }
        cJSON_AddItemToObject(target, cell_id, cell_config);
    }
    return SUCCESS;
}

int parse_vicinities_from_data_source(data_source_t *data_source, char *from_map, char *to_map, cJSON *vicinity_map, cJSON *target) {
    cJSON *vicinity_iterator = NULL;
    cJSON_ArrayForEach(vicinity_iterator, data_source->data) {
        char *cell_from = feature_get_string_property(vicinity_iterator, from_map);
        char *cell_to = feature_get_string_property(vicinity_iterator, to_map);
        if(cell_from == NULL || cell_to == NULL) {
            return SIM_MODEL_VICINITY_MAPPING_INVALID;
        }
        /* Cells of the vicinity must be already defined in target cJSON */
        cJSON *destination_cell = cJSON_GetObjectItemCaseSensitive(target, cell_to);
        if (destination_cell == NULL || cJSON_GetObjectItemCaseSensitive(target, cell_from) == NULL) {
            return SIM_MODEL_VICINITY_MAPPING_INVALID;
        }
        /* Source cell cannot be already a neighbor of destination cell */
        cJSON * neighborhood = cJSON_GetObjectItemCaseSensitive(destination_cell, MODEL_NEIGHBORHOOD);
        if (neighborhood == NULL) {
            neighborhood = cJSON_CreateObject();
            cJSON_AddItemToObject(destination_cell, MODEL_NEIGHBORHOOD, neighborhood);
        } else if (cJSON_GetObjectItemCaseSensitive(neighborhood, cell_from) != NULL) {
            return SIM_MODEL_VICINITY_MAPPING_INVALID;
        }
        /* We reuse the function for default state. As there is no default vicinity, we use the vicinity map as default */
        cJSON *vicinity = create_param_from_feature(vicinity_iterator, vicinity_map, vicinity_map);
        if (vicinity == NULL) {
            return SIM_MODEL_VICINITY_MAPPING_INVALID;
        }
        /* If everything went OK, we add the new vicinity to the corresponding neighborhood */
        cJSON_AddItemToObject(neighborhood, cell_from, vicinity);
    }
    return SUCCESS;
}

int valid_delay(char *delay_id) {
    if (strcmp(delay_id, DELAY_BUFFER_TRANSPORT) != 0) {
        if (strcmp(delay_id, DELAY_BUFFER_INERTIAL) != 0) {
            if (strcmp(delay_id, DELAY_BUFFER_HYBRID) != 0) {
                return 0;
            }
        }
    }
    return 1;
}

cJSON * create_param_from_feature(cJSON *feature, cJSON *default_param, cJSON *source_to_param_map) {
    /* 1. If default state is a cJSON object, we call the function recursively to explore each of its items */
    if (cJSON_IsObject(default_param)) {
        cJSON *res = cJSON_CreateObject();
        cJSON *param_iterator = NULL;
        cJSON_ArrayForEach(param_iterator, default_param) {
            /* 1.1. We get the map for the specific child item of the object */
            cJSON *submap = cJSON_GetObjectItemCaseSensitive(source_to_param_map, param_iterator->string);
            cJSON *param = create_param_from_feature(feature, param_iterator, submap);
            /* 1.2. If result is NULL, something went wrong -> remove result and return NULL */
            if (param == NULL) {
                cJSON_Delete(res);
                return NULL;
                /* 1.3 Otherwise, we add mapped parameter to result with the exact same name as the default parameter */
            } else {
                cJSON_AddItemToObject(res, param_iterator->string, param);
            }
        }
        /* 1.4 After finishing with all the iterations of the object, we return the resulting state */
        return res;
        /* 2. If default state is not a cJSON object and the source-to-state map is a string -> we try to map the value */
    } else if (cJSON_IsString(source_to_param_map)) {
        cJSON *res = feature_get_property(feature, cJSON_GetStringValue(source_to_param_map));
        return (res == NULL) ? res : cJSON_Parse(cJSON_Print(res));
        /* 3. If user didn't specified a string for the source-to-state map, we just copy the default value */
    } else if (source_to_param_map == NULL) {
        return cJSON_Parse(cJSON_Print(default_param));
        /* 4. Currently, we do not support arrays, numbers, nor booleans as maps. Return NULL */
    } else {
        return NULL;
    }
}
