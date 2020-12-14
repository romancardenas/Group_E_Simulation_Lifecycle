#include <stdio.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <string.h>
#include <cJSON.h>
#include <dirent.h>
#include <limits.h>
#include "simulation_lifecycle/error.h"
#include "simulation_lifecycle/convert.h"
#include "simulation_lifecycle/utils/file.h"

#define VIZ_OBJECT "fields"
#define RESULT_OBJECT "cells"
#define INPUT_PATH "simulation/"
#define OUTPUT_PATH "conversion/"


char * get_output_path(cJSON * conv) {
    cJSON * path = cJSON_GetObjectItem(conv, "output");

    return (path == NULL) ? NULL : cJSON_GetStringValue(path);
}

char * get_input_path(cJSON * conv) {
    cJSON * path = cJSON_GetObjectItem(conv, "input");

    return (path == NULL) ? NULL : cJSON_GetStringValue(path);
}

int convert_results(char *output_folder, cJSON *visualization) {
    if (output_folder == NULL || strlen(output_folder) == 0) {
        return CONVERT_OUTPUT_PATH_INCORRECT;
    }

    char input[PATH_MAX] = "";
    join_paths(input, output_folder, INPUT_PATH);

    char output[PATH_MAX] = "";
    join_paths(output, output_folder, OUTPUT_PATH);
    mkdir(output, 0777);

    /* Opening the directory containing the results.
    * directory_results is a pointer to manage the directory.*/
    DIR *p_directory_results = opendir(input);

    if (p_directory_results == NULL) {
        return CONVERT_INPUT_PATH_INCORRECT;
    }

    const char delimiter_period = '.';
    char *start_extract = NULL;
    char filename_json[NAME_MAX] = "";
    char filename_txt[NAME_MAX] = "";
    char path_json[PATH_MAX] = "";
    char path_txt[PATH_MAX] = "";
    int count_json_files = 0;
    int count_txt_files = 0;
    int count_log_files = 0;

    /* Searching for .json and .txt files in the folder containing
    * the simulation results. */
    struct dirent *path;
    while ((path = readdir(p_directory_results)) != NULL) {
        char *filename = path->d_name;

        /* Start extracting characters after "." */
        start_extract = strchr(filename, delimiter_period);

        /* start_extract == NULL when a filename does not contain
        * a file extension.*/
        if (start_extract == NULL) {
            continue;
        }
        if (!strcmp(start_extract, ".json")) {
            strcpy(filename_json, filename);
            count_json_files++;
        } else if (!strcmp(start_extract, ".txt")) {
            strcpy(filename_txt, filename);
            count_txt_files++;
        } else if (!strcmp(start_extract, ".log")) {
            count_log_files++;
            /* When the filename extension is not ".json" or ".txt" */
        } else {
            continue;
        }
    }
    closedir(p_directory_results);

    /* This function is only executed if path_results only contain
     * one .json file, one .txt file and no .log file.
     * This avoids overwriting over previous simulation results. */
    if (count_txt_files != 1 || count_json_files != 1 || count_log_files != 0) {
        return CONVERT_PATH_FILES_INCORRECT;
    } 
    else {
        snprintf(path_json, sizeof(path_json), "%s%s", input, filename_json);
        snprintf(path_txt, sizeof(path_json), "%s%s", input, filename_txt);

        /* The function convert result files only if the path_results
         * has one .txt file using the irregular Cadmium Cell-DEVS format.
         * The third line of any .txt files using the irregular Cadmium Cell-DEVS
         * format should always start as such:
         * "[cadmium::celldevs::cell_ports_def<std::string"
         * This text extract will be used for string comparison. */
        FILE *f_input_txt = fopen(path_txt, "r");
        char line[LINE_MAX] = "";
        int count_lines = 0;

        /* Loop to get the third line of the .txt file */
        while (count_lines < 3) {
            fgets(line, sizeof(line), f_input_txt);
            count_lines++;
        }

        fclose(f_input_txt);

        /* Calling the function convert_json_file to convert
         * the .json into the proper format for the simulation viewer. */
        int res = convert_json_file(output, path_json, visualization);
        if(res != 0){
            return res;
        }

        /* Calling the function convert_txt_file to convert
         * the .txt into the proper format for the simulation viewer. */
        res = convert_txt_file(output, path_txt);

        if(res != 0){
            return res;
        }

        /* Verifying that the results folder now contains the
         * converted results files.
         * Results folder should now contain:
         * 1) 2 x .json file,
         * 2) 1 x .txt file, and
         * 3) 1 x .log file. */
        if (!file_exists(path_txt) || !file_exists(path_json)) {
            res = CONVERSION_FAILED;
        }

        return res;

    }
}

