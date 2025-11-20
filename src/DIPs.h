#ifndef DIPS_H
#define DIPS_H

#include "Image.h"

/* Age the image */
IMAGE *Aging(IMAGE *image);

/* Black and White */
IMAGE *BlackNWhite(IMAGE *image);

/* Reverse image color */
IMAGE *Negative(IMAGE *image);

/* flip image vertically */
IMAGE *VFlip(IMAGE *image);

/* mirror image horizontally */
IMAGE *HMirror(IMAGE *image);

/* exchange R and B color channels */
IMAGE *ExGB(IMAGE *image);

/* [Bonus] sharpen the image */
IMAGE *Sharpen(IMAGE *image);

/* Keep the value within 255 */
int saturate(int pixel_value);

/* Helper to pass any array */
int sharpen_helper(unsigned char* V, unsigned int width, int x, int y);

#endif