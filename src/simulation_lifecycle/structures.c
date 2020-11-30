#include <string.h>
#include "simulation_lifecycle/structures.h"

int feature_set_empty(const feature_set_t *p_features) {
    return 1;  // TODO
}

int relation_set_empty(const relation_set_t *p_relations){
    return 1;  // TODO
}

int inconsistent_data(const feature_set_t *p_features, const relation_set_t *p_relations) {
    return 1;  // TODO
}

data_source_t * get_data_source(node_t **data_sources, char *id) {
    data_source_t *res = NULL;
    if (id != NULL) {
        node_t *current;
        for (current = *data_sources; current != NULL; current = current->next) {
            data_source_t *p_source = (data_source_t *) current->data;
            if (!strcmp(p_source->id, id)) {
                res = p_source;
                break;
            }
        }
    }
    return res;
}
