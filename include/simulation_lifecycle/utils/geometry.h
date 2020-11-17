#ifndef __SIMULATION_LIFECYCLE_GEOMETRY_H__
#define __SIMULATION_LIFECYCLE_GEOMETRY_H__

#include "simulation_lifecycle/structures.h"

cJSON * get_first(feature_t * f);

point_t * get_point(cJSON * coordinates);

node_t * get_exterior_ring(cJSON * coordinates);

#endif //__SIMULATION_LIFECYCLE_GEOMETRY_H__
