#ifndef __SIMULATION_LIFECYCLE_WORKFLOW__
#define __SIMULATION_LIFECYCLE_WORKFLOW__

#include <unistd.h>
#include "cJSON.h"

cJSON * read_workflow_file(char *path_to_file);

cJSON * read_data_sources(cJSON * workflow);

cJSON * read_simulation(cJSON * workflow);

cJSON * read_spatial_analysis(cJSON * workflow);

cJSON * read_visualization(cJSON * workflow);

int validate_workflow(cJSON * workflow);

#endif //__SIMULATION_LIFECYCLE_WORKFLOW__