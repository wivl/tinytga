#include "tinytga.h"
#include <stdio.h>
#include <stdlib.h>

int main(void) {
	/* create a 32-bit-depth color value */
	tt_color color = {.a = 0xFF, .r = 0x98, .g = 0x6B, .b = 0xEB};
	/* create a width800, height600 image and use the color to fill the image */
	tt_image* image = tt_create(800, 600, color);
	
	if (image != NULL) { /* otherwise it will fail a assert in the tt_save function */
		tt_save(image, "saved.tga");
	}
	tt_destroy(image);
}
