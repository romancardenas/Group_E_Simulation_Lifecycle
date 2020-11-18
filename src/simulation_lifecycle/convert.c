#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "cJSON.h"
#include <dirent.h>
#include <sys/types.h>
#include <sys/stat.h>
#include "simulation_lifecycle/error.h"
#include "simulation_lifecycle/utils/file.h"
#include "simulation_lifecycle/convert.h"

#define CONVERTED_RESULTS_LOG_FILENAME "messages.log"
#define CONVERTED_RESULTS_JSON_FILENAME "structure.json"

int convert_results(char *path_results) {

    /* stat() function and the S_ISDIR() macro on the st_mode field
     * of the stat structure will be used to determine if the path_results
     * exists and if path_results points to a folder. */
    struct stat path_stat;
    stat(path_results, &path_stat);

    if (path_results == NULL) {
        return INPUT_PATH_EMPTY;
    } else if (path_results == "\0") {
        return INPUT_PATH_EMPTY;
        /*This allows to check if path exists and if path points to a folder.*/
    } else if (!S_ISDIR((path_stat.st_mode))) {
        return INPUT_PATH_FOLDER_ERROR;
    } else {
        const char delimiter_period = '.';
        char *p_start_extract = NULL;
        char filename_json[200] = "";
        char filename_txt[200] = "";
        char path_json[200] = "";
        char path_txt[200] = "";
        int count_json_files = 0;
        int count_txt_files = 0;
        int count_log_files = 0;

        /* Opening the directory containing the results.
        * directory_results is a pointer to manage the directory.*/
        DIR *directory_results = opendir(path_results);
        if (directory_results == NULL) {
            printf("\n Could not open the directory containing");
            printf("\n the simulation results.");
            return INPUT_PATH_FOLDER_ERROR;
        }

        /* Searching for .json and .txt files in the folder containing
         * the simulation results. */
        struct dirent *path;
        while ((path = readdir(directory_results)) != NULL) {
            char *filename = path->d_name;

            /* Start extracting characters after "." */
            p_start_extract = strchr(filename, delimiter_period);

            /* p_start_extract == NULL when a filename does not contain
             * a file extension.*/
            if (p_start_extract == NULL) {
                continue;
            }
            if (!strcmp(p_start_extract, ".json")) {
                strcpy(filename_json, filename);
                count_json_files++;
            } else if (!strcmp(p_start_extract, ".txt")) {
                strcpy(filename_txt, filename);
                count_txt_files++;
            } else if (!strcmp(p_start_extract, ".log")) {
                count_log_files++;
                /* When the filename extension is not ".json" or ".txt" */
            } else {
                continue;
            }
        }
        closedir(directory_results);

        /* This function is only executed if path_results only contain
         * one .json file, one .txt file and no .txt file. */
        if (count_txt_files != 1 || count_json_files != 1 || count_log_files != 0) {
            printf("\n Please ensure that your simulation results folder");
            printf("\n contains only one .json file, one .txt file and");
            printf("\n no .log file.");
            return CONVERSION_PATH_FILES_INCORRECT;
        } else {
            strcat(path_json, path_results);
            strcat(path_json, filename_json);
            strcat(path_txt, path_results);
            strcat(path_txt, filename_txt);
            printf("\n txt filename is: %s",path_txt);

            /* The function convert result files only if the path_results
             * has one .txt file using the Cadmium Cell-DEVS format.
             * The third line of any .txt files using the Cadmium Cell-DEVS
             * format should always start as such: [cadmium::celldevs
             * This text extract will be used for string comparison. */
            FILE *f_input_txt = fopen(path_txt, "r");
            char line[200] = "";
            int count_lines = 0;

            /* Loop to get the third line of the .txt file */
            while (count_lines < 3) {
                fgets(line, sizeof(line), f_input_txt);
                count_lines++;
            }

            /* Comparing the 18th first characters of the third line
             * with "[cadmium::celldevs" */
            if (strncmp(line, "[cadmium::celldevs", 18) != 0) {
                printf("\n Please verify that your txt file contain");
                printf("\n the Cadmium Cell-Devs format.");
                return FILE_FORMAT_INCORRECT;
            }

            fclose(f_input_txt);

            /* Calling the function convert_json_file(path_json) to convert
             * the .json into the proper format for the simulation viewer. */
            convert_json_file(path_results,path_json);

            /* Calling the function convert_txt_file(path_txt) to convert
             * the .txt into the proper format for the simulation viewer. */
            convert_txt_file(path_results,path_txt);

            /* Verifying that the results folder now contains the
             * converted results files.
             * Results folder should now contain:
             * 1) 2 x .json file,
             * 2) 1 x .txt file, and
             * 3) 1 x .log file. */
            int count_json_files_after_conversion = 0;
            int count_txt_files_after_conversion = 0;
            int count_log_files_after_conversion = 0;

            DIR *directory_results = opendir(path_results);
            while ((path = readdir(directory_results)) != NULL) {
                char *filename = path->d_name;

                /* Start extracting characters after "." */
                p_start_extract = strchr(filename, delimiter_period);

                /* p_start_extract == NULL when a filename does not
                 * contain an extension.*/
                if (p_start_extract == NULL) {
                    continue;
                }
                if (!strcmp(p_start_extract, ".json")) {
                    count_json_files_after_conversion++;
                } else if (!strcmp(p_start_extract, ".txt")) {
                    count_txt_files_after_conversion++;
                } else if (!strcmp(p_start_extract, ".log")) {
                    count_log_files_after_conversion++;
                    /* When the filename extension is not a
                     * ".json" or ".txt" or ".log". */
                } else {
                    continue;
                }
            }
            closedir(directory_results);
            if (count_json_files_after_conversion == 2 &&
                count_txt_files_after_conversion == 1 &&
                count_log_files_after_conversion == 1) {
                printf("\n Converted results are now found");
                printf("\n in the results folder.");
                return SUCCESS;
            } else {
                printf("\n Something went wrong...");
                printf("\n Expected files are not all in the results folder.");
                return CONVERSION_FAILED;
            }
        }
    }
}

