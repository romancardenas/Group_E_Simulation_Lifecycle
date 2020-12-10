#include <stdlib.h>
#include <cJSON.h>
#include <string.h>
#include "simulation_lifecycle/error.h"
#include "simulation_lifecycle/structures.h"
#include "simulation_lifecycle/utils/file.h"
#include "simulation_lifecycle/utils/workflow.h"
#include "simulation_lifecycle/spatial_analysis_operations/centroids.h"
#include "simulation_lifecycle/spatial_analysis_operations/select_by_atttributes.h"
#include "simulation_lifecycle/spatial_analysis_operations/closest_distance.h"

node_t * registered_operations = NULL;

// TODO: This could be used in feature.c
char * json_string_value(cJSON * json, char * property) {
    cJSON * p = cJSON_GetObjectItem(json, property);

    return (p == NULL) ? NULL : cJSON_GetStringValue(p);
}

int read_data_in(cJSON * workflow, node_t ** data_sources) {
    cJSON * json_data_sources = read_data_sources(workflow);
    cJSON * json_data_source;

    cJSON_ArrayForEach(json_data_source, json_data_sources) {
        data_source_t * data_source = (data_source_t *)malloc(sizeof(data_source_t));

        data_source->id = json_string_value(json_data_source, "id");
        data_source->path = json_string_value(json_data_source, "path");

        if (data_source->id == NULL) return DATA_SOURCE_ID_NULL;
        if (data_source->path == NULL) return DATA_SOURCE_PATH_NULL;

        data_source->data = NULL;

        int res = read_json_file(data_source->path, &data_source->data);

        if (res != SUCCESS) return res;

        push_node(data_sources, data_source, sizeof(data_source_t));
    }

    return SUCCESS;
}

operation_t * get_operation(char * name) {
    node_t * current = registered_operations;

    while (current != NULL) {
        operation_t * op = (operation_t *)current->data;

        if (strcmp(op->name, name) == 0) return op;

        current = current->next;
    }

    return NULL;
}

void register_operation(char * name, int (* execute)(char * id, node_t ** data, cJSON * parameters)) {
    operation_t * operation = (operation_t *)malloc(sizeof(operation_t));

    operation->name = name;
    operation->execute = execute;

    push_node(&registered_operations, operation, sizeof(operation_t));
}

void register_operations(void) {
    register_operation("centroids", centroids_execute);
    register_operation("select_by_attributes", select_by_attributes_execute);
    register_operation("closest_distance", closest_distance_execute);
}

int execute_workflow(cJSON * workflow, node_t ** data_sources) {
    cJSON * operations = read_spatial_analysis(workflow);

    if (cJSON_GetArraySize(operations) == 0) return WORKFLOW_NO_OPERATIONS;

    cJSON * operation = NULL;

    cJSON_ArrayForEach(operation, operations) {
        char * name = json_string_value(operation, "operation");
        char * id = json_string_value(operation, "id");

        if (!id || !name) return OPERATION_NAME_NULL;

        cJSON * parameters = cJSON_GetObjectItem(operation, "parameters");

        if (cJSON_IsNull(parameters)) return OPERATION_NO_PARAMETERS;

        operation_t * op = get_operation(name);

        if (!op) return OPERATION_UNREGISTERED;

        int res = op->execute(id, data_sources, parameters);

        if (res != SUCCESS) return res;
    }

    return SUCCESS;
}