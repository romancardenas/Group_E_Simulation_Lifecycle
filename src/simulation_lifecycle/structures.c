#include <string.h>
#include "simulation_lifecycle/structures.h"

data_source_t * get_data_source(node_t ** data_sources, char *id) {
    data_source_t *res = NULL;
    if (id != NULL) {
        const node_t *current;
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
