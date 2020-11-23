#ifndef __SIMULATION_LIFECYCLE_FILE_H__
#define __SIMULATION_LIFECYCLE_FILE_H__

#include <unistd.h>
#include "simulation_lifecycle/structures.h"

/**
 * @brief checks if there is an existing file.
 * @param[in] file_name path to the file to be checked.
 * @return 1 if file does not exist.
 */
static inline int file_exists(const char *file_name) {
    return access(file_name, F_OK) != -1;
}

/**
 * @brief writes string to file.
 * @param filepath path to output file.
 * @param data string containing the data to be written on file.
 * @return 0 if data is successfully written on file. Otherwise, it returns an error code (see error.h)
 */
int write_data_to_file(char *filepath, char *data);

/**
 * @brief reads a file as a string.
 * @param path_to_file path to input file.
 * @param pp_data a pointer to a string that will contain the content of the input file.
 * @return 0 if the file is successfully read. Otherwise, it returns an error code (see error.h)
 */
int read_file(char * path_to_file, char ** pp_data);

/**
 * @brief reads a json file as a cJSON struct.
 * @param path_to_file path to input json file.
 * @param pp_data a pointer to a cJSON struct that will contain the content of the input file.
 * @return 0 if the file is successfully read. Otherwise, it returns an error code (see error.h)
 */
int read_json_file(char * path_to_file, cJSON ** pp_data);

#endif //__SIMULATION_LIFECYCLE_FILE_H__
