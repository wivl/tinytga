#ifndef _TINY_TGA_H
#define _TINY_TGA_H

#include <stdint.h>


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


typedef struct {
	tt_header header;
	uint8_t *image_id;
	uint16_t width;
	uint16_t height;
	tt_color *color_map;
	uint32_t *pixels;
} tt_image;

/* load tga image from file */
tt_image* tt_load_from_file(const char* file_path);



#endif

