#ifndef __SIMULATION_LIFECYCLE_CONVERT_H__
#define __SIMULATION_LIFECYCLE_CONVERT_H__

/**
 * @brief convert the cell-DEVS simulation results .json file into the required
 * format for the simulation viewer.
 * @param[in] path_results string containing the path to the results folder.
 * @param[in] json_filename string containing the .json filename.
 * @return 0 if the function ran successfully. Otherwise, it returns an error
 * code.
 */
int convert_json_file(char *path_results, char *json_filename);

/**
 * @brief convert the cell-DEVS simulation results .txt file into the required
 * format for the simulation viewer.
 * @param[in] path_results string containing the path to the results folder.
 * @param[in] txt_filename string containing the .txt filename.
 * @return 0 if the function ran successfully. Otherwise, it returns an error
 * code.
 */
int convert_txt_file(char *path_results, char *txt_filename);

/**
 * @brief convert the cell-DEVS simulation results files into the required
 * format for the simulation viewer.
 * @param[in] path_results string containing the results folder path containing the
 * simulation results.
 * @return 0 if the function ran successfully. Otherwise, it returns an error
 * code.
 */
int convert_results(char *path_results);


#endif //__SIMULATION_LIFECYCLE_CONVERT_H__