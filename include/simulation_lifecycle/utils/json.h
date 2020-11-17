#ifndef __SIMULATION_LIFECYCLE_JSON_H__
#define __SIMULATION_LIFECYCLE_JSON_H__

#include "cJSON.h"
#include "simulation_lifecycle/structures.h"

/**
 * @brief reads a field from a cJSON struct.
 * @param p_json a cJSON struct to read the field from.
 * @param field_name a string containing the name of a field.
 * @return a cJSON struct that contains the field as cJSON
 */
cJSON * json_read_field(cJSON * p_json, char * field_name);

/**
 * @brief reads the content of a field from a cJSON struct.
 * @param p_json a cJSON struct to read the content of a field from.
 * @param field_name a string containing the name of a field.
 * @return a cJSON struct that contains the content of a field as cJSON
 */
cJSON * json_read_object(cJSON * p_json, char * field_name);

/**
 * @brief reads a string field from a cJSON struct.
 * @param p_json a cJSON struct to read the string field from.
 * @param field_name a string containing the name of a field.
 * @return a string that contains the string field.
 */
char * json_read_string(cJSON * p_json, char * field_name);

/**
 * @brief reads a feature_t struct from a cJSON struct.
 * @param p_json a cJSON struct that will be parsed to a feature_t struct.
 * @return a feature_t initialized from the cJSON struct.
 */
feature_t * feature_from_json(cJSON * p_json) ;

/**
 * @brief reads a feature_set_t struct from a cJSON struct.
 * @param p_json a cJSON struct that will be parsed to a feature_set_t struct.
 * @return a feature_set_t initialized from the cJSON struct.
 */
feature_set_t * feature_set_from_json(cJSON * p_json) ;

#endif //__SIMULATION_LIFECYCLE_JSON_H__
