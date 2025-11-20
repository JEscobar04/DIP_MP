#include "Constants.h"
#include "Image.h"

/* Allocate dynamic memory for the image structure and its R/G/B values */
/* Return the pointer to the image, or NULL in case of error */
IMAGE *CreateImage(unsigned int Width, unsigned int Height)
{
	IMAGE* new_image = malloc(sizeof(IMAGE));
	new_image->H = Height;
	new_image->W = Width;
	
	new_image->R = malloc(sizeof(unsigned char) * Width * Height);
	new_image->G = malloc(sizeof(unsigned char) * Width * Height);
	new_image->B = malloc(sizeof(unsigned char) * Width * Height);
	return new_image;
}

/* Release the memory spaces for the pixel color intensity values */
/* Release the memory spaces for the image                        */
/* Set R/G/B pointers to NULL                                     */
void DeleteImage(IMAGE *image) {
    assert(image);
    assert(image->R);
    assert(image->G);
    assert(image->B);
    free(image->R);
    free(image->G);
    free(image->B);
    image->R = NULL;
    image->G = NULL;
    image->B = NULL;
    free(image);
}


/* Get the R intensity of pixel (x, y) in image */
unsigned char GetPixelR(const IMAGE *image, unsigned int x,  unsigned int y)
{
	assert(image);
	
	/* Gets the array */
	unsigned char* r = image->R;
	return (unsigned char) r[x + y * ImageWidth(image)];
}

/* Get the G intensity of pixel (x, y) in image */
unsigned char GetPixelG(const IMAGE *image, unsigned int x,  unsigned int y)
{
	assert(image);
	
	/* Gets the array */
	unsigned char* g = image->G;
	return (unsigned char) g[x + y * ImageWidth(image)];
}

/* Get the B intensity of pixel (x, y) in image */
unsigned char GetPixelB(const IMAGE *image, unsigned int x,  unsigned int y)
{
	assert(image);
	
	/* Gets the array */
	unsigned char* b = image->B;
	return (unsigned char) b[x + y * ImageWidth(image)];
}

/* Set the R intensity of pixel (x, y) in image to r */
void SetPixelR(IMAGE *image, unsigned int x,  unsigned int y, unsigned char r)
{
	assert(image);

	/* Gets the value by indexing the array */
	image->R[x + y * ImageWidth(image)] = r;
}

/* Set the G intensity of pixel (x, y) in image to g */
void SetPixelG(IMAGE *image, unsigned int x,  unsigned int y, unsigned char g)
{
	assert(image);

	/* Gets the value by indexing the array */
	image->G[x + y * ImageWidth(image)] = g;}

/* Set the B intensity of pixel (x, y) in image to b */
void SetPixelB(IMAGE *image, unsigned int x,  unsigned int y, unsigned char b)
{
	assert(image);

	/* Gets the value by indexing the array */
	image->B[x + y * ImageWidth(image)] = b;}

/* Return the image's width in pixels */
unsigned int ImageWidth(const IMAGE *image)
{
	assert(image);

	return image->W;
}

/* Return the image's height in pixels */
unsigned int ImageHeight(const IMAGE *image)
{
	assert(image);

	return image->H;
}