#include "tinytga.h"
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

int main(void) {
	/* create 32-bit-depth color value */
	tt_color pink = tt_make_color(0xFF986BEB);
	tt_color black = tt_make_color(0xFF000000);
	/* create a width800, height600 image and use the color to fill the image */
	tt_image* image = tt_create(800, 600, pink);

    assert(image != NULL);

	/* set a dot in black, w100, h200, w and h starts from 0 */
	tt_set_color(image, 100, 200, black);

    tt_save(image, "saved.tga");

	tt_destroy(image);
}

