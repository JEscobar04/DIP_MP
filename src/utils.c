#include "utils.h"

#include <assert.h>
#include <omp.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

#include "DIPs.h"
#include "FileIO.h"

void build_file_name(const char *dir,
                     const char *base,
                     const char *suffix,
                     const char *ext,
                     char *out,
                     int out_size) {
    const char *dot = strrchr(base, '.');
    int len = dot ? (int)(dot - base) : strlen(base);
    snprintf(out, out_size, "%s/%.*s%s%s", dir, (int)len, base, suffix, ext);
}

int check_input_files(int argc, char **argv) {
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
    return 0;
}

void func(int file, int id) {
    printf("File %d, Func %d executed by thread %d\n", file, id, omp_get_thread_num());
}

void process_file(int file_id) {
#pragma omp single
    {
#pragma omp task
        {
            LoadImage(NULL);
            Sharpen(NULL);
        }
#pragma omp taskwait
    }
}

// Given total_threads = M and num_files = N,
// compute threads_per_file[i] for each file i.
void compute_thread_split(int M, int N, int *threads_per_file) {
    int base = M / N;  // minimum threads per file
    int extra = M % N; // number of files that get +1 thread

    for (int i = 0; i < N; i++) {
        threads_per_file[i] = base + (i < extra ? 1 : 0);
    }
}
