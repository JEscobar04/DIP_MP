#include "utils.h"

#include <stddef.h>
#include <stdio.h>
#include <string.h>
#include <omp.h>
#include <assert.h>

#include "Image.h"
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

void func(int file, int id) {
    printf("File %d, Func %d executed by thread %d\n",
           file, id, omp_get_thread_num());
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
    int base = M / N;        // minimum threads per file
    int extra = M % N;       // number of files that get +1 thread

    for (int i = 0; i < N; i++) {
        threads_per_file[i] = base + (i < extra ? 1 : 0);
    }

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
