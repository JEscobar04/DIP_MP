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

const char *FILE_EXT = ".ppm";

int main(int argc, char **argv) {
    if (argc == 1) {
        fprintf(stderr, "Parallel Digital Image Processing\n");
        fprintf(stderr, "Usage: %s [FILE_NAME...]\n", argv[0]);
        return 1;
    }

    // Ensure all files exist and have the correct extension
    for (int i = 1; i < argc; i++) {
        char *file_name = argv[i];
        int len = strlen(file_name);
        bool exists = access(file_name, F_OK) != -1;
        if (len > 4) {
            bool has_correct_ext = (strcmp(&file_name[len - 4], FILE_EXT) == 0);
            if (has_correct_ext && exists) continue;
        }
        fprintf(stderr, "Error: invalid file name: %s\n", file_name);
        return 1;
    }

    // Use the maximum number of available processors
    int num_procs = omp_get_num_procs();
    omp_set_num_threads(num_procs);

#pragma omp parallel for
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

    return 0;
}