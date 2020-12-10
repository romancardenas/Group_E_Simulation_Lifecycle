
#include "cJSON.h"
#include "simulation_lifecycle/error.h"
#include "simulation_lifecycle/utils/linked_list.h"

int centroids_validate(cJSON * parameters) {
    // Nothing to do, data is already read and in data_source_t objects at this point
    return SUCCESS;
}

int centroids_execute(char * id, node_t ** data_sources, cJSON * parameters) {
    return SUCCESS;
}