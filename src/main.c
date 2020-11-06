#include <stdio.h>
#include <stdlib.h>
#include <cjson/cJSON.h>
#include "simulation_lifecycle/simulation.h"

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

    FILE *f = fopen(argv[1], "rb");
    fseek(f, 0, SEEK_END);
    long fsize = ftell(f);
    fseek(f, 0, SEEK_SET);  /* same as rewind(f); */

    char *string = malloc(fsize + 1);
    fread(string, 1, fsize, f);
    fclose(f);
    string[fsize] = '\0';

    cJSON *json = cJSON_Parse(string);
    if (json == NULL) {
        exit(1);
    }
    build_simulation_scenario("sir", NULL, NULL, "output.json");
    return json == NULL;
}