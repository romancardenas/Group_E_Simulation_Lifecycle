#include <stdio.h>
#include "simulation_lifecycle/error.h"
#include "simulation_lifecycle/structures.h"
#include "simulation_lifecycle/simulation.h"
#include "simulation_lifecycle/utils.h"

int model_exists(const char *model) {
    return 0;  // TODO
}

int check_sim_preconditions(const char *model, const feature_set_t *p_features,
                            const relation_set_t *p_relations, const char *output_path) {
    if (model == NULL || *model == '\0') {
        return MODEL_EMPTY;
    } else if (!model_exists(model)) {
        return MODEL_NOT_FOUND;
    }
    if (p_features == NULL || feature_set_empty(p_features)) {
        return FEATURE_SET_EMPTY;
    }
    if (p_relations == NULL || relation_set_empty(p_relations)) {
        return RELATION_SET_EMPTY;
    }
    if (inconsistent_data(p_features, p_relations)) {
        return INCONSISTENT_DATA;
    }
    if (output_path == NULL || *output_path == '\0') {
        return OUTPUT_PATH_EMPTY;
    } else if (file_exists(output_path)) {
        return OUTPUT_FILE_EXISTS;
    }
    return SUCCESS;
}

int build_simulation_scenario(char *model, feature_set_t *p_features, relation_set_t *p_relations, char *output_path) {
    int res = check_sim_preconditions(model, p_features, p_relations, output_path);
    if (res) {
        return res;
    }
    // TODO
    return res;
}
