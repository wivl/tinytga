#ifndef _TINY_TGA_H
#define _TINY_TGA_H

#include <stdint.h>

enum image_type {
	NO_IMAGE_DATA = 0,
	COLOR_MAPPED_U = 1,
	TRUE_COLOR_U = 2,
	BLACK_AND_WHITE_U = 3,
	COLOR_MAPPED_R = 9,
	TRUE_COLOR_R = 10,
	BLACK_AND_WHITE_C = 11,
	// 32, 33
};

typedef struct {
	uint8_t b;
	uint8_t g;
	uint8_t r;
	uint8_t a;
} tt_color;

// image type
// width, height
// pixel depth
// pixels
typedef struct {
	uint8_t image_type;
	uint16_t width;
	uint16_t height;
	uint8_t pixel_depth;
	uint32_t *pixels;
} tt_image;

/* load tga image from file */
tt_image* tt_load_from_file(const char* file_path);

/* save a tt_image variable to file */
void tt_save(tt_image* image, const char* filename);

/* create a tt_image variable */
tt_image* tt_create(uint16_t w, uint16_t h, tt_color color);

/* destory a tt_image variable */
void tt_destroy(tt_image* image);

/* set dot */
void tt_set_color(tt_image* image, int w, int h, tt_color color);

/* get color from color (pixel) value */
tt_color tt_make_color(uint32_t color_value);

/* get color value from color */
uint32_t tt_get_color_value(tt_color color);

uint32_t tt_get_color_value_from(tt_image *image, int w, int h);

tt_color tt_get_color_from(tt_image *image, int w, int h);

void tt_color_intensity(tt_color *color, float intensity);

void tt_flip_vertically(tt_image *image);

#endif

