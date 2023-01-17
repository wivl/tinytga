#ifndef _TINY_TGA_H
#define _TINY_TGA_H

#include <stdint.h>


struct tt_header {
	uint8_t 
}

struct tt_color {
	uint8_t a;
	uint8_t b;
	uint8_t g;
	uint8_t r;
};


struct tt_image {
	tt_color *color_map;
};




#endif

