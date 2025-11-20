#ifndef FILEIO_H
#define FILEIO_H

#include "Image.h"

/* Read image from a file 						*/
/* The size of the image needs to be pre-set				*/
/* The memory spaces of the image will be allocated in this function	*/
/* Return values:							*/
/* NULL: fail to load or create an image				*/
/* image: load or create an image successfully				*/
IMAGE *LoadImage(const char *fname);

/* Save a processed image 		*/
/* Return values:			*/
/* 0: successfully saved the image	*/
/* 1: Cannot open the file for writing	*/
/* 2: File error while writing to file	*/
int SaveImage(const char *fname, const IMAGE *image);

/* Copy a source file to a destination file using standard library */
/* Parameters source and destination file name                     */
/* Return 0 on success, or a value greater than 0 in case of error */
int Copy(const char *fname_src, const char *fname_dst);

/* [Bonus]                                                         */
/* Copy a source file to a destination file using system calls     */
/* Parameters source and destination file name                     */
/* Return 0 on success, or a value greater than 0 in case of error */
int Copy_syscall(const char *fname_src, const char *fname_dst);

#endif