#ifndef SIMULATION_LIFECYCLE_FILE_H
#define SIMULATION_LIFECYCLE_FILE_H

#include <unistd.h>

/**
 * Checks if there is an existing file.
 * @param[in] file_name path to the file to be checked.
 * @return 1 if file does not exist.
 */
static inline int file_exists(const char *file_name) {
    return access(file_name, F_OK) != -1;
}

#endif //SIMULATION_LIFECYCLE_FILE_H
