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
	uint8_t id_length;				  // Length of the image ID field (0-255)
	uint8_t color_map_type;			  // Whether a color map is included
	uint8_t image_type;				  // Compression and color types
	uint8_t *color_map_specification; // 5 bytes, describes the color map
	uint8_t *image_specification;	  // 10 bytes, image dimensions and format
} tt_header;

typedef struct {
	uint8_t b;
	uint8_t g;
	uint8_t r;
	uint8_t a;
} tt_color;

// TODO: rewrite the tt_image struct
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
void tt_save(tt_image* image, const char* filename);


#endif

