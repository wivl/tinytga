#include "tinytga.h"
#include <assert.h>

int main(void) {
	/* create a 32-bit-depth color value */
	tt_color color = tt_make_color(0xFF986BEB);

	/* create a width800, height600 image and use the color to fill the image */
	tt_image* image = tt_create(800, 600, color);
	
    assert(image != NULL);

    tt_save(image, "saved.tga");

    /* destory image */
	tt_destroy(image);
}
