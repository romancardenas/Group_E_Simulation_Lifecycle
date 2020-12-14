#ifndef __SIMULATION_LIFECYCLE_STRINGS_H__
#define __SIMULATION_LIFECYCLE_STRINGS_H__

/**
 * @brief Concatenates two strings.
 * @param[in] s1 first string. It cannot be NULL.
 * @param[in] s2 second string. It cannot be NULL.
 * @return pointer to concatenated string. If concatenation failed, it returns a NULL pointer.
 * NOTE: It is located in the heap, you need to call the free function.
 */
char* concat(const char *s1, const char *s2);

#endif //__SIMULATION_LIFECYCLE_STRINGS_H__
