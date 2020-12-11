#include "string.h"
#include "cJSON.h"
#include "simulation_lifecycle/error.h"
#include "simulation_lifecycle/utils/linked_list.h"
#include "simulation_lifecycle/structures.h"
#include "simulation_lifecycle/utils/feature.h"

int select_by_attributes_execute(char * id, node_t ** data_sources, cJSON * parameters) {
    // Find target data source *json_data
    cJSON * json_data = cJSON_GetObjectItemCaseSensitive(parameters, "data");
    if(NULL == json_data){
        return SBA_DATA_MISSING;
    }

    // Find Source data name
    char * source_data = cJSON_GetStringValue(cJSON_GetObjectItem(json_data, "source"));
    if(NULL == source_data){
        return SBA_MISSING_DATA_SOURCE;
    }

    // Find Field attribute
    char * field = cJSON_GetStringValue(cJSON_GetObjectItemCaseSensitive(parameters, "field"));
    if(NULL == field){
        return SBA_MISSING_FIELD;
    }

    // Find Value attribute
    char * value = cJSON_GetStringValue(cJSON_GetObjectItemCaseSensitive(parameters, "value"));
    if(NULL == value){
        return SBA_MISSING_VALUE;
    }
    data_source_t * data_source = get_data_source(data_sources, source_data);
    cJSON * result = cJSON_CreateObject();
    cJSON * result_features = cJSON_CreateArray();
    cJSON_AddItemToObject(result, "features", result_features);
    cJSON * features = geojson_get_features(data_source->data);
    cJSON * feature = NULL;
    char * p = NULL;
    cJSON_ArrayForEach(feature, features) {
        p = feature_get_string_property(feature, field);
        if (strcmp(p, value) == 0) {
            cJSON * properties = feature_get_properties(feature);
            cJSON * geometry = feature_get_geometry(feature);
            cJSON * new_feature = feature_create(properties, geometry);
            cJSON_AddItemToArray(result_features, new_feature);
        }
    }
    data_source_t * ds = create_data_source(id, result_features);
    push_node(data_sources, ds, sizeof(data_source_t));
    return SUCCESS;
}