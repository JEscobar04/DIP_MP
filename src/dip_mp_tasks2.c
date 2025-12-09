#include <assert.h>
#include <libgen.h>
#include <omp.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>

#include "DIPs.h"
#include "FileIO.h"
#include "Image.h"
#include "utils.h"


#define NUM_THREADS 0
#define LOGGING 1

static void run_section(int lane, const char *file_name, const char *file_path, const char *base_name)
{
    char output_file[512] = {0};

    if (lane == 0) {
        IMAGE *image = LoadImage(file_name);
        assert(image);
        image = BlackNWhite(image);
        build_file_name(file_path, base_name, "_bw", FILE_EXT, output_file, sizeof(output_file));
        assert(!SaveImage(output_file, image));
        DeleteImage(image);
    } 
    else if (lane == 1) {
        IMAGE *image = LoadImage(file_name);
        assert(image);
        image = Sharpen(image);
        build_file_name(file_path, base_name, "_sharpen", FILE_EXT, output_file, sizeof(output_file));
        assert(!SaveImage(output_file, image));
        DeleteImage(image);
    } 
    else if (lane == 2) {
        IMAGE *image = LoadImage(file_name);
        assert(image);
        image = VFlip(image);
        build_file_name(file_path, base_name, "_vflip", FILE_EXT, output_file, sizeof(output_file));
        assert(!SaveImage(output_file, image));
        DeleteImage(image);
    }
}

int main(int argc, char **argv)
{
    if (argc == 1) {
        fprintf(stderr, "Parallel Digital Image Processing\n");
        fprintf(stderr, "Usage: %s [FILE_NAME...]\n", argv[0]);
        return 1;
    }

    const int num_files = argc - 1;

    // Use the maximum number of available processors
    const int num_procs = omp_get_num_procs();
    const int thread_count = (NUM_THREADS > 0) ? NUM_THREADS : num_procs;
    omp_set_num_threads(thread_count);

    int T = thread_count;
    int team_size = 3;

    int full_teams = T / team_size;
    int remainder = T % team_size;

    int num_teams = full_teams + (remainder > 0 ? 1 : 0);

    if (LOGGING) {
        printf("Total threads: %d\n", T);
        printf("Full teams:    %d (3 threads each)\n", full_teams);
        if (remainder > 0)
            printf("Remainder team: %d threads (concurrent execution)\n", remainder);
        printf("Total teams:   %d\n", num_teams);
        printf("Images to process: %d\n", num_files);
        printf("Max parallel images: %d\n", full_teams + (remainder > 0 ? 1 : 0));
    }

    int next_image = 1;  // index in argv[]

    double start_time = omp_get_wtime();

    #pragma omp parallel num_threads(T)
    {
        int tid = omp_get_thread_num();
        int team_id = tid / team_size;  // which team
        int lane = tid % team_size;     // 0,1,2

        int team_threads = (team_id == full_teams && remainder > 0)
                               ? remainder       // final partial team
                               : team_size;      // full team
        int i = 0, done = 0;

        while (1) {
            /* Team leader grabs job */
            #pragma omp barrier
            #pragma omp single
            {
                if (next_image > num_files) {
                    done = 1;
                } else {
                    i = next_image++;
                }
            }
            #pragma omp barrier   // broadcast done and i

            if (done)
                break;

            const char *file_name = argv[i];

            // Split file path + base name
            char *tmp1 = strdup(file_name);
            char *base_name = basename(tmp1);
            char *tmp2 = strdup(file_name);
            char *file_path = dirname(tmp2);

            // Synchronize before starting sections
            #pragma omp barrier

            // Run the lane-specific section
            if (lane < team_threads) {
                run_section(lane, file_name, file_path, base_name);
            }

            // synchronize before grabbing next image
            #pragma omp barrier

            free(tmp1);
            free(tmp2);
        }
    }

    double end_time = omp_get_wtime();

    if (LOGGING) {
        printf("Elapsed time: %.5f s\n", end_time - start_time);
    }

    return 0;
}
