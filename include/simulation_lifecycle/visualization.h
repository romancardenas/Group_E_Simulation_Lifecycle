#ifndef __SIMULATION_LIFECYCLE_VISUALIZATION_H__
#define __SIMULATION_LIFECYCLE_VISUALIZATION_H__

#include "cJSON.h"
#include "simulation_lifecycle/error.h"

/**
 * @brief Validates the visualization section of a workflow json file
 * @param[in] visualization, a cJSON object containing the visualization section of a workflow json file
 * @return 0 if the operation was successful, an error code otherwise
 */
int validate_visualization(cJSON * visualization);


/**
 * @brief Packages the visualization section into a visualization.json file that can be read by the DEVS WebViewer.
 * Also copies any geojson file required for layers in the visualization.
 * @param[in] visualization, a cJSON object containing all the parameters for DEVS WebViewer visualization
 * @param[in] output_folder, a string to the output folder where to store the visualization files
 * @return 0 if the operation was successful, an error code otherwise
 */
int package_visualization(cJSON * visualization, char * output_folder);

#endif //__SIMULATION_LIFECYCLE_VISUALIZATION_H__
