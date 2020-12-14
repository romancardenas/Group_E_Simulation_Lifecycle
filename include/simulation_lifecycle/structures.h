#ifndef __SIMULATION_LIFECYCLE_STRUCTURES_H__
#define __SIMULATION_LIFECYCLE_STRUCTURES_H__

#include <cJSON.h>
#include "simulation_lifecycle/utils/linked_list.h"

#define DATA_SOURCE_FEATURES "features"

/**
 * @brief a data structure to hold the coordinates of a point geometry
 * @field lat, a double containing the latitude of the point
 * @field lon, a double containing the longitude of the point
 */
typedef struct point {
    double lat;
    double lon;
} point_t;

/**
 * @brief a data structure to hold a workflow data source
 * @field id, a string containing the id of the data_source
 * @field path, an optional string containing the path to the data source
 * @field data, a cJSON object containing a JSON representation of the data contents
 */
typedef struct data_source {
    char * id;
    char * path;
    cJSON * data;
} data_source_t;

/**
 * @brief a data structure to hold a spatial analysis operation
 * @field name, a string containing the name of the spatial analysis operation
 * @field execute, a function that runs the spatial analysis operation
 */
typedef struct operation {
    char * name;
    int (* execute)(char * id, node_t ** data_sources, cJSON * parameters);
} operation_t;

/**
 * @brief Get a data source from a list of data sources.
 * @param[in] data_sources pointer to a list of data sources.
 * @param[in] id a string containing the id of the desired data source.
 * @return pointer to data source with matching ID. If no data source is found, it returns a NULL pointer.
 */
data_source_t * get_data_source(node_t **data_sources, char *id);

/**
 * @brief Creates a data source struct containing a cJSON representation of a geojson file from a cJSON
 * representation of a list of geospatial features.
 * @param[in] id a string containing the id of the data_source_t to create.
 * @param[in] features a cJSON representation of a list of geospatial features.
 * @return a data_source_t object with a cJSON representation of a geojson file.
 */
data_source_t * create_data_source(char * id, cJSON * features);

#endif //__SIMULATION_LIFECYCLE_STRUCTURES_H__
