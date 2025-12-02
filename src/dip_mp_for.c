#include <assert.h>
#include <libgen.h>
#include <omp.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "DIPs.h"
#include "FileIO.h"
#include "Image.h"
#include "utils.h"

#define NUM_THREADS 0
#define LOGGING 1

int main(int argc, char **argv) {
    if (argc == 1) {
        fprintf(stderr, "Parallel Digital Image Processing (FOR)\n");
        fprintf(stderr, "Usage: %s [FILE_NAME...]\n", argv[0]);
        return 1;
    }

    if (check_input_files(argc, argv)) return 1;

    // Use the maximum number of available processors if unspecified
    const int num_procs = omp_get_num_procs();
    int thread_count = (NUM_THREADS > 0) ? NUM_THREADS : num_procs;
    omp_set_num_threads(thread_count);
#if LOGGING
    const int num_images = argc - 1;
    printf("Using %d / %d available threads to process %d images...\n", thread_count, num_procs, num_images);
    double start_time = omp_get_wtime();
#endif

#pragma omp parallel for schedule(guided)
    for (int i = 1; i < argc; i++) {
        const char *file_name = argv[i];
        char *fname_copy_base = strdup(file_name);
        char *base_name = basename(fname_copy_base);
        char *fname_copy_path = strdup(file_name);
        char *path = dirname(fname_copy_path);
        char output_file[512] = {0};
        // Load image
        IMAGE *image = NULL;
        image = LoadImage(file_name);
        assert(image);
        // Convert to black and white
        image = BlackNWhite(image);
        build_file_name(path, base_name, "_bw", FILE_EXT, output_file, sizeof(output_file));
        assert(!SaveImage(output_file, image));
        DeleteImage(image);
        // Apply sharpening effect
        image = LoadImage(file_name);
        assert(image);
        image = Sharpen(image);
        build_file_name(path, base_name, "_sharpened", FILE_EXT, output_file, sizeof(output_file));
        assert(!SaveImage(output_file, image));
        DeleteImage(image);
        // Vertically flip
        image = LoadImage(file_name);
        assert(image);
        image = VFlip(image);
        build_file_name(path, base_name, "_vflipped", FILE_EXT, output_file, sizeof(output_file));
        assert(!SaveImage(output_file, image));
        DeleteImage(image);
        // Free memory
        free(fname_copy_base);
        free(fname_copy_path);
    }

#if LOGGING
    double end_time = omp_get_wtime();
    double elapsed_time = end_time - start_time;
    printf("Elapsed time: %.5f s\n", elapsed_time);
#endif

    return 0;
}