#include <stdio.h>
#include <omp.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <assert.h>
#include <libgen.h>

#include "Image.h"
#include "DIPs.h"
#include "FileIO.h"
#include "utils.h"
#include "Constants.h"

const char *FILE_EXT = ".ppm";

int main(int argc, char** argv) {
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
	int id;
	int num_files = argc - 1;

	int threads_per_file[num_files];
	compute_thread_split(num_procs, num_files, threads_per_file);
	/* Optional */
    // printf("Thread distribution:\n");
    // for (int i = 0; i < num_files; i++) {
    //     printf("File %d → %d threads\n", i, threads_per_file[i]);
	// }

	for (int i = 1; i < argc; i++) {
		const char *file_name = argv[i];
		char *fname_copy_base = strdup(file_name);
		char *base_name = basename(fname_copy_base);
		char *fname_copy_path = strdup(file_name);
		char *file_path = dirname(fname_copy_path);
		char output_file[512] = {0};

		#pragma omp parallel num_threads(threads_per_file[i])
		{
			id = omp_get_thread_num();
			process_file(i);
		}
	}
}