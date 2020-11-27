#ifndef __SIMULATION_LIFECYCLE_MODELS_H__
#define __SIMULATION_LIFECYCLE_MODELS_H__

#include "cJSON.h"

/**
 * @brief parses default configuration parameters common to all models.
 * @param[in] default_config pointer to JSON configuration provided by the user.
 * @param[out] target JSON structure that will contain configuration parameters.
 * @return 0 if it works successfully. Otherwise, it returns the corresponding error code.
 */
int parse_common_default_fields(const cJSON *default_config, cJSON *target);

/**
 * @brief returns pointer to a default configuration parser function.
 * @param[in] model_id string containing the Cell-DEVS model ID.
 * @return pointer to a default configuration function. If model ID couldn't be identified, it returns a NULL pointer.
 */
int (*get_model_default_config_parser(char *model_id))(const cJSON *, cJSON *);

#endif //__SIMULATION_LIFECYCLE_MODELS_H__
