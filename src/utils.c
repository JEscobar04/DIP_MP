#include "utils.h"

#include <stddef.h>
#include <stdio.h>
#include <string.h>

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
