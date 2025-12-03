#include <assert.h>
#include <libgen.h>
#include <omp.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

#include "DIPs.h"
#include "FileIO.h"
#include "Image.h"
#include "utils.h"

#define NUM_THREADS 0
#define LOGGING 1

int main(int argc, char **argv) {
    omp_set_nested(1);
    omp_set_max_active_levels(2);

    if (argc == 1) {
        fprintf(stderr, "Parallel Digital Image Processing (TASKS)\n");
        fprintf(stderr, "Usage: %s [FILE_NAME...]\n", argv[0]);
        return 1;
    }

    if (check_input_files(argc, argv)) return 1;

    // Use the maximum number of available processors
    const int num_procs = omp_get_num_procs();
    const int thread_count = (NUM_THREADS > 0) ? NUM_THREADS : num_procs;
    omp_set_num_threads(thread_count);
    const int function_counts = 3; // 3 DIP functions per image
    const int num_files = argc - 1;

    int threads_per_file[num_files];
    compute_thread_split(num_procs, num_files, threads_per_file);

    #if LOGGING
    const int num_images = argc - 1;
    printf("Using %d / %d available threads to process %d images...\n", thread_count, num_procs, num_images);
    printf("Each image is allocated %d threads as it's being loaded in. One thread per DIP function\n", function_counts);
    const int thread_limit = omp_get_thread_limit();
    if (thread_count > thread_limit) {
        printf("Warning: The requested thread count (%d) is greater than the thread limit (%d)\n", thread_count, thread_limit);
    }
    double start_time = omp_get_wtime();
    #endif

    #pragma omp parallel num_threads(thread_count)
    #pragma omp single
    {
        for (int i = 1; i < argc; i++) {
            #pragma omp task firstprivate(i)
            {
                const char *file_name = argv[i];
                char *fname_copy_base = strdup(file_name);
                char *base_name = basename(fname_copy_base);
                char *fname_copy_path = strdup(file_name);
                char *file_path = dirname(fname_copy_path);
                char output_file[512] = {0};
                #pragma omp parallel sections num_threads(3)
                {
                    #pragma omp section
                    {
                        // Load image
                        IMAGE *image = NULL;
                        image = LoadImage(file_name);
                        assert(image);
                        // Convert to black and white
                        image = BlackNWhite(image);
                        build_file_name(file_path, base_name, "_bw", FILE_EXT, output_file, sizeof(output_file));
                        assert(!SaveImage(output_file, image));
                        DeleteImage(image);
                    }
                    #pragma omp section
                    {
                        // Load image
                        IMAGE *image = NULL;
                        image = LoadImage(file_name);
                        assert(image);
                        // Convert to black and white
                        image = Sharpen(image);
                        build_file_name(file_path, base_name, "_bw", FILE_EXT, output_file, sizeof(output_file));
                        assert(!SaveImage(output_file, image));
                        DeleteImage(image);
                    }
                    #pragma omp section
                    {
                        // Load image
                        IMAGE *image = NULL;
                        image = LoadImage(file_name);
                        assert(image);
                        // Convert to black and white
                        image = VFlip(image);
                        build_file_name(file_path, base_name, "_bw", FILE_EXT, output_file, sizeof(output_file));
                        assert(!SaveImage(output_file, image));
                        DeleteImage(image);
                    }
                }
            }
        }
    }
    #if LOGGING
    double end_time = omp_get_wtime();
    double elapsed_time = end_time - start_time;
    printf("Elapsed time: %.5f s\n", elapsed_time);
    #endif

    return 0;
}
