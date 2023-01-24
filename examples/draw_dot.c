#include "tinytga.h"
#include <stdio.h>
#include <stdlib.h>

int main(void) {
	/* create 32-bit-depth color value */
	tt_color pink = {.a = 0xFF, .r = 0x98, .g = 0x6B, .b = 0xEB};
	tt_color black = {.a = 0xFF, .r = 0, .g = 0, .b = 0};
	/* create a width800, height600 image and use the color to fill the image */
	tt_image* image = tt_create(800, 600, pink);
	// tt_image *image = tt_load_from_file("earth.tga");

	/* set a dot in black, w100, h200 */
	tt_set_color(image, 100, 200, black);

	
	if (image != NULL) { /* otherwise it will fail a assert in the tt_save function */
		tt_save(image, "saved.tga");
	}
	tt_destroy(image);
}

