#include <stdio.h>
#include "cJSON.h"
#include "simulation_lifecycle/utils/file.h"
#include "simulation_lifecycle/simulation.h"

/**
 * Simulation Lifecycle: main function. So far, it simply reads a JSON file.
 * @param argc number of arguments.
 * @param argv value of the arguments.
 * @return 0 if JSON file is read successfully.
 */
int main(int argc, char *argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Wrong number of arguments.\n");
        return -1;
    }

    cJSON *json = NULL;


   char *path = "../data/simulation_config.json";
   read_json_file(path,&json);
   run_sim(json);
    //printf("\n Test.");
    //return read_json_file(argv[1], &json);
}
