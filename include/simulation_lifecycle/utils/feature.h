#ifndef __SIMULATION_LIFECYCLE_FEATURE_H__
#define __SIMULATION_LIFECYCLE_FEATURE_H__

#include "cJSON.h"
#include "stdlib.h"
#include "string.h"
#include "simulation_lifecycle/structures.h"

cJSON * geojson_get_features(cJSON * geo_json);

cJSON * feature_get_properties(cJSON * feature_json);

cJSON * feature_get_property(cJSON * feature_json, char * property);

char * feature_get_string_property(cJSON * feature_json, char * property);

int * feature_get_int_property(cJSON * feature_json, char * property);

double * feature_get_double_property(cJSON * feature_json, char * property);

cJSON * feature_get_geometry(cJSON * feature_json);

cJSON * feature_get_geometry_coordinates(cJSON * feature_json);

char * feature_get_geometry_type(cJSON * feature_json);

cJSON * feature_get_first_geometry_in_multi(cJSON * feature_json);

cJSON * feature_get_exterior_ring(cJSON * feature_json);

point_t * json_to_point(cJSON * json);

node_t * json_ring_to_points(cJSON * json);

#endif //__SIMULATION_LIFECYCLE_FEATURE_H__
