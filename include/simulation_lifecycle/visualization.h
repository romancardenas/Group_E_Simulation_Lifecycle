#ifndef __SIMULATION_LIFECYCLE_VISUALIZATION_H__
#define __SIMULATION_LIFECYCLE_VISUALIZATION_H__

#include "cJSON.h"
#include "simulation_lifecycle/error.h"

int validate_visualization(cJSON * visualization);

int package_visualization(cJSON * visualization);

#endif //__SIMULATION_LIFECYCLE_VISUALIZATION_H__
