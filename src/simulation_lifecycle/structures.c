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

data_source_t * create_data_source(char * id, cJSON * features) {
    cJSON * geojson = cJSON_CreateObject();

    char * json_crs = "\"crs\":{\"type\":\"name\",\"properties\":{\"name\":\"urn:ogc:def:crs:OGC:1.3:CRS84\"}}";

    cJSON_AddStringToObject(geojson, "type", "FeatureCollection");
    cJSON_AddStringToObject(geojson, "name", id);
    cJSON_AddItemReferenceToObject(geojson, "crs", cJSON_Parse(json_crs));
    cJSON_AddItemReferenceToObject(geojson, "features", features);

    data_source_t * ds = malloc(sizeof(data_source_t));

    ds->id = id;
    ds->path = NULL;
    ds->data = geojson;

    return ds;
}
