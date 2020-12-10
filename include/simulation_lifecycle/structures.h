#ifndef __SIMULATION_LIFECYCLE_STRUCTURES_H__
#define __SIMULATION_LIFECYCLE_STRUCTURES_H__

#include "simulation_lifecycle/utils/linked_list.h"
#include "cJSON.h"

#define DATA_SOURCE_FEATURES "features"

typedef struct workflow {
    // TODO are we ever gonna use this?
} workflow_t;

typedef struct point {
    double lat;
    double lon;
} point_t;

typedef struct feature_set {
    // TODO are we ever gonna use this?
} feature_set_t;

typedef struct relation_set {
    // TODO are we ever gonna use this?
} relation_set_t;

typedef struct cell_config_set {
    char *config_name;  // TODO are we ever gonna use this?
    cJSON *config_params;
} cell_config_t;

typedef struct viz_options {
    // TODO are we ever gonna use this?
} viz_options_t;

typedef struct data_source {
    char * id;
    char * path;
    cJSON * data;
} data_source_t;

typedef struct operation {
    char * name;
    int (* execute)(char * id, node_t ** data_sources, cJSON * parameters);
} operation_t;

/**
 * @brief from a list of data sources, it returns a pointer to the data source with the desired ID.
 *        It returns a NULL pointer if the data source is not found.
 * @param[in] data_sources pointer to list of data sources.
 * @param[in] id ID of the desired data source.
 * @return pointer to data source with matching ID. If no data source is found, it returns a NULL pointer.
 */
data_source_t * get_data_source(node_t **data_sources, char *id);

/**
 * @brief Creates a data source struct containing a cJSON representation of a geojson file from a cJSON
 * representation of a list of geospatial features.
 * @param[in] id a string containing the id of the data_source_t to create
 * @param[in] features, a cJSON representation of a list of geospatial features
 * @return a data_source_t object with a cJSON representation of a geojson file
 */
data_source_t * create_data_source(char * id, cJSON * features);

#endif //__SIMULATION_LIFECYCLE_STRUCTURES_H__
