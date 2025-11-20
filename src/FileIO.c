#include "Constants.h"
#include "FileIO.h"

#include <string.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/types.h>

#define BUFFER_SIZE 80

IMAGE *LoadImage(const char *fname)
{
	FILE         *File;
	char         Type[SLEN];
	int          W, H, MaxValue;
	unsigned int x, y;
	char         ftype[] = ".ppm";
	char         fname_tmp[SLEN];
	IMAGE        *image;
	strcpy(fname_tmp, fname);
	strcat(fname_tmp, ftype);
	File = fopen(fname_tmp, "r");
	if (!File) {
		printf("\nCan't open file \"%s\" for reading!\n", fname);
		return NULL;
	}

	fscanf(File, "%79s", Type);
	if (Type[0] != 'P' || Type[1] != '6' || Type[2] != 0) {
		printf("\nUnsupported file format!\n");
		fclose(File);
		return NULL;
	}

	fscanf(File, "%d", &W);
	if (W <= 0) {
		printf("\nUnsupported image width %d!\n", W);
		fclose(File);
		return NULL;
	}

	fscanf(File, "%d", &H);
	if (H <= 0) {
		printf("\nUnsupported image height %d!\n", H);
		fclose(File);
		return NULL;
	}

	fscanf(File, "%d", &MaxValue);
	if (MaxValue != 255) {
		printf("\nUnsupported image maximum value %d!\n", MaxValue);
		fclose(File);
		return NULL;
	}
	if ('\n' != fgetc(File)) {
		printf("\nCarriage return expected at the end of the file!\n");
		fclose(File);
		return NULL;
	}

	image = CreateImage(W, H);

	if (!image) {
		printf("\nError creating image from %s!\n", fname_tmp);
		fclose(File);
		return NULL;
	}
	else {
		for (y = 0; y < ImageHeight(image); y++)
			for (x = 0; x < ImageWidth(image); x++) {
				SetPixelR(image, x, y, fgetc(File));
				SetPixelG(image, x, y, fgetc(File));
				SetPixelB(image, x, y, fgetc(File));
			}

		if (ferror(File)) {
			printf("\nFile error while reading from file!\n");
			DeleteImage(image);
			return NULL;
		}

		printf("%s was read successfully!\n", fname_tmp);
		fclose(File);
		return image;
	}
}

int SaveImage(const char *fname, const IMAGE *image)
{
    assert(image != NULL && "No image to save!\n");
	FILE         *File;
	int          x, y;
	char         SysCmd[SLEN * 5];
	char         ftype[] = ".ppm";
	char         fname_tmp[SLEN];
	char         fname_tmp2[SLEN];
	unsigned int WIDTH = ImageWidth(image);
	unsigned int HEIGHT = ImageHeight(image);
	strcpy(fname_tmp, fname);
	strcpy(fname_tmp2, fname);
	strcat(fname_tmp2, ftype);

	File = fopen(fname_tmp2, "w");
	if (!File) {
		printf("\nCan't open file \"%s\" for writing!\n", fname);
		return 1;
	}
	fprintf(File, "P6\n");
	fprintf(File, "%d %d\n", WIDTH, HEIGHT);
	fprintf(File, "255\n");

	for (y = 0; y < HEIGHT; y++)
		for (x = 0; x < WIDTH; x++) {
			fputc(GetPixelR(image, x, y), File);
			fputc(GetPixelG(image, x, y), File);
			fputc(GetPixelB(image, x, y), File);
		}

	if (ferror(File)) {
		printf("\nError while writing to file!\n");
		return 2;
	}
	fclose(File);
	printf("%s was saved successfully. \n", fname_tmp2);
	/*
	 * Rename file to image.ppm, convert it to ~/public_html/ceng231/<fname>.jpg
	 * and make it world readable
	 */
	sprintf(SysCmd, "~ceng231/bin/pnmtojpeg.sh %s",
			fname_tmp2);
	if (system(SysCmd) != 0) {
		printf("\nError while converting to JPG:\nCommand \"%s\" failed!\n", SysCmd);
		return 3;
	}
	printf("%s.jpg was stored for viewing. \n", fname_tmp);
	return 0;
}

int Copy(const char* fname_src, const char* fname_dst) 
{	
	char read_buffer[BUFFER_SIZE];
	
	FILE* source_file = fopen(fname_src, "r");
	if (!source_file) 
	{
		printf("Error reading from file\n");
		return -1;
	}
	
	FILE* destination_file = fopen(fname_dst, "w+");
	if (!destination_file)
	{
		printf("Error creating file\n");
		return -1;
	}
	
	while (fread(read_buffer, sizeof(char), BUFFER_SIZE, source_file) > 0) 
	{
		fwrite(read_buffer, sizeof(char), BUFFER_SIZE, destination_file);
	}
	fclose(source_file);
	fclose(destination_file);
	
	return 0;
}

int Copy_syscall(const char *fname_src, const char *fname_dst)
{
	int source_fd, destination_fd;
	char read_buffer[BUFFER_SIZE];

	source_fd = open(fname_src, O_RDONLY);
	if (source_fd == -1) 
	{
		printf("Error getting file descriptor for %s\n", fname_src);
		return -1;
	}
	destination_fd = open(fname_dst, O_CREAT | O_WRONLY, S_IRWXU | S_IRGRP | S_IROTH);
	if (destination_fd == -1) 
	{
		printf("Error getting file descriptor for %s\n", fname_dst);
		close(source_fd);
		return -1;
	}

	while (read(source_fd, read_buffer, BUFFER_SIZE) > 0) 
	{
		write(destination_fd, read_buffer, BUFFER_SIZE);
	}
	
	close(source_fd);
	fsync(destination_fd);
	close(destination_fd);


	return 0;
}