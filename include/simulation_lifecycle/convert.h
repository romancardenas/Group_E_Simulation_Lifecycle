#ifndef __SIMULATION_LIFECYCLE_CONVERT_H__
#define __SIMULATION_LIFECYCLE_CONVERT_H__

#define CONVERTED_RESULTS_LOG_FILENAME "messages.log"
#define CONVERTED_RESULTS_JSON_FILENAME "structure.json"

/**
 * @brief returns the output path as a string from the conversion json.
 * @param[in] conv, a cJSON representation of the conversion section in the workflow.
 * @return a string containing the output path
 */
char * get_output_path(cJSON * conv);

/**
 * @brief returns the output path as a string from the conversion json.
 * @param[in] conv, a cJSON representation of the conversion section in the workflow.
 * @return a string containing the output path
 */
char * get_input_path(cJSON * conv);

/**
 * @brief convert irregular cell-DEVS simulation results .json file into the required
 * format for the simulation viewer.
 * @param[in] path_results string containing the path to the results folder.
 * @param[in] json_filename string containing the .json filename.
 * @param[in] visualization JSON structure containing the visualization part of the workflow.
 * @return 0 if the function ran successfully. Otherwise, it returns an error
 * code.
 */
int convert_json_file(char *path_results, char *json_filename, cJSON *visualization);

/**
 * @brief convert irregular cell-DEVS simulation results .txt file into the required
 * format for the simulation viewer.
 * @param[in] path_results string containing the path to the results folder.
 * @param[in] txt_filename string containing the .txt filename.
 * @return 0 if the function ran successfully. Otherwise, it returns an error
 * code.
 */
int convert_txt_file(char *path_results, char *txt_filename);

/**
 * @brief convert irregular cell-DEVS simulation results files into the required
 * format for the simulation viewer.
 * @param[in] output string containing the path to the folder where converted results will
 * be saved.
 * @param[in] visualization JSON structure containing the visualization part of the workflow.
 * @return 0 if the function ran successfully. Otherwise, it returns an error
 * code.
 */
int convert_results(char *output_folder, cJSON *visualization);

#endif //__SIMULATION_LIFECYCLE_CONVERT_H__
