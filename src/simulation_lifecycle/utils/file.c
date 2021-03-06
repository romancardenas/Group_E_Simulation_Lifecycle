#include <stdio.h>
#include <stdlib.h>
#include <cJSON.h>
#include <string.h>
#include "simulation_lifecycle/error.h"
#include "simulation_lifecycle/utils/file.h"

int write_data_to_file(char * file_path, char *data) {
    FILE *f_out;

    int res = SUCCESS;

    if (file_exists(file_path)) {
        res = FILE_EXISTS_ERROR;
    } else if ((f_out = fopen(file_path, "w")) == NULL) {
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

int read_file(char * path_to_file, char ** pp_data) {
    FILE *f;

    int res = SUCCESS;

    if (!file_exists(path_to_file)) {
        res = FILE_DOES_NOT_EXIST;
    } else if ((f = fopen(path_to_file, "rb")) == NULL) {
        res = UNABLE_OPEN_FILE;
    } else {
        fseek(f, 0, SEEK_END);
        long fsize = ftell(f);
        fseek(f, 0, SEEK_SET);  /* same as rewind(f); */

        *pp_data = malloc(fsize + 1);
        fread(*pp_data, 1, fsize, f);

        if (fclose(f) != 0) {
            res = UNABLE_CLOSE_FILE;
        }

        (*pp_data)[fsize] = '\0';
    }

    return res;
}

int read_json_file(char * path_to_file, cJSON ** pp_data) {
    char * content = NULL;

    int res = read_file(path_to_file, &content);

    if (res != SUCCESS) {
        return res;
    }

    *pp_data = cJSON_Parse(content);

    if (*pp_data == NULL) {
        res = JSON_UNABLE_TO_PARSE;
    }

    return res;
}

void join_paths(char * out, char * a, char * b) {
    size_t n = strlen(a);
    char last = a[n-1];

    if (last == '/' || last == '\\') {
        a[n-1] = '\0';
    }

    snprintf(out, strlen(a) + strlen(b) + 2, "%s/%s", a, b);
}

int copy_file(char * source, char * target) {
    int ch;

    FILE * f_source = fopen(source, "r");

    if (f_source == NULL) {
        return UNABLE_OPEN_FILE;
    }

    FILE * f_target = fopen(target, "w");

    if (f_target == NULL) {
        return UNABLE_OPEN_FILE;
    }

    while ((ch = fgetc(f_source)) != EOF) {
        fputc(ch, f_target);
    }

    fclose(f_source);
    fclose(f_target);

    return SUCCESS;
}
