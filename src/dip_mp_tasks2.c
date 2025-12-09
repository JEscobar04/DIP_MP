#include <assert.h>
#include <omp.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "DIPs.h"
#include "FileIO.h"
#include "Image.h"
#include "utils.h"

#define TEAM_SIZE 3    // BW, Sharpen, Flip
#define NUM_THREADS 128  // 0 = max threads, >0 = user-defined
#define LOGGING 1

/* Thread-safe path splitter */
static void split_path(const char *filepath,
                       char *dir, size_t dsz,
                       char *base, size_t bsz)
{
    const char *slash = strrchr(filepath, '/');
    if (!slash) {
        snprintf(dir, dsz, ".");
        snprintf(base, bsz, "%s", filepath);
    } else {
        size_t plen = slash - filepath;
        if (plen >= dsz) plen = dsz - 1;
        memcpy(dir, filepath, plen);
        dir[plen] = '\0';
        snprintf(base, bsz, "%s", slash + 1);
    }
}

static void run_section(int lane,
                        const char *file_name,
                        const char *file_path,
                        const char *base_name)
{
    char out[512];
    IMAGE *image = LoadImage(file_name);
    assert(image);

    if (lane == 0) {
        image = BlackNWhite(image);
        build_file_name(file_path, base_name, "_bw", FILE_EXT, out, sizeof(out));
    }
    else if (lane == 1) {
        image = Sharpen(image);
        build_file_name(file_path, base_name, "_sharpen", FILE_EXT, out, sizeof(out));
    }
    else { // lane == 2
        image = VFlip(image);
        build_file_name(file_path, base_name, "_vflip", FILE_EXT, out, sizeof(out));
    }

    assert(!SaveImage(out, image));
    DeleteImage(image);
}

int main(int argc, char **argv)
{
    if (argc < 2) {
        fprintf(stderr, "Usage: %s [files...]\n", argv[0]);
        return 1;
    }

    const int num_files = argc - 1;

    /* NUM_THREADS handling */
    const int max_procs = omp_get_num_procs();
    const int T = (NUM_THREADS > 0) ? NUM_THREADS : max_procs;
    omp_set_num_threads(T);

    const int full_teams = T / TEAM_SIZE;
    const int remainder  = T % TEAM_SIZE;
    const int num_teams  = full_teams + (remainder ? 1 : 0);

    if (LOGGING) {
        printf("Total threads: %d\n", T);
        printf("Full 3-thread teams: %d\n", full_teams);
        if (remainder)
            printf("Remainder team: %d threads (concurrent)\n", remainder);
        printf("Total teams: %d\n", num_teams);
        printf("Images: %d\n", num_files);
    }

    double start = omp_get_wtime();

    int next_image = 1;

    #pragma omp parallel shared(next_image)
    {
        const int tid = omp_get_thread_num();
        const int team_id = tid / TEAM_SIZE;
        const int lane = tid % TEAM_SIZE;

        /* team thread count (3 or remainder) */
        const int team_threads =
            (team_id == full_teams && remainder > 0)
                ? remainder
                : TEAM_SIZE;

        int img_idx = 0;
        int done = 0;

        while (1) {

            /* 1) Team leader grabs the next image index */
            #pragma omp barrier
            #pragma omp single
            {
                if (next_image > num_files)
                    done = 1;
                else
                    img_idx = next_image++;
            }
            #pragma omp barrier

            if (done) break;

            const char *file_name = argv[img_idx];

            /* Thread-safe path split */
            char dir[512], base[512];
            split_path(file_name, dir, sizeof(dir), base, sizeof(base));

            /* 2) Lane performs the assigned DIP operation */
            if (lane < team_threads) {
                run_section(lane, file_name, dir, base);
            }

            /* 3) Wait for the team before grabbing next image */
            #pragma omp barrier
        }
    }

    double end = omp_get_wtime();
    if (LOGGING) {
        printf("Elapsed: %.4f s\n", end - start);
    }

    return 0;
}
