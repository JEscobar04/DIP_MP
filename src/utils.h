#ifndef UTILS_H
#define UTILS_H

#include <stddef.h>

void build_file_name(const char *dir,
                     const char *base,
                     const char *suffix,
                     const char *ext,
                     char *out,
                     int out_size);

#endif
