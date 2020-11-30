#include <stdio.h>
#include "cJSON.h"
#include "simulation_lifecycle/utils/file.h"
<<<<<<< HEAD
#include "simulation_lifecycle/simulation.h"
=======
#include "simulation_lifecycle/utils/linked_list.h"
#include "simulation_lifecycle/spatial_analysis.h"
>>>>>>> cae67c957b9f3a639588bab8b0cd32e22f8ee11e

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

<<<<<<< HEAD
    cJSON *json = NULL;


   char *path = "../data/simulation_config.json";
   read_json_file(path,&json);
   run_sim(json);
    //printf("\n Test.");
    //return read_json_file(argv[1], &json);
=======
    cJSON * workflow = NULL;
    node_t * results;

    int res = read_json_file(argv[1], &workflow);

    if (res != SUCCESS) return res;

    register_operations();

    res = execute_workflow(workflow, &results);

    if (res != SUCCESS) return res;

    return SUCCESS;
>>>>>>> cae67c957b9f3a639588bab8b0cd32e22f8ee11e
}
