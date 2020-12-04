#include <stdio.h>
#include "cJSON.h"
#include "simulation_lifecycle/utils/linked_list.h"
#include "simulation_lifecycle/utils/workflow.h"
#include "simulation_lifecycle/spatial_analysis.h"
#include "simulation_lifecycle/simulation.h"
#include "simulation_lifecycle/convert.h"

/**
 * Simulation Lifecycle: main function.
 * @param argc number of arguments.
 * @param argv value of the arguments.
 * @return 0 if flow ran successfully.  TODO better documentation
 */
int main(int argc, char *argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Wrong number of arguments.\n");
        return -1;
    }

    node_t *data_sources = NULL;
    cJSON *workflow = read_workflow_file(argv[1]);

    int res = validate_workflow(workflow);
    if (res) {
        fprintf(stderr, "Workflow is invalid.\n");
        goto main_end;
    }

    if (spatial_analysis_required(workflow)) {
        fprintf(stderr, "Spatial analysis is not currently available. Skipping this part.\n"); // TODO
    }

    if (build_sim_scenario_required(workflow)) {
        res = read_data_in(workflow, &data_sources);
        if (res) {
            fprintf(stderr, "Build simulation scenario failed. Error code: %d\n", res);
            goto main_end;
        }
        if ((res = build_simulation_scenario(read_simulation(workflow), &data_sources))) {
            fprintf(stderr, "Build simulation scenario failed. Error code: %d\n", res);
            fprintf(stderr, "Hint: did you remove the output file of a previous scenario building?\n");
            goto main_end;
        }
    }

    if (run_sim_required(workflow)) {
        if ((res = run_sim(read_simulation(workflow)))) {
            fprintf(stderr, "Run simulation failed. Error code: %d\n", res);
            fprintf(stderr, "Hint: did you remove the output file of a previous simulation?\n");
            goto main_end;
        }
    }

    if (create_viz_required(workflow)) {
        fprintf(stderr, "Visualization is not currently available. Skipping this part.\n"); // TODO
    }

main_end:  /* We use label to return error code and clean the workflow object TODO anything else? */
    if (workflow != NULL) {
        cJSON_Delete(workflow);
    }
    return res;
    remove_list(&data_sources);
}
