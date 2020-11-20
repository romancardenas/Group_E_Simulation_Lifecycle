#include <stdio.h>
#include <stdlib.h>
#include "cJSON.h"
#include "simulation_lifecycle/utils/file.h"
#include "simulation_lifecycle/simulation.h"
#include "simulation_lifecycle/error.h"
#include "simulation_lifecycle/utils/feature.h"

/**
 * Simulation Lifecycle: main function. So far, it i just a simple hello world.
 * @param argc number of arguments
 * @param argv value of the arguments
 * @return always 0
 */
int main(int argc, char *argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Wrong number of arguments.\n");
        return -1;
    }

    cJSON * geojson = NULL;

    int res = read_json_file(argv[1], &geojson);

    if (res != SUCCESS) {
        exit(res);
    }

    cJSON * features = geojson_get_features(geojson);
    cJSON * current = features;

    for (int i = 0; i < cJSON_GetArraySize(features); i++) {
        cJSON * current = cJSON_GetArrayItem(features, i);

        printf("FEATURE START \n");
        printf("Type: %s\n", feature_get_geometry_type(current));

        int * id = feature_get_int_property(current, "id");

        (id == NULL) ? printf("ID: NULL\n") : printf("ID: %d\n", *id);

        printf("DB_UID: %s\n", feature_get_string_property(current, "dbuid"));

        double * dbrplamx = feature_get_double_property(current, "dbrplamx");
        double * dbrplamy = feature_get_double_property(current, "dbrplamy");

        (id == NULL) ? printf("dbrplamx: NULL\n") : printf("dbrplamx: %f\n", *dbrplamx);
        (id == NULL) ? printf("dbrplamy: NULL\n") : printf("dbrplamy: %f\n", *dbrplamy);

        cJSON * ring = feature_get_exterior_ring(current);

        (ring == NULL) ? printf("number of vertices: NULL\n") : printf("number of vertices: %d\n", cJSON_GetArraySize(ring));

        node_t * points = json_ring_to_points(ring);

        if (list_length(&points) == 0) {
            printf("first point: NULL");
        }
        else {
            point_t * point = (point_t *)points->data;
            printf("first point: lat %f, lon %f\n", point->lat, point->lon);
        }

        printf("FEATURE END \n\n");
    }

    build_simulation_scenario("sir", NULL, NULL, "output.json");

    return SUCCESS;
}
