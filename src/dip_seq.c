#include <assert.h>
#include <libgen.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>

#include "DIPs.h"
#include "FileIO.h"
#include "Image.h"
#include "utils.h"

#define LOGGING 1

int main(int argc, char **argv) {
    if (argc == 1) {
        fprintf(stderr, "Sequential Digital Image Processing\n");
        fprintf(stderr, "Usage: %s [FILE_NAME...]\n", argv[0]);
        return 1;
    }

    if (check_input_files(argc, argv)) return 1;

#if LOGGING
    const int num_images = argc - 1;
    printf("Processing %d images...\n", num_images);
    struct timespec start_time;
    clock_gettime(CLOCK_MONOTONIC, &start_time);
#endif

    for (int i = 1; i < argc; i++) {
        const char *file_name = argv[i];
        char *fname_copy_base = strdup(file_name);
        char *base_name = basename(fname_copy_base);
        char *fname_copy_path = strdup(file_name);
        char *file_path = dirname(fname_copy_path);
        char output_file[512] = {0};
        // Load image
        IMAGE *image = NULL;
        image = LoadImage(file_name);
        assert(image);
        // Convert to black and white
        image = BlackNWhite(image);
        build_file_name(file_path, base_name, "_bw", FILE_EXT, output_file, sizeof(output_file));
        assert(!SaveImage(output_file, image));
        DeleteImage(image);
        // Apply sharpening effect
        image = LoadImage(file_name);
        assert(image);
        image = Sharpen(image);
        build_file_name(file_path, base_name, "_sharpened", FILE_EXT, output_file, sizeof(output_file));
        assert(!SaveImage(output_file, image));
        DeleteImage(image);
        // Vertically flip
        image = LoadImage(file_name);
        assert(image);
        image = VFlip(image);
        build_file_name(file_path, base_name, "_vflipped", FILE_EXT, output_file, sizeof(output_file));
        assert(!SaveImage(output_file, image));
        DeleteImage(image);
        // Free memory
        free(fname_copy_base);
        free(fname_copy_path);
    }

#if LOGGING
    struct timespec end_time;
    clock_gettime(CLOCK_MONOTONIC, &end_time);
    double elapsed_time = (end_time.tv_sec - start_time.tv_sec) + (end_time.tv_nsec - start_time.tv_nsec) * 1e-9;
    printf("Elapsed time: %.5f s\n", elapsed_time);
#endif

    return 0;
}