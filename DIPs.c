#include "DIPs.h"
#include "Constants.h"

/* Age the image */
IMAGE *Aging(IMAGE *image)
{
	assert(image);
	int x, y, tmp;

	for (y = 0; y < ImageHeight(image); y++) {
			for (x = 0; x < ImageWidth(image); x++) {
				tmp = (GetPixelR(image, x, y) +  GetPixelG(image, x, y) + GetPixelB(image, x, y))/5;
				SetPixelB(image, x, y, (unsigned char)tmp);
				SetPixelR(image, x, y, (unsigned char)(tmp*1.6));
				SetPixelG(image, x, y, (unsigned char)(tmp*1.6));
			}
	}
	return image;
}

/* Black and White */
IMAGE *BlackNWhite(IMAGE *image)
{
	assert(image);
	int x, y, average_intensity;

	for (y = 0; y < ImageHeight(image); y++) {
		for (x = 0; x < ImageWidth(image); x++) {
			average_intensity = (GetPixelR(image, x, y) +  GetPixelG(image, x, y) + GetPixelB(image, x, y)) / 3;
			SetPixelR(image, x, y, (unsigned char) average_intensity);
			SetPixelG(image, x, y, (unsigned char) average_intensity);
			SetPixelB(image, x, y, (unsigned char) average_intensity);
		}
	}
	return image;
}

/* Reverse image color */
IMAGE *Negative(IMAGE *image)
{
	assert(image);
	int x, y;

	for (y = 0; y < ImageHeight(image); y++) {
		for (x = 0; x < ImageWidth(image); x++) {
			SetPixelR(image, x, y, (unsigned char) (255 - GetPixelR(image, x, y)));
			SetPixelG(image, x, y, (unsigned char) (255 - GetPixelG(image, x, y)));
			SetPixelB(image, x, y, (unsigned char) (255 - GetPixelB(image, x, y)));
		}
	}
	return image;
}

/* flip image vertically */
IMAGE *VFlip(IMAGE *image)
{
	assert(image);
	int x, y, temp;

	for (y = 0; y < ImageHeight(image) / 2; y++) {
		for (x = 0; x < ImageWidth(image); x++) {
			temp = GetPixelR(image, x, y);
			SetPixelR(image, x, y, GetPixelR(image, x, (ImageHeight(image) - y - 1)));
			SetPixelR(image, x, (ImageHeight(image) - y - 1), temp);

			temp = GetPixelG(image, x, y);
			SetPixelG(image, x, y, GetPixelG(image, x, (ImageHeight(image) - y - 1)));
			SetPixelG(image, x, (ImageHeight(image) - y - 1), temp);

			temp = GetPixelB(image, x, y);
			SetPixelB(image, x, y, GetPixelB(image, x, (ImageHeight(image) - y - 1)));
			SetPixelB(image, x, (ImageHeight(image) - y - 1), temp);
		}
	}
	return image;
}

/* mirror image horizontally */
IMAGE *HMirror(IMAGE *image)
{
	assert(image);
	int x, y;

	for (y = 0; y < ImageHeight(image); y++) {
		for (x = 0; x < ImageWidth(image) / 2; x++) {
			SetPixelR(image, (ImageWidth(image) - x - 1), y, GetPixelR(image, x, y));
			SetPixelG(image, (ImageWidth(image) - x - 1), y, GetPixelG(image, x, y));
			SetPixelB(image, (ImageWidth(image) - x - 1), y, GetPixelB(image, x, y));
		}
	}
	return image;
}

/* exchange G and B color channels */
IMAGE *ExGB(IMAGE *image)
{
	assert(image);
	int x, y, temp;

	for (y = 0; y < ImageHeight(image); y++) {
		for (x = 0; x < ImageWidth(image); x++) {
			temp = GetPixelG(image, x, y);
			SetPixelG(image, x, y, GetPixelB(image, x, y));
			SetPixelB(image, x, y, temp);
		}
	}
	return image;
}

/* [Bonus] sharpen the image */
IMAGE *Sharpen(IMAGE *image)
{
	assert(image);
	int x, y;
	IMAGE* output_image = CreateImage(ImageWidth(image), ImageHeight(image));

	for (y = 0; y < ImageHeight(output_image); y++) {
		for (x = 0; x < ImageWidth(output_image); x++) {
			SetPixelR(output_image, x, y, GetPixelR(image, x, y));
			SetPixelG(output_image, x, y, GetPixelG(image, x, y));
			SetPixelB(output_image, x, y, GetPixelB(image, x, y));
		}
	}
	
	for (y = 1; y < ImageHeight(output_image) - 1; y++) {
		for (x = 1; x < ImageWidth(output_image) - 1; x++) {
			SetPixelR(output_image, x, y, (unsigned char) saturate(sharpen_helper(image->R, ImageWidth(image), x, y)));
			SetPixelG(output_image, x, y, (unsigned char) saturate(sharpen_helper(image->G, ImageWidth(image), x, y)));
			SetPixelB(output_image, x, y, (unsigned char) saturate(sharpen_helper(image->B, ImageWidth(image), x, y)));
		}
	}

	DeleteImage(image);
	image = NULL;
	return output_image;
}

/* Decided to create this function so that it passes the specified array to allow for a modular approach */
int sharpen_helper(unsigned char* V, unsigned int width, int x, int y)
{
	int s_V = 0;
	s_V = 9 * V[x + y * width] - V[(x - 1) + (y * width)] - V[(x - 1) + ((y - 1) * width)] - V[(x) + ((y - 1) * width)] - V[(x + 1) + ((y) * width)] - V[(x) + ((y + 1) * width)] - V[(x + 1) + ((y + 1) * width)] - V[(x - 1) + ((y + 1) * width)] - V[(x + 1) + ((y - 1) * width)];

	return s_V;
}

/* Same helper from previous assignment*/
int saturate(int pixel_value)
{
	if (pixel_value > 255)
	{
		return 255;
	}
	else if (pixel_value < 0)
	{
		return 0;
	}
	else
	{
		return pixel_value;
	}
}
