#include <cJSON.h>
#include <math.h>
#include "simulation_lifecycle/error.h"
#include "simulation_lifecycle/utils/linked_list.h"
#include "simulation_lifecycle/structures.h"
#include "simulation_lifecycle/utils/feature.h"

typedef struct distance {
    cJSON * feature;
    double distance;
} distance_t;

// Compare delegate function. This compares two distance_t elements using their distances.
int compare (const void * a, const void * b) {
    distance_t * c = (distance_t *)a;
    distance_t * d = (distance_t *)b;

    if (c->distance > d->distance) {
        return  1;
    }

    if (c->distance < d->distance) {
        return -1;
    }

    return 0;
}

int closest_distance_execute(char * id, node_t ** data_sources, cJSON * parameters) {
    // Read origin and destination data source IDs from JSON parameters
    char * origin_id = cJSON_GetStringValue(cJSON_GetObjectItem(parameters, "origin"));
    char * destination_id = cJSON_GetStringValue(cJSON_GetObjectItem(parameters, "destination"));

    // Get data source objects corresponding to IDs found above
    data_source_t * origin = get_data_source(data_sources, origin_id);
    data_source_t * destination = get_data_source(data_sources, destination_id);

    // Check if data sources were provided, return error if not
    if (origin == NULL || destination == NULL) {
        return CD_MISSING_DATA_SOURCE;
    }

    // Read number of neighbors parameter from JSON parameters
    cJSON * n_param = cJSON_GetObjectItem(parameters, "number_of_neighbors");

    // Read from id from JSON parameters. This parameter is used to identify the ID field of the origin feature
    char * from_id = cJSON_GetStringValue(cJSON_GetObjectItem(parameters, "origin_id"));

    // Read to id from JSON parameters. This parameter is used to identify the ID field of the destination feature
    char * to_id = cJSON_GetStringValue(cJSON_GetObjectItem(parameters, "destination_id"));

    // Check if any parameter is missing return an error if so
    if (n_param == NULL || from_id == NULL || to_id == NULL) {
        return CD_MISSING_PARAMETER;
    }

    // Keep number of neighbors in a convenient variable
    int n_neighbors = n_param->valueint;

    // Read all geospatial features from geojson origin and destination data sources
    cJSON * origin_features = geojson_get_features(origin->data);
    cJSON * destination_features = geojson_get_features(destination->data);

    // Prepare iterators for loop below
    cJSON * origin_iterator = NULL;
    cJSON * destination_iterator = NULL;

    // Get number of features in destination to declare properly sized distances array
    int n = cJSON_GetArraySize(destination_features);

    // if user requested more neighbors than are available, use the number available.
    if (n_neighbors > n) {
        n_neighbors = n;
    }

    // Prepare a cJSON object that will hold the results of the closest distance operation. To start with
    // the JSON object, for now, looks like this { "features":[]  }
    cJSON *result = cJSON_CreateObject();
    cJSON *result_features = cJSON_CreateArray();
    cJSON_AddItemToObject(result, "features", result_features);

    // This is a double loop that evaluates each origin feature against each destination feature
    cJSON_ArrayForEach(origin_iterator, origin_features) {
        // Declare a structure of distance_t array, each distance_t is a composition of a double distance and a
        // cJSON geospatial feature. This is done for convenience, so that we can easily sort the features based
        // on their calculated distances. Manipulating two different arrays would've been messy.
        distance_t distances[n];

        // i is the counter variable to determine current distance in distance structure array
        int i = 0;

        // Get origin point as cJSON first, then convert to actual point object.
        cJSON * origin_coords = feature_get_geometry_coordinates(origin_iterator);
        point_t * x = json_to_point(origin_coords);

        cJSON_ArrayForEach(destination_iterator, destination_features) {
            // Get destination point as cJSON first, then convert to actual point object.
            cJSON * destination_coords = feature_get_geometry_coordinates(destination_iterator);
            point_t * y = json_to_point(destination_coords);

            // Assign distances[i]. Provide geospatial feature and calculate distance
            distances[i].feature = destination_iterator;

            // Calculate distance
            distances[i].distance = sqrt(pow(fabs(y->lon - x->lon), 2) + pow(fabs(y->lat - x->lat), 2));

            i++;
        }

        // Sort all distance objects using delegate compare function, see above. Result is a properly sorted array with
        // each feature and their corresponding distance to destination.
        qsort(distances, n, sizeof(distance_t), compare);

        // max distance, used to calculate connectivity
        double max = 0;

        // Get max distance within the neighbors requested by user, for normalization
        for (int j = 0; j < n_neighbors; j++) {
            if (distances[j].distance > max) {
                max = distances[j].distance;
            }
        }

        // Get the JSON representation of the id field for the origin feature
        cJSON * s_from_id = feature_get_property(origin_iterator, from_id);

        // Ready to add all features and calculate normalized distances
        for (int j = 0; j < n_neighbors; j++) {
            // Get the JSON representation of the id field for the destination feature
            cJSON * s_to_id = feature_get_property(distances[j].feature, to_id);

            // Create an empty properties JSON object
            cJSON * properties = cJSON_CreateObject();

            // Fill the properties object with the from id, to id, distance and normalized distance using max value
            cJSON_AddItemReferenceToObject(properties, "from", s_from_id);
            cJSON_AddItemReferenceToObject(properties, "to", s_to_id);
            cJSON_AddNumberToObject(properties, "distance", distances[j].distance);
            cJSON_AddNumberToObject(properties, "normalized", distances[j].distance / max);

            // TODO: This part is cheating, it's specifically for the SIRD model, we don't have time
            // TODO: to do this properly. This forces furthest neighbor to have a value of 0.2
            cJSON_AddNumberToObject(properties, "conn", 0.2 + 0.8 * (1 - (distances[j].distance / max)));
            cJSON_AddNumberToObject(properties, "mob", 1);

            // Create new feature, add to set of results.
            cJSON *new_feature = feature_create(properties, NULL);

            cJSON_AddItemToArray(result_features, new_feature);
        }
    }

    // Create a new data source to hold the created features. This data source is then added to the data sources array
    data_source_t *ds = create_data_source(id, result_features);
    push_node(data_sources, ds, sizeof(data_source_t));

    return SUCCESS;
}