#include <cJSON.h>
#include <stdlib.h>
#include <string.h>
#include "simulation_lifecycle/utils/file.h"
#include "simulation_lifecycle/error.h"

char * get_string(cJSON * json, char * field) {
    cJSON * item = cJSON_GetObjectItem(json, field);

    return (item == NULL) ? NULL : cJSON_GetStringValue(item);
}

int validate_string(cJSON * data, char * field, int optional, int error) {
    cJSON * section = cJSON_GetObjectItem(data, field);

    if (optional == 1 && section == NULL) return SUCCESS;

    return cJSON_IsString(section) ? SUCCESS : error;
}

int validate_int(cJSON * data, char * field, int optional, int error) {
    cJSON * section = cJSON_GetObjectItem(data, field);

    if (optional == 1 && section == NULL) return SUCCESS;

    return cJSON_IsNumber(section) ? SUCCESS : error;
}

int validate_optional_string(cJSON * data, char * field, int error) {
    return validate_string(data, field, 1, error);
}

int validate_mandatory_string(cJSON * data, char * field, int error) {
    return validate_string(data, field, 0, error);
}

int validate_mandatory_number(cJSON * data, char * field, int error) {
    return validate_int(data, field, 0, error);
}

int validate_array(cJSON * data, char * field, int (* validate)(cJSON * item), int error) {
    int res = SUCCESS;
    cJSON * items = cJSON_GetObjectItem(data, field);
    cJSON * item = NULL;

    if (!cJSON_IsArray(items)) return error;

    cJSON_ArrayForEach(item, items) {
        if ((res = validate(item)) != SUCCESS) {
            return res;
        }
    }

    return res;
}

int validate_view(cJSON * viz) {
    cJSON * view = cJSON_GetObjectItem(viz, "view");

    if (cJSON_HasObjectItem(view, "center") == 0) {
        return VIZ_LAYER_VIEW_BAD_CENTER;
    }

    if (cJSON_HasObjectItem(view, "zoom") == 0) {
        return VIZ_LAYER_VIEW_BAD_ZOOM;
    }

    return SUCCESS;
}

int validate_layer(cJSON * layer) {
    int res = validate_mandatory_string(layer, "id", VIZ_LAYER_BAD_ID);

    char * type = get_string(layer, "type");
    char * file = get_string(layer, "file");
    char * style = get_string(layer, "style");

    if (res == SUCCESS) {
        if (type == NULL || (strcmp(type, "point") != 0 && strcmp(type, "polygon") != 0)) {
            res = VIZ_LAYER_BAD_TYPE;
        }
    }

    if (res == SUCCESS) {
        if (file == NULL || !file_exists(file))  {
            res = VIZ_LAYER_BAD_FILE;
        }
    }

    if (res == SUCCESS) {
        res = validate_mandatory_string(layer, "label", VIZ_LAYER_BAD_LABEL);
    }

    if (res == SUCCESS) {
        res = validate_mandatory_string(layer, "file", VIZ_LAYER_MISSING_STYLE);
    }

    // TODO : This should validate against the styles array
    if (res == SUCCESS) {
        res = validate_mandatory_string(layer, "style", VIZ_LAYER_BAD_STYLE);
    }

    if (res == SUCCESS) {
        res = validate_optional_string(layer, "model", VIZ_LAYER_BAD_MODEL);
    }

    if (res == SUCCESS) {
        res = validate_optional_string(layer, "join", VIZ_LAYER_BAD_JOIN);
    }

    return res;
}

int validate_symbol(cJSON * symbol) {
    // TODO: Validating every possible combination of symbol is a bit nightmarish. Keeping this for later.
    return SUCCESS;
}

int validate_style(cJSON * style) {
    int res = validate_mandatory_string(style, "id", VIZ_STYLE_BAD_IDL);

    if (res == SUCCESS) {
        res = validate_symbol(style);
    }

    return res;
}

int validate_simulation(cJSON * data) {
    int res = validate_mandatory_string(data, "name", VIZ_SIMULATION_BAD_NAME);

    // TODO: This should be validated against the layers array
    if (res == SUCCESS) {
        res = validate_mandatory_string(data, "layer", VIZ_SIMULATION_BAD_LAYER);
    }

    if (res == SUCCESS) {
        res = validate_symbol(data);
    }

    return res;
}

int validate_visualization(cJSON * data) {
    int res = validate_mandatory_string(data, "output", VIZ_NO_OUTPUT_PATH);

    if (res == SUCCESS) {
        res = validate_mandatory_string(data, "basemap", VIZ_BAD_BASEMAP);
    }

    if (res == SUCCESS) {
        res = validate_view(data);
    }

    if (res == SUCCESS) {
        res = validate_array(data, "layers", validate_layer, VIZ_BAD_LAYERS);
    }

    if (res == SUCCESS) {
        res = validate_array(data, "simulation", validate_simulation, VIZ_BAD_SIMULATION);
    }

    return res;
}

char * join_strings(char * a, char * b) {
    char * out = malloc(strlen(a) + strlen(b) + 1);

    strcpy(out, a);
    strcat(out, b);

    return out;
}

int package_visualization(cJSON * data) {
    char * o_dir = get_string(data, "output");

    cJSON * layers = cJSON_GetObjectItem(data, "layers");
    cJSON * layer = NULL;

    int res = SUCCESS;

    // Replace path to geojson by filename
    cJSON_ArrayForEach(layer, layers) {
        cJSON * file = cJSON_GetObjectItem(layer, "file");

        // Get filename without folder path
        char * s_path = cJSON_GetStringValue(file);
        char * s_file = strrchr(s_path, '/');

        s_file = (!s_file) ? strdup(s_file) : strdup(s_file + 1);

        // Read geojson content so that it can be copied
        cJSON * geojson = NULL;

        if ((res = read_json_file(s_path, &geojson) != SUCCESS)) {
            return res;
        };

        // Copy geojson file from input folder to output folder
        char * output_file = join_strings(o_dir, s_file);
        write_data_to_file(output_file, cJSON_Print(geojson));
        free(output_file);

        // Remove folder from file path so users can load from client-side
        cJSON_SetValuestring(file, s_file);
    }

    // Copy copy visualization.json to output folder
    char * output_file = join_strings(o_dir, "visualization.json");
    write_data_to_file(output_file, cJSON_Print(data));
    free(output_file);

    // Remove output field, shouldn't be in visualization.json
    cJSON_DeleteItemFromObject(data, "output");

    return SUCCESS;
}