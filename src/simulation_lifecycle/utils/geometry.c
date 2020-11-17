
#include "simulation_lifecycle/structures.h"
#include "string.h"

cJSON * get_first(feature_t * f) {
    if (strcmp(f->type, "MultiPoint") != 0 && strcmp(f->type, "MultiPolygon") != 0) {
        return NULL;
    }

    return cJSON_GetArrayItem(f->coordinates, 0);
}

point_t * get_point(cJSON * coordinates) {
    point_t * v = malloc(sizeof(point_t));

    v->lon = coordinates->child->valuedouble;
    v->lat = coordinates->child->next->valuedouble;

    return v;
}

node_t * get_exterior_ring(cJSON * coordinates) {
    cJSON * curr = cJSON_GetArrayItem(coordinates, 0);

    node_t * ring = NULL;

    while(curr != NULL) {
        point_t * point = get_point(curr);

        push_node(&ring, point, sizeof(point_t));

        curr = curr->next;
    }

    return ring;
}