int convert_json_file(char *path_results,char *json_filename) {

    /* Open .json file and start extracting text */

    /* Should use the following existing function: read_json_file */
    FILE *f_input_json = fopen(json_filename, "r");

    if (f_input_json == NULL) {
        printf("\n File could not be opened.");
        return UNABLE_OPEN_FILE;
    }

    fseek(f_input_json, 0, SEEK_END);
    long fsize = ftell(f_input_json);
    fseek(f_input_json, 0, SEEK_SET);

    char *string = malloc(fsize + 1);
    fread(string, 1, fsize, f_input_json);
    fclose(f_input_json);
    string[fsize] = '\0';

    cJSON *input_json = cJSON_Parse(string);
    free(string);

    if (input_json == NULL) {
        printf("\n input_json is NULL.");
        return UNABLE_OPEN_FILE;
    }

    cJSON *scenario_field = NULL;
    cJSON *default_cell_type_field = NULL;
    cJSON *shape_field = NULL;
    cJSON *default_state_field = NULL;

    scenario_field = cJSON_GetObjectItemCaseSensitive(input_json, "scenario");

    /* default_cell_type (equivalent to getName) */
    default_cell_type_field =
            cJSON_GetObjectItemCaseSensitive(scenario_field, "default_cell_type");
    char *default_cell_type_name = cJSON_GetStringValue(default_cell_type_field);

    /* shape (equivalent to getSize) */
    shape_field = cJSON_GetObjectItemCaseSensitive(scenario_field, "shape");
    int size_shape_field = cJSON_GetArraySize(shape_field);
    int shape_value[size_shape_field];
    int *p_shape_value[size_shape_field];
    for (int i = 0; i < size_shape_field; i++) {
        shape_value[i] = cJSON_GetArrayItem(shape_field, i)->valueint;
        p_shape_value[i] = &shape_value[i];
    }

    /* default state (equivalent to getPorts) */
    default_state_field =
            cJSON_GetObjectItemCaseSensitive(scenario_field, "default_state");
    int size_state_field = cJSON_GetArraySize(default_state_field);
    char *state_value[size_state_field][100];
    for (int i = 0; i < size_state_field; i++) {
        state_value[i][100] = cJSON_GetArrayItem(default_state_field, i)->string;
    }

    /* Create new json structure and then write data to new json file */
    char *out;
    cJSON *root;
    cJSON *info;
    cJSON *nodes;
    cJSON *node;
    cJSON *ports;
    cJSON *port;

    root = cJSON_CreateObject();
    info = cJSON_CreateObject();
    nodes = cJSON_CreateArray();
    ports = cJSON_CreateArray();

    /* Info */
    cJSON_AddItemToObject
            (root, "info", info);
    cJSON_AddItemToObject
            (info, "simulator", cJSON_CreateString("Cadmium"));
    cJSON_AddItemToObject
            (info, "name", cJSON_CreateString("transport_output_messages"));
    cJSON_AddItemToObject
            (info, "type", cJSON_CreateString("Cell-DEVS"));

    /* Nodes */
    cJSON_AddItemToObject
            (root, "nodes", nodes);
    cJSON_AddItemToArray
            (nodes, node = cJSON_CreateObject());
    cJSON_AddItemToObject
            (node, "name", cJSON_CreateString(default_cell_type_name));
    cJSON_AddItemToObject
            (node, "type", cJSON_CreateString("coupled"));
    cJSON_AddItemToObject
            (node, "size", cJSON_CreateIntArray(shape_value, size_shape_field));

    /* Ports */
    cJSON_AddItemToObject(root, "ports", ports);
    for (int i = 0; i < size_state_field; i++) {
        cJSON_AddItemToArray
                (ports, port = cJSON_CreateObject());
        cJSON_AddItemToObject
                (port, "model", cJSON_CreateString(default_cell_type_name));
        cJSON_AddItemToObject
                (port, "name", cJSON_CreateString(state_value[i][100]));
        cJSON_AddItemToObject
                (port, "type", cJSON_CreateString("output"));
    }

    out = cJSON_Print(root);

    /* Should use the write_data_to_file function */
    FILE *f_output_json;
    char json_filename1[200] = "";
    strcat(json_filename1, path_results);
    strcat(json_filename1, CONVERTED_RESULTS_JSON_FILENAME);
    f_output_json = fopen(json_filename1, "w");
    fputs(out, f_output_json);
    fclose(f_output_json);
    free(out);
    cJSON_Delete(root);

    return SUCCESS;
}

