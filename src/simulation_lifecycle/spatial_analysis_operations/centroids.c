#include "cJSON.h"
#include "simulation_lifecycle/error.h"
#include "simulation_lifecycle/utils/linked_list.h"
#include "simulation_lifecycle/structures.h"
#include "simulation_lifecycle/utils/feature.h"

int centroids_execute(char *id, node_t **data_sources, cJSON *parameters) {
    cJSON *json_data = cJSON_GetObjectItemCaseSensitive(parameters, "data");
    char *source_data = cJSON_GetStringValue(cJSON_GetObjectItem(json_data, "source"));
    data_source_t *data_source = get_data_source(data_sources, source_data);
    if (data_source == NULL){
        return DATA_SOURCE_ID_NULL;
    }

    int count_data_sources = list_length(data_sources);

    cJSON *result = cJSON_CreateObject();
    cJSON *result_features = cJSON_CreateArray();
    cJSON_AddItemToObject(result, "features", result_features);
    cJSON *features = geojson_get_features(data_source->data);
    cJSON *feature = NULL;

    cJSON_ArrayForEach(feature, features) {
        cJSON *ring = feature_get_exterior_ring(feature);
        node_t *coordinates = json_ring_to_points(ring);
        point_t *point = (point_t *) coordinates->data;
        double min_lat = point->lat;
        double max_lat = point->lat;
        double min_lon = point->lon;
        double max_lon = point->lat;

        /* Find minimum and maximum latitude and longitude */
        node_t *current = coordinates;
        while (current != NULL) {
            point = (point_t *) current->data;
            if (min_lat > point->lat) {
                min_lat = point->lat;
            }
            if (max_lat < point->lat) {
                max_lat = point->lat;
            }
            if (min_lon > point->lon) {
                min_lon = point->lon;
            }
            if (max_lon < point->lon) {
                max_lat = point->lon;
            }
            current = current->next;
        }

        /* Calculate the centroid latitude and longitude */
        double centroid_lat = (min_lat + max_lat) / 2;
        double centroid_lon = (min_lon + max_lon) / 2;

        /* Creation of new geoson data with centroids */
        cJSON *properties = feature_get_properties(feature);
        cJSON *geo = feature_create_point_geometry(centroid_lat, centroid_lon);
        cJSON *new_feature = feature_create(properties, geo);
        cJSON_AddItemToArray(result_features, new_feature);
    }

    data_source_t *ds = create_data_source(id, result_features);
    push_node(data_sources, ds, sizeof(data_source_t));

    /* Check if a new data source was created */
    if (count_data_sources == list_length(data_sources)){
        return OPERATION_NO_OUTPUT;
    }

    return SUCCESS;
}