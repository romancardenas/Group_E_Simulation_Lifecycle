#include "cJSON.h"
#include "simulation_lifecycle/error.h"
#include "simulation_lifecycle/structures.h"
#include "string.h"

cJSON * json_read_field(cJSON * p_json, char * field_name) {
    int exists =  cJSON_HasObjectItem(p_json, field_name);

    return exists ? cJSON_GetObjectItem(p_json, field_name) : NULL;
}

cJSON * json_read_object(cJSON * p_json, char * field_name) {
    cJSON * jField = json_read_field(p_json, field_name);

    return (jField) ? jField->child : NULL;
}

char * json_read_string(cJSON * p_json, char * field_name) {
    cJSON * jField = json_read_field(p_json, field_name);

    return (jField) ? cJSON_GetStringValue(jField) : NULL;
}

feature_t * feature_from_json(cJSON * p_json) {
    cJSON * geometry = json_read_field(p_json, "geometry");

    if (geometry == NULL) {
        return NULL;
    }

    feature_t * f = malloc(sizeof(feature_t));

    f->type = json_read_string(geometry, "type");
    f->properties = json_read_object(p_json, "properties");
    f->coordinates = json_read_object(geometry, "coordinates");

    if (f->type == NULL || f->properties == NULL || f->coordinates == NULL) {
        return NULL;
    }

    return f;
}

feature_set_t * feature_set_from_json(cJSON * p_json) {
    cJSON * jFeatures = json_read_field(p_json, "features");

    if (!jFeatures) {
        return NULL;
    }

    feature_set_t * fs = malloc(sizeof(feature_set_t));

    fs->name = json_read_string(p_json, "name");
    fs->n = cJSON_GetArraySize(jFeatures);
    fs->features = NULL;

    for (int i = 0; i < fs->n; i++) {
        cJSON * jFeature = cJSON_GetArrayItem(jFeatures, i);

        feature_t * f = feature_from_json(jFeature);

        if (f == NULL) {
            return NULL;
        }

        push_node(&(fs->features), f, sizeof(feature_t));
    }

    return fs;
}