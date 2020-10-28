#include <unistd.h>
#include "simulation_lifecycle/utils.h"

inline int file_exists(const char *file_name) {
    return access(file_name, F_OK) != -1;
}
