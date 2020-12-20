#ifndef __SIMULATION_LIFECYCLE_FEATURE_H__
#define __SIMULATION_LIFECYCLE_FEATURE_H__

#include <cJSON.h>
#include <stdlib.h>
#include <string.h>
#include "simulation_lifecycle/structures.h"

/**
 * @brief Creates a JSON representation of a feature
 * @param properties, a cJSON representation of the feature properties (similar to a hashmap)
 * @param geometry, a cJSON representation of the feature geometry
 * @return a cJSON containing a feature
 */
cJSON * feature_create(cJSON * properties, cJSON * geometry);

/**
 * @brief Creates a JSON representation of a geometry
 * @param type, a string containing the type of the geometry (Point, Line, Polygon, MultiPoint,...)
 * @param coordinates, a JSON representation of the geometry coordinates
 * @return a cJSON containing a feature
 */
cJSON * feature_create_geometry(char * type, cJSON * coordinates);

/**
 * @brief Creates a JSON representation of a point geometry
 * @param lat, a double containing the latitude of the point
 * @param lon, a double containing the longitude of the point
 * @return a cJSON containing a point geometry.
 */
cJSON * feature_create_point_geometry(double lat, double lon);

/**
 * @brief Extracts a JSON representation of the features from a JSON representation of a geojson file.
 * @param[in] geo_json a cJSON containing the JSON representation of a geojson file.
 * @return a cJSON containing features. This value can be NULL.
 */
cJSON * geojson_get_features(cJSON * geo_json);

/**
 * @brief Extracts a JSON representation of the properties from a JSON representation of a feature.
 * @param[in] feature_json a cJSON containing the JSON representation of a feature.
 * @return a cJSON containing the properties of a feature. This value can be NULL.
 */
cJSON * feature_get_properties(cJSON * feature_json);

/**
 * @brief Extracts a JSON representation of an attribute property from a JSON representation of a feature.
 * @param[in] feature_json a cJSON containing the JSON representation of a feature.
 * @param[in] property a string containing the name of the attribute property to extract.
 * @return a cJSON containing the value of the property. This value can be NULL.
 */
cJSON * feature_get_property(cJSON * feature_json, char * property);

/**
 * @brief Extracts an attribute property from a JSON representation of a feature as a string.
 * @param[in] feature_json a cJSON containing the JSON representation of a feature.
 * @param[in] property a string containing the name of the attribute property to extract.
 * @return a string containing the value of the property. This value can be NULL.
 */
char * feature_get_string_property(cJSON * feature_json, char * property);

/**
 * @brief Extracts an attribute property from a JSON representation of a feature as an int.
 * @param[in] feature_json a cJSON containing the JSON representation of a feature.
 * @param[in] property a string containing the name of the attribute property to extract.
 * @return a pointer to an int containing the value of the property. This value can be NULL.
 */
int * feature_get_int_property(cJSON * feature_json, char * property);

/**
 * @brief Extracts an attribute property from a JSON representation of a feature as a double.
 * @param[in] feature_json a cJSON containing the JSON representation of a feature.
 * @param[in] property a string containing the name of the attribute property to extract.
 * @return a pointer to a double containing the value of the property. This value can be NULL.
 */
double * feature_get_double_property(cJSON * feature_json, char * property);

/**
 * @brief Extracts a JSON representation of a geometry from a JSON representation of a feature. The
 * feature can be of any geometry type.
 * @param[in] feature_json a cJSON containing the JSON representation of a feature.
 * @return a cJSON containing the JSON representation of a feature's geometry.
 */
cJSON * feature_get_geometry(cJSON * feature_json);

/**
 * @brief Extracts a JSON representation of the coordinates from a JSON representation of a feature. The
 * feature can be of any geometry type.
 * @param[in] feature_json a cJSON containing the JSON representation of a feature.
 * @return a cJSON containing the JSON representation of the coordinates of a feature.
 */
cJSON * feature_get_geometry_coordinates(cJSON * feature_json);

/**
 * @brief Gets the type of the geometry associated to a JSON representation of a feature.
 * @param[in] feature_json a cJSON containing the JSON representation of a feature.
 * @return a string containing the type of the geometry associated to a JSON representation of a feature.
 * Possible values are Point, Line, Polygon, MultiPoint, MultiLine and MultiPolygon.
 */
char * feature_get_geometry_type(cJSON * feature_json);

/**
 * @brief Extracts a JSON representation of the first geometry from a JSON representation of a multi
 * feature. The feature must either be a MultiPoint, MultiLine or MultiPolygon.
 * @param[in] feature_json a cJSON containing the JSON representation of a multi feature.
 * @return a cJSON containing the JSON representation of the first geometry from a multi feature.
 */
cJSON * feature_get_first_geometry_coordinates_in_multi(cJSON * feature_json);

/**
 * @brief Extracts a JSON representation of a ring from a JSON representation of a polygon feature.
 * The feature must either be a MultiPolygon or a Polygon.
 * @param[in] feature_json a cJSON containing the JSON representation of a polygon feature.
 * @return a cJSON containing the JSON representation of a ring.
 */
cJSON * feature_get_exterior_ring(cJSON * feature_json);

/**
 * @brief Converts a JSON representation of a point into a point_t.
 * @param[in] json a cJSON containing the JSON representation of a point.
 * @return a point_t containing the point data.
 */
point_t * json_to_point(cJSON * json);

/**
 * @brief Converts a JSON representation of a polygon ring into a list of point_t
 * @param[in] json a cJSON containing the JSON representation of a polygon ring.
 * @return a node_t containing the head of a list of point_t.
 */
node_t * json_ring_to_points(cJSON * json);

#endif //__SIMULATION_LIFECYCLE_FEATURE_H__
