#include <stdio.h>
#include <stdlib.h>
#include "cJSON.h"
#include "simulation_lifecycle/utils/file.h"
#include "simulation_lifecycle/simulation.h"
#include "simulation_lifecycle/error.h"
#include "simulation_lifecycle/utils/geometry.h"

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

    feature_set_t * fs = NULL;

    int res = read_geojson_file(argv[1], &fs);

    if (res != SUCCESS) {
        exit(res);
    }

    node_t * current = fs->features;

    while (current != NULL) {
        feature_t * f = (feature_t *)current->data;

        cJSON * first = get_first(f);

        node_t * ring = get_exterior_ring(first);

        current = current->next;
    }

    build_simulation_scenario("sir", NULL, NULL, "output.json");

    return SUCCESS;
}
