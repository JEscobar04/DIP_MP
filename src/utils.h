#ifndef UTILS_H
#define UTILS_H

#include <stddef.h>
#include <stdio.h>
#include <string.h>
#include <omp.h>

void build_file_name(const char *dir,
                     const char *base,
                     const char *suffix,
                     const char *ext,
                     char *out,
                     int out_size);

void process_file(int file_id);

void compute_thread_split(int M, int N, int *threads_per_file);

#endif