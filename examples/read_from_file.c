#include "tinytga.h"
#include <stdio.h>
#include <stdlib.h>

int main(void) {
	/* load from file */
	tt_image* image = tt_load_from_file("demo.tga");
	/* if no file found, the function returns NULL */
	if (image == NULL) {
		fprintf(stderr, "[ERR]File not found.\n");
		exit(1);
	}
	if (image->pixels == NULL) {
		fprintf(stderr, "[ERR]The file contains no image infomation.\n");
		exit(1);
	}
	/* print the first pixel value */
	printf("%u", image->pixels[0]); /* output 32 bit true color value in the form of #AARRGGBB */

	/* destroy image variable */
	tt_destroy(image);
	return 0;
}
