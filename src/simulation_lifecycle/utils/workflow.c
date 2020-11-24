#include <string.h>
#include <unistd.h>
#include "cJSON.h"
#include "simulation_lifecycle/error.h"
#include "simulation_lifecycle/utils/file.h"
#include "simulation_lifecycle/utils/workflow.h"


cJSON * read_workflow_file(char *path_to_file){
    cJSON *content = NULL;
    read_json_file(path_to_file, &content);
    return(content);
}

cJSON * read_data_sources(cJSON * workflow){
    return  cJSON_GetObjectItem(workflow, "data_sources");
}

cJSON * read_simulation(cJSON * workflow){
    return  cJSON_GetObjectItem(workflow, "simulation");
}

cJSON * read_spatial_analysis(cJSON * workflow){
    return  cJSON_GetObjectItem(workflow, "spatial_analysis");
}

cJSON * read_visualization(cJSON * workflow){
    return  cJSON_GetObjectItem(workflow, "visualization");
}

int validate_workflow(cJSON * workflow){
    //Check workflow
    if(workflow == NULL){
        return WORKFLOW_DOES_NOT_EXIST;
    }

    if(workflow->child == NULL){
        return WORKFLOW_CONTAINS_EMPTY_JSON_OBJ;
    }
    //Check data_sources
    cJSON * data_sources = read_data_sources(workflow);
    if(data_sources == NULL){
        return NULL_DATA_SOURCES;
    }

    cJSON *data_source;
    cJSON_ArrayForEach(data_source, data_sources) {
        int id = 0;
        int path = 0;
        cJSON *element;
        cJSON_ArrayForEach(element, data_source) {
            if(strcmp(element->string, "id") == 0){
                id += 1;
                if(element->valuestring[0] == '\0' || element->valuestring == NULL){
                    return DATA_SOURCE_ID_MISSING;
                }
            } else if(strcmp(element->string, "path") == 0){
                path += 1;
                if(element->valuestring[0] == '\0'){
                    return DATA_SOURCE_PATH_MISSING;
                }
            }else{
                return INVALID_DATA_SOURCE_ARGS;
            }
        }

        // Check if both id and path were accounted for
        if(id < 1){
            return DATA_SOURCE_ID_MISSING;
        } else if(id > 1){
            return DUPLICATE_DATA_SOURCE_ID;
        }
        if(path < 1){
            return DATA_SOURCE_PATH_MISSING;
        } else if(path > 1){
            return DUPLICATE_DATA_SOURCE_PATH;
        }
    }

    //Check spatial analysis
    cJSON * spatial_analysis = read_spatial_analysis(workflow);
    if(spatial_analysis == NULL){
        return NULL_SPATIAL_ANALYSIS;
    }

    //Check visualization
    cJSON * visualization = read_visualization(workflow);
    if(visualization == NULL){
        return NULL_VISUALIZATION;
    }

    //Check simulation
    cJSON * simulation = read_simulation(workflow);
    if(simulation == NULL){
        return NULL_SIMULATION;
    }

    return SUCCESS;
}