int convert_txt_file(char *path_results, char *txt_filename) {

    /* Open .txt file to be converted into required format
     * for the simulation viewer. */
    FILE *f_input_txt = fopen(txt_filename, "r");

    if (f_input_txt == NULL) {
        printf("\n File could not be opened.");
        return UNABLE_OPEN_FILE;
    }

    /* Creating and opening new .log file where the converted results
     * will be written. */
    char log_filename[200] = "";
    strcat(log_filename, path_results);
    strcat(log_filename, CONVERTED_RESULTS_LOG_FILENAME);

    FILE *f_output_txt = fopen(log_filename, "w");

    if (f_output_txt == NULL) {
        printf("\n File could not be opened.");
        return UNABLE_OPEN_FILE;
    }

    /* Cadmium results files always contain an extra "0\n" at the beginning.
     * Starts the conversion after this extra "0\n" (equal to two characters). */
    fseek(f_input_txt, 2, SEEK_SET);

    const char start_extract_character = '(';
    const char start_extract_character_2 = '<';
    const char end_extract_character = ')';
    const char end_extract_character_2 = '>';
    const char delimiter_space[] = " ";
    int j = 0;

    /* Looping through each line of the .txt file. */
    while (!feof(f_input_txt)) {
        char line_time[200] = "";
        char line_text[200] = "";
        char *p_start_extract = NULL;
        char *p_end_extract = NULL;
        char *p_start_extract_2 = NULL;
        char *p_end_extract_2 = NULL;
        char data[200] = "";
        char data_2[200] = "";
        int count_characters = 0;
        int line_length = 0;

        fgets(line_text, sizeof(line_text), f_input_txt);

        /* Example of line:
         *
         * [cadmium::celldevs::cell_ports_def<std::vector<int, std::allocator
         * <int> >,co2>::cell_out: {(37,56) ; <-1,-10,2>}] generated by model
         * CO2_model_(37,56)
         *
         * */
        if (line_text[0] == '[') {

            /* Get the coords:
             * (i.e., in the example, the coords are 37 and 56). */
            p_start_extract = strchr(line_text, start_extract_character);
            p_end_extract = strchr(line_text, end_extract_character);
            count_characters = p_end_extract - p_start_extract;
            strncpy(data, p_start_extract + 1, count_characters - 1);

            for (int k = 0; k <= strlen(data); k++) {
                if (data[k] == '(' || data[k] == ',') {
                    data[k] = ' ';
                }
            }

            char *ptr = strtok(data, delimiter_space);
            char **coords = NULL;
            int n_spaces1 = 0;

            while (ptr) {
                coords = realloc(coords, sizeof(char *) * ++n_spaces1);
                if (coords == NULL){
                    exit(-1);
                }
                coords[n_spaces1 - 1] = ptr;
                ptr = strtok(NULL, " ");
            }

            coords = realloc(coords, sizeof(char *) * (n_spaces1 + 1));
            coords[n_spaces1] = 0;

            if (n_spaces1 == 2) {
                coords[2] = "0";
            }

            /* Get the values:
             * (i.e., in the example, the values are -1,-10, and 2). */
            p_start_extract_2 = strchr(p_end_extract, start_extract_character_2);
            p_end_extract_2 = strchr(p_end_extract, end_extract_character_2);
            count_characters = p_end_extract_2 - p_start_extract_2;
            strncpy(data_2, p_start_extract_2 + 1, count_characters - 1);

            for (int n = 0; n <= strlen(data_2); n++) {
                if (data_2[n] == ',') {
                    data_2[n] = ' ';
                }
            }

            char *ptr2 = strtok(data_2, delimiter_space);
            char **values = NULL;
            int n_spaces = 0;

            while (ptr2) {
                values = realloc(values, sizeof(char *) * ++n_spaces);
                if (values == NULL){
                    exit(-1);
                }
                values[n_spaces - 1] = ptr2;
                ptr2 = strtok(NULL, " ");
            }

            values = realloc(values, sizeof(char *) * (n_spaces + 1));
            values[n_spaces] = 0;

            for (int q = 0; q < n_spaces; q++) {
                fputs(";", f_output_txt);
                fputs(coords[0], f_output_txt);
                fputs(",", f_output_txt);
                fputs(coords[1], f_output_txt);
                fputs(",", f_output_txt);
                fputs(coords[2], f_output_txt);
                fputs(",", f_output_txt);
                fprintf(f_output_txt, "%d", q);
                fputs(",", f_output_txt);
                fputs(values[q], f_output_txt);
            }

            free(coords);
            free(values);

        } else if (j == 0) {
            line_length = strlen(line_text);
            line_text[line_length - 1] = '\0';
            strcpy(line_time, line_text);
            fputs(line_time, f_output_txt);
        } else {
            line_length = strlen(line_text);
            line_text[line_length - 1] = '\0';
            strcpy(line_time, line_text);
            fputs("\n", f_output_txt);
            fputs(line_time, f_output_txt);
        }
        j++;
    }
    fclose(f_input_txt);
    fclose(f_output_txt);
    return SUCCESS;
}