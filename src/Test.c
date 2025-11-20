#include "Test.h"
#include "FileIO.h"
#include "Constants.h"
#include "Image.h"
#include "DIPs.h"

/* auto test */
int AutoTest(void) {
    int result;
    const char fname[SLEN] = "argyros";

	IMAGE *image = NULL;
    image = LoadImage(fname);
    if (! image) return 11;
    image = Aging(image);
    if (! image) return 12;
    result = SaveImage("aging", image);
    if (result) return result;
    printf("Aging tested!\n\n");
	DeleteImage(image);
	image = NULL;

    image = LoadImage(fname);
    if (! image) return 13;
    image = BlackNWhite(image);
    if (! image) return 14;
    result = SaveImage("bw", image);
    if (result) return result;
    printf("Black and White tested!\n\n");
	DeleteImage(image);
	image = NULL;

    image = LoadImage(fname);
    if (! image) return 15;
    image = Negative(image);
    if (! image) return 16;
    result = SaveImage("negative", image);
    if (result) return result;
    printf("Negative tested!\n\n");
    DeleteImage(image);
	image = NULL;

    image = LoadImage(fname);
    if (! image) return 17;
    image = VFlip(image);
    if (! image) return 18;
    result = SaveImage("vflip", image);
    if (result) return result;
    printf("VFlip tested!\n\n");
    DeleteImage(image);
	image = NULL;

    image = LoadImage(fname);
    if (! image) return 19;
    image = HMirror(image);
    if (! image) return 20;
    result = SaveImage("hmirror", image);
    if (result) return result;
    printf("HMirror tested!\n\n");
    DeleteImage(image);
	image = NULL;

    image = LoadImage(fname);
    if (! image) return 21;
    image = Sharpen(image);
    if (! image) return 22;
    result = SaveImage("sharpen", image);
    if (result) return result;
    printf("Sharpen tested!\n\n");
    DeleteImage(image);
	image = NULL;

    image = LoadImage(fname);
    if (! image) return 23;
    image = ExGB(image);
    if (! image) return 24;
    result = SaveImage("xRG", image);
    if (result) return result;
    printf("ExRG tested!\n\n");
    DeleteImage(image);
	image = NULL;

    
	result = Copy("argyros.ppm", "copy_argyros.ppm");
	if (result) return result;
	image = LoadImage("copy_argyros");
	if (! image) return 25;
	result = SaveImage("copy_argyros", image);
	if (result) return result;
	printf("Copy with library tested!\n\n");
	DeleteImage(image);
    image = NULL;

	result = Copy_syscall("aging.ppm", "copy_aging.ppm");
	if (result) return result;
	image = LoadImage("copy_aging");
	if (! image) return 26;
	result = SaveImage("copy_aging", image);
	if (result) return result;
	printf("Copy with system call tested!\n\n");
	DeleteImage(image);
    image = NULL;
    

    return 0; /* success! */
}