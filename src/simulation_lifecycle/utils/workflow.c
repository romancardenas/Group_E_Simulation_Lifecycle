#include <string.h>
#include <cJSON.h>
#include "simulation_lifecycle/error.h"
#include "simulation_lifecycle/utils/file.h"
#include "simulation_lifecycle/utils/workflow.h"


cJSON * read_workflow_file(char *path_to_file) {
    cJSON *content = NULL;
    read_json_file(path_to_file, &content);
    return(content);
}

// TODO ROMAN: I think we must add a header file with macros corresponding to JSON labels (I don't like magic values)

char * read_output_folder(const cJSON *const workflow) {
    return cJSON_GetStringValue(cJSON_GetObjectItem(workflow, "output"));
}

cJSON * read_data_sources(const cJSON *const workflow) {
    return  cJSON_GetObjectItem(workflow, "data_sources");
}

cJSON * read_simulation(const cJSON *const workflow){
    return  cJSON_GetObjectItem(workflow, "simulation");
}

cJSON * read_spatial_analysis(const cJSON *const workflow){
    return  cJSON_GetObjectItem(workflow, "spatial_analysis");
}

cJSON * read_conversion(const cJSON *const workflow){
    return  cJSON_GetObjectItem(workflow, "conversion");
}

cJSON * read_visualization(const cJSON *const workflow){
    return  cJSON_GetObjectItem(workflow, "visualization");
}

int validate_workflow(const cJSON *const workflow){
    /* Check workflow */
    if (NULL == workflow) {
        return WORKFLOW_DOES_NOT_EXIST;
    }

    if (NULL == workflow->child) {
        return WORKFLOW_CONTAINS_EMPTY_JSON_OBJ;
    }

    /* Check output folder */
    char * output = read_output_folder(workflow);
    if (NULL == output) {
        return NULL_OUTPUT_FOLDER;
    }

    /* Check data_sources */
    cJSON * data_sources = read_data_sources(workflow);
    if (NULL == data_sources) {
        return NULL_DATA_SOURCES;
    }

    cJSON *data_source = NULL;
    cJSON_ArrayForEach(data_source, data_sources) {
        int id = 0;
        int path = 0;
        cJSON *element = NULL;
        cJSON_ArrayForEach(element, data_source) {
            char *elem_string = cJSON_GetStringValue(element);
            if (0 == strcmp(element->string, "id")) {
                id += 1;
                if (NULL == elem_string || '\0' == elem_string[0]) {
                    return DATA_SOURCE_ID_MISSING;
                }
            } else if (0 == strcmp(element->string, "path")) {
                path += 1;
                if (NULL == elem_string || '\0' == elem_string[0]) {
                    return DATA_SOURCE_PATH_MISSING;
                }
            } else {
                return INVALID_DATA_SOURCE_ARGS;
            }
        }

        /* Check if both id and path were accounted for */
        if (id < 1) {
            return DATA_SOURCE_ID_MISSING;
        } else if (id > 1) {
            return DUPLICATE_DATA_SOURCE_ID;
        }
        if (path < 1) {
            return DATA_SOURCE_PATH_MISSING;
        } else if(path > 1) {
            return DUPLICATE_DATA_SOURCE_PATH;
        }
    }

    return SUCCESS;
}

int data_sources_required(const cJSON *const workflow) {
    return build_sim_scenario_required(workflow) || create_viz_required(workflow) || spatial_analysis_required(workflow);
}

int spatial_analysis_required(const cJSON *const workflow) {
    return NULL != read_spatial_analysis(workflow);
}

int build_sim_scenario_required(const cJSON *const workflow ) {
    return NULL != read_simulation(workflow);
}

int run_sim_required(const cJSON *const workflow) {
    return NULL != read_simulation(workflow);
}

int conversion_required(const cJSON *const workflow) {
    return NULL != read_conversion(workflow);
}

int create_viz_required(const cJSON *const workflow) {
    return NULL != read_visualization(workflow);
}
