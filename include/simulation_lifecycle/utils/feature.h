#ifndef __SIMULATION_LIFECYCLE_FEATURE_H__
#define __SIMULATION_LIFECYCLE_FEATURE_H__

#include "cJSON.h"
#include "stdlib.h"
#include "string.h"
#include "simulation_lifecycle/structures.h"

/**
 * @brief Extracts a JSON representation of the features from a JSON representation of a geojson file.
 * @param geo_json, a cJSON containing the JSON representation of a geojson file.
 * @return a cJSON containing features. This value can be NULL.
 */
cJSON * geojson_get_features(cJSON * geo_json);

/**
 * @brief Extracts a JSON representation of the properties from a JSON representation of a feature.
 * @param feature_json, a cJSON containing the JSON representation of a feature.
 * @return a cJSON containing the properties of a feature. This value can be NULL.
 */
cJSON * feature_get_properties(cJSON * feature_json);

/**
 * @brief Extracts a JSON representation of an attribute property from a JSON representation of a feature.
 * @param feature_json, a cJSON containing the JSON representation of a feature.
 * @param property, a string containing the name of the attribute property to extract.
 * @return a cJSON containing the value of the property. This value can be NULL.
 */
cJSON * feature_get_property(cJSON * feature_json, char * property);

/**
 * @brief Extracts an attribute property from a JSON representation of a feature as a string.
 * @param feature_json, a cJSON containing the JSON representation of a feature.
 * @param property, a string containing the name of the attribute property to extract.
 * @return a string containing the value of the property. This value can be NULL.
 */
char * feature_get_string_property(cJSON * feature_json, char * property);

/**
 * @brief Extracts an attribute property from a JSON representation of a feature as an int.
 * @param feature_json, a cJSON containing the JSON representation of a feature.
 * @param property, a string containing the name of the attribute property to extract.
 * @return a pointer to an int containing the value of the property. This value can be NULL.
 */
int * feature_get_int_property(cJSON * feature_json, char * property);

/**
 * @brief Extracts an attribute property from a JSON representation of a feature as a double.
 * @param feature_json, a cJSON containing the JSON representation of a feature.
 * @param property, a string containing the name of the attribute property to extract.
 * @return a pointer to a double containing the value of the property. This value can be NULL.
 */
double * feature_get_double_property(cJSON * feature_json, char * property);

/**
 * @brief Extracts a JSON representation of a geometry from a JSON representation of a feature. The
 * feature can be of any geometry type.
 * @param feature_json, a cJSON containing the JSON representation of a feature.
 * @return a cJSON containing the JSON representation of a feature's geometry.
 */
cJSON * feature_get_geometry(cJSON * feature_json);

/**
 * @brief Extracts a JSON representation of the coordinates from a JSON representation of a feature. The
 * feature can be of any geometry type.
 * @param feature_json, a cJSON containing the JSON representation of a feature.
 * @return a cJSON containing the JSON representation of the coordinates of a feature.
 */
cJSON * feature_get_geometry_coordinates(cJSON * feature_json);

/**
 * @brief Gets the type of the geometry associated to a JSON representation of a feature.
 * @param feature_json, a cJSON containing the JSON representation of a feature.
 * @return a string containing the type of the geometry associated to a JSON representation of a feature.
 * Possible values are Point, Line, Polygon, MultiPoint, MultiLine and MultiPolygon.
 */
char * feature_get_geometry_type(cJSON * feature_json);

/**
 * @brief Extracts a JSON representation of the first geometry from a JSON representation of a multi
 * feature. The feature must either be a MultiPoint, MultiLine or MultiPolygon.
 * @param feature_json, a cJSON containing the JSON representation of a multi feature.
 * @return a cJSON containing the JSON representation of the first geometry from a multi feature.
 */
cJSON * feature_get_first_geometry_coordinates_in_multi(cJSON * feature_json);

/**
 * @brief Extracts a JSON representation of a ring from a JSON representation of a polygon feature.
 * The feature must either be a MultiPolygon or a Polygon.
 * @param feature_json, a cJSON containing the JSON representation of a polygon feature.
 * @return a cJSON containing the JSON representation of a ring
 */
cJSON * feature_get_exterior_ring(cJSON * feature_json);

/**
 * @brief Converts a JSON representation of a point into a point_t.
 * @param feature_json, a cJSON containing the JSON representation of a point.
 * @return a point_t containing the point data
 */
point_t * json_to_point(cJSON * json);

/**
 * @brief Converts a JSON representation of a polygon ring into a list of point_t
 * @param json, a cJSON containing the JSON representation of a polygon ring.
 * @return a node_t containing the head of a list of point_t
 */
node_t * json_ring_to_points(cJSON * json);

#endif //__SIMULATION_LIFECYCLE_FEATURE_H__