int convert_json_file(char *path_results, char *json_filename, cJSON *visualization) {

    /* Open .json file and start extracting text */
    FILE *f_input_json = fopen(json_filename, "r");

    if (f_input_json == NULL) {
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
        return UNABLE_OPEN_FILE;
    }

    char *fields = cJSON_GetStringValue(cJSON_GetObjectItemCaseSensitive(visualization, VIZ_OBJECT));
    if (fields == NULL || fields[0] == '\0') {
        return CONVERT_VIZ_FIELDS_INVALID;
    }

    /* Get "cells" sub object */
    cJSON *cells_object = NULL;
    cells_object = cJSON_GetObjectItem(input_json, RESULT_OBJECT);
    int size_cell_object = cJSON_GetArraySize(cells_object);
    char cell[size_cell_object][LINE_MAX];

    int i = 0;
    if (cells_object){
        cJSON *cells_sub_object = cells_object->child;
        while(cells_sub_object){
            strcpy(cell[i],cells_sub_object->string);
            cells_sub_object = cells_sub_object->next;
            i++;
        }
    }

    /* Create new json structure. Then, write data to new json file. */
    char *out;
    cJSON *root;
    cJSON *info;
    cJSON *nodes;
    cJSON *node;

    root = cJSON_CreateObject();
    info = cJSON_CreateObject();
    nodes = cJSON_CreateArray();

    cJSON_AddItemToObject
            (root, "info", info);
    cJSON_AddItemToObject
            (info, "simulator", cJSON_CreateString("Cadmium"));
    cJSON_AddItemToObject
            (info, "name", cJSON_CreateString("Messages"));
    cJSON_AddItemToObject
            (info, "type", cJSON_CreateString("Irregular Cell-DEVS"));


    cJSON_AddItemToObject(root, "nodes", nodes);
    for (int i = 1; i < size_cell_object; i++) {
        cJSON_AddItemToArray
                (nodes, node = cJSON_CreateObject());
        cJSON_AddItemToObject
                (node, "name", cJSON_CreateString(cell[i]));
        cJSON_AddItemToObject
                (node, "type", cJSON_CreateString("atomic"));
        cJSON_AddItemToObject
                (node, "template", cJSON_CreateStringReference(fields));
    }

    out = cJSON_Print(root);

    FILE *f_output_json;
    char json_filename1[NAME_MAX] = "";
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
        return UNABLE_OPEN_FILE;
    }

    /* Creating and opening new .log file where the converted results
     * will be written. */
    char log_filename[NAME_MAX] = "";
    sprintf(log_filename, "%s%s", path_results, CONVERTED_RESULTS_LOG_FILENAME);

    FILE *f_output_txt = fopen(log_filename, "w");
    if (f_output_txt == NULL) {
        return UNABLE_OPEN_FILE;
    }

    /* Cadmium results files always contain an extra "0\n" at the beginning.
     * Starts the conversion after this extra "0\n" which is equal to two characters. */
    fseek(f_input_txt, 2, SEEK_SET);

    const char start_extract_character = '<';
    const char end_extract_character = '>';
    int line = 0;
    int count = 0;

    /* Looping through each line of the .txt file. */
    while (!feof(f_input_txt)) {
        char line_time[LINE_MAX] = "";
        char line_text[LINE_MAX] = "";
        char *start_extract = NULL;
        char *end_extract = NULL;
        char data[LINE_MAX] = "";
        int count_characters = 0;
        int line_length = 0;

        fgets(line_text, sizeof(line_text), f_input_txt);

        /* Example of line:
         *
         * [cadmium::celldevs::cell_ports_def<std::string, sir>::cell_out:
         * {Alta Vista ; <1,0.485,0.515,0,0.515,0,0>}] generated by model _Alta Vista
         *
         * */
        if (line_text[0] == '[') {

            /* Get values:
             * (i.e., in the example, the values are 1,0.485,0.515,0,0.515,0,0). */
            start_extract = strrchr(line_text, start_extract_character);
            end_extract = strrchr(line_text, end_extract_character);
            count_characters = end_extract - start_extract;
            strncpy(data, start_extract + 1, count_characters - 1);
            fprintf(f_output_txt, ";%d,%s", count, data);
            count++;

        } else {
            count = 0;
            line_length = strlen(line_text);
            line_text[line_length - 1] = '\0';
            strcpy(line_time, line_text);
            if (line != 0) {
                fputs("\r\n", f_output_txt);
                fputs(line_time, f_output_txt);
            } else {
                fputs(line_time, f_output_txt);
            }
        }
        line++;
    }

    fclose(f_input_txt);
    fclose(f_output_txt);

    return SUCCESS;
}