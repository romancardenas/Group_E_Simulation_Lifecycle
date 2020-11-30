#ifndef __SIMULATION_LIFECYCLE_STRUCTURES_H__
#define __SIMULATION_LIFECYCLE_STRUCTURES_H__

#include "simulation_lifecycle/utils/linked_list.h"
#include "cJSON.h"

typedef struct workflow {
    // TODO
} workflow_t;

typedef struct point {
    double lat;
    double lon;
} point_t;

typedef struct feature_set {
    // TODO
} feature_set_t;

typedef struct relation_set {
    // TODO
} relation_set_t;

typedef struct cell_config_set {
    char *config_name;
    cJSON *config_params;
} cell_config_t;

typedef struct viz_options {
    // TODO
} viz_options_t;

typedef struct data_source {
    char * id;
    char * path;
    cJSON * data;
} data_source_t;

typedef struct operation {
    char * name;
    int (* validate)(cJSON * parameters);
    int (* execute)(node_t ** data_sources, cJSON * parameters);
} operation_t;

/**
 * @brief from a list of data sources, it returns a pointer to the data source with the desired ID.
 *        It returns a NULL pointer if the data source is not found.
 * @param[in] data_sources pointer to list of data sources.
 * @param[in] id ID of the desired data source.
 * @return pointer to data source with matching ID. If no data source is found, it returns a NULL pointer.
 */
data_source_t * get_data_source(node_t **data_sources, char *id);

int feature_set_empty(const feature_set_t *p_features);  // TODO
int relation_set_empty(const relation_set_t *p_relations);  // TODO
int inconsistent_data(const feature_set_t *p_features, const relation_set_t *p_relations);  // TODO

#endif //__SIMULATION_LIFECYCLE_STRUCTURES_H__
