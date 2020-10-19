#include <stdio.h>
#include <gdal.h>
#include <cpl_conv.h>

/**
 * Simulation Lifecycle: main function. So far, it i just a simple example to check if GDAL is installed properly.
 * @param argc number of arguments
 * @param argv value of the arguments
 * @return always 0
 */
int main(int argc, char *argv[]) {
    char *pszFilename = "../data/example.geojson";
    GDALDatasetH hDataSet;

    if (argc != 2) {
        fprintf(stderr, "Wrong number of arguments. Usage: %s PATH_TO_RASTER_IMAGE\n", argv[0]);
        return -1;
    }

    GDALAllRegister();
    hDataSet = GDALOpen(argv[1], GA_ReadOnly);
    if (hDataSet != NULL) {
        puts("Input Raster read successfully");
        return 0;
    } else {
        fprintf(stderr, "Unable to read raster file %s\n", argv[1]);
        return -1;
    }
}