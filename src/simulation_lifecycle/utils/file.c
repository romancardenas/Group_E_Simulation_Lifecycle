#include <stdio.h>
#include <stdlib.h>
#include "cJSON.h"
#include "simulation_lifecycle/error.h"
#include "simulation_lifecycle/utils/file.h"

int write_data_to_file(char *filepath, char *data) {
    FILE *f_out;
    int res = SUCCESS;
    if (file_exists(filepath)) {
        res = FILE_EXISTS_ERROR;
    } else if ((f_out = fopen(filepath, "w")) == NULL) {
        res = UNABLE_OPEN_FILE;
    } else {
        if (fputs (data, f_out) == EOF) {
            res = UNABLE_WRITE_FILE;
        }
        if (fclose(f_out) != 0) {
            res = UNABLE_CLOSE_FILE;
        }
    }
    return res;
}

void read_json_file(char *file_path, cJSON **p_target) {
    FILE *f = fopen(file_path, "rb");
    fseek(f, 0, SEEK_END);
    long fsize = ftell(f);
    fseek(f, 0, SEEK_SET);  /* same as rewind(f); */

    char *string = malloc(fsize + 1);
    fread(string, 1, fsize, f);
    fclose(f);
    string[fsize] = '\0';

    *p_target = cJSON_Parse(string);
    free(string);
}