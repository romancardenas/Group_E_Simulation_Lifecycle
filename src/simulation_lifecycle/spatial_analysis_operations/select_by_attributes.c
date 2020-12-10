
#include "cJSON.h"
#include "simulation_lifecycle/error.h"
#include "simulation_lifecycle/utils/linked_list.h"

// TODO: Since there's no validate anymore, we could put all this in spatial_analysis.c
int select_by_attributes_execute(char * id, node_t ** data_sources, cJSON * parameters) {
    return SUCCESS;
}