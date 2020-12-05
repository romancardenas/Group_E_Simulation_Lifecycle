#include <stdio.h>
#include "cJSON.h"
#include "simulation_lifecycle/utils/file.h"
#include "simulation_lifecycle/simulation.h"
#include "simulation_lifecycle/utils/linked_list.h"
#include "simulation_lifecycle/spatial_analysis.h"
#include "simulation_lifecycle/convert.h"
#include "simulation_lifecycle/utils/workflow.h"


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

    //return read_json_file(argv[1], &json);

    cJSON * workflow = NULL;
    node_t * results;

    int res = read_json_file(argv[1], &workflow);

    if (res != SUCCESS) return res;


    cJSON *visualization = read_visualization(workflow);
    char *results_folder_path = "../data/results/";
    int error_code = convert_results(results_folder_path,visualization);
    return error_code;

    /*
    register_operations();

    res = execute_workflow(workflow, &results);

    if (res != SUCCESS) return res;*/

    return SUCCESS;

}
