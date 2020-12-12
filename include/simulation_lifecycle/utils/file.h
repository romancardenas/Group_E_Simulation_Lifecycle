#ifndef __SIMULATION_LIFECYCLE_FILE_H__
#define __SIMULATION_LIFECYCLE_FILE_H__

#include <unistd.h>
#include <cJSON.h>

/**
 * @brief Checks if there is an existing file with read permissions.
 * @param[in] file_name path to the file to be checked.
 * @return 1 if file does not exist.
 */
static inline int file_exists(const char *file_name) {
    return file_name != NULL && access(file_name, F_OK) != -1;
}

/**
 * @brief Checks if there is an existing file with execute permissions.
 * @param[in] file_name path to the file to be checked.
 * @return 1 if file does not exist.
 */
static inline int executable_exists(const char *file_name) {
    return file_name != NULL && access(file_name, X_OK) != -1;
}

/**
 * @brief Writes string to file.
 * @param[in] filepath path to output file.
 * @param[in] data string containing the data to be written on file.
 * @return 0 if data is successfully written on file. Otherwise, it returns an error code (see error.h).
 */
int write_data_to_file(char *filepath, char *data);

/**
 * @brief Reads a file as a string.
 * @param[in] path_to_file path to input file.
 * @param[out] pp_data a pointer to a string that will contain the content of the input file.
 * @return 0 if the file is successfully read. Otherwise, it returns an error code (see error.h).
 */
int read_file(char * path_to_file, char ** pp_data);

/**
 * @brief Reads a json file as a cJSON struct.
 * @param[in] path_to_file path to input json file.
 * @param[out] pp_data a pointer to a cJSON struct that will contain the content of the input file.
 * @return 0 if the file is successfully read. Otherwise, it returns an error code (see error.h)
 */
int read_json_file(char * path_to_file, cJSON ** pp_data);

#endif //__SIMULATION_LIFECYCLE_FILE_H__
