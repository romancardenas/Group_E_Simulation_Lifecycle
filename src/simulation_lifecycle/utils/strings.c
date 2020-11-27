#include <string.h>
#include <stdlib.h>
#include "simulation_lifecycle/utils/strings.h"

char * concat(const char *s1, const char *s2) {
    const size_t len1 = strlen(s1);
    const size_t len2 = strlen(s2);
    char *result = malloc(len1 + len2 + 1); // +1 for the null-terminator
    if (result != NULL) {
        memcpy(result + len1, s2, len2 + 1); // +1 to copy the null-terminator
        memcpy(result, s1, len1);
    }
    return result;
}