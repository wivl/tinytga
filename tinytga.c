#include "tinytga.h"
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <stdbool.h>
#include <string.h>

#define HEADER_LENGTH 18

void dev_log(tt_image* image, uint8_t id_length, uint8_t color_map_type,
		uint8_t *color_map_specification, uint8_t *image_specification,
		uint8_t *image_id, tt_color *color_map) {
	if (image == NULL) {
		fprintf(stderr, "[DEV]Image pointer value is NULL\n");
		exit(1);
	}
	printf("[DEV]Header\n");
	printf("[DEV]Field 1:\t\t%u\n", id_length);
	printf("[DEV]Field 2:\t\t%u\n", color_map_type);
	printf("[DEV]Field 3:\t\t%u\n", image->image_type);
	printf("[DEV]Field 4:\t\t");
	for(int i = 0; i < 5; i++) { printf("%u ", color_map_specification[i]); }
	puts("");
	printf("[DEV]Field 5:\t\t");
	for(int i = 0; i < 10; i++) { printf("%u ", image_specification[i]); }
	puts("");
	printf("[DEV]Field 6:\t\t");
	for (int i = 0; i < id_length; i++) { printf("%u ", image_id[i]); }
	puts("");
	printf("[DEV]Field 7:\t\t\n");
	uint16_t color_map_length = color_map_specification[2]
		+ (color_map_specification[3] << 8);

	for (int i = 0; i < color_map_length; i++) {
		printf("\tColormap %d\t%u %u %u %u\n", i,
				color_map[i].b,
				color_map[i].g,
				color_map[i].a,
				color_map[i].r);
	}



	printf("[DEV]width:\t\t%u\n", image->width);
	printf("[DEV]height:\t\t%u\n", image->height);
	printf("[DEV]pixels:\t\t%u\n", image->pixels[0]);
	// for (uint32_t i = 0; i < image->height; i++) {
	// 	for (uint32_t j = 0; j < image->width; j++) {
	// 		printf("w%uh%u:\t\t%u\n", i, j, image->pixels[i*image->width+j]);
	//	}
	//	puts("");
	// }
}

// color 3
// image 8
void _colormap_to_true_color(tt_image* image, uint8_t* image_raw, tt_color* color_map, long* cp) {
	uint8_t pixel_depth = image_raw[16];
	uint8_t color_map_size = image_raw[7];
	switch (pixel_depth) {
		case 32:
			switch (color_map_size) {
				case 32:
					for (int i = 0; i < image->width*image->height; i++) {
						uint32_t color_index = 0;
						color_index += image_raw[(*cp)++];
						color_index += image_raw[(*cp)++] << 8;
						color_index += image_raw[(*cp)++] << 16;
						color_index += image_raw[(*cp)++] << 24;
						uint32_t color_value = (color_map[color_index-1].b)
							+ (color_map[color_index].g << 8)
							+ (color_map[color_index].r << 16)
							+ (color_map[color_index].a << 24); // bgra
						image->pixels[i] = color_value;
					}
					break;
				case 24:
					for (int i = 0; i < image->width*image->height; i++) {
						uint32_t color_index = 0;
						color_index += image_raw[(*cp)++];
						color_index += image_raw[(*cp)++] << 8;
						color_index += image_raw[(*cp)++] << 16;
						color_index += image_raw[(*cp)++] << 24;
						uint32_t color_value = (color_map[color_index-1].b)
							+ (color_map[color_index].g << 8)
							+ (color_map[color_index].r << 16)
							+ 0xFF000000; // bgra
						image->pixels[i] = color_value;
					}
					break;
				case 16: // 
					for (int i = 0; i < image->width*image->height; i++) {
						uint32_t color_index = 0;
						color_index += image_raw[(*cp)++];
						color_index += image_raw[(*cp)++] << 8;
						color_index += image_raw[(*cp)++] << 16;
						color_index += image_raw[(*cp)++] << 24;
						uint32_t color_value = (color_map[color_index-1].b)
							+ (color_map[color_index].g << 8)
							+ (color_map[color_index].r << 16)
							+ 0xFF000000; // bgra
						image->pixels[i] = color_value;
					}
					break;
				default:
					fprintf(stderr, "[ERR]Unsupport colormap entry size: %u", color_map_size);
					exit(1);
					break;
			}
			break;
		case 24: // pixel_depth == 24
			switch (color_map_size) {
				case 32:
					for (int i = 0; i < image->width*image->height; i++) {
						uint32_t color_index = 0;
						color_index += image_raw[(*cp)++];
						color_index += image_raw[(*cp)++] << 8;
						color_index += image_raw[(*cp)++] << 16;
						uint32_t color_value = (color_map[color_index-1].b)
							+ (color_map[color_index].g << 8)
							+ (color_map[color_index].r << 16)
							+ (color_map[color_index].a << 24); // bgra
						image->pixels[i] = color_value;
					}
					break;
				case 24:
					for (int i = 0; i < image->width*image->height; i++) {
						uint32_t color_index = 0;
						color_index += image_raw[(*cp)++];
						color_index += image_raw[(*cp)++] << 8;
						color_index += image_raw[(*cp)++] << 16;
						uint32_t color_value = (color_map[color_index-1].b)
							+ (color_map[color_index].g << 8)
							+ (color_map[color_index].r << 16)
							+ 0xFF000000; // bgra
						image->pixels[i] = color_value;
					}
					break;
				case 16: // 
					for (int i = 0; i < image->width*image->height; i++) {
						uint32_t color_index = 0;
						color_index += image_raw[(*cp)++];
						color_index += image_raw[(*cp)++] << 8;
						color_index += image_raw[(*cp)++] << 16;
						uint32_t color_value = (color_map[color_index-1].b)
							+ (color_map[color_index].g << 8)
							+ (color_map[color_index].r << 16)
							+ 0xFF000000; // bgra
						image->pixels[i] = color_value;
					}
					break;
				default:
					fprintf(stderr, "[ERR]Unsupport colormap entry size: %u", color_map_size);
					exit(1);
					break;
			}
			break;
		case 16:
			switch (color_map_size) {
				case 32:
					for (int i = 0; i < image->width*image->height; i++) {
						uint32_t color_index = 0;
						color_index += image_raw[(*cp)++];
						color_index += image_raw[(*cp)++] << 8;
						uint32_t color_value = (color_map[color_index-1].b)
							+ (color_map[color_index].g << 8)
							+ (color_map[color_index].r << 16)
							+ (color_map[color_index].a << 24); // bgra
						image->pixels[i] = color_value;
					}
					break;
				case 24:
					for (int i = 0; i < image->width*image->height; i++) {
						uint32_t color_index = 0;
						color_index += image_raw[(*cp)++];
						color_index += image_raw[(*cp)++] << 8;
						uint32_t color_value = (color_map[color_index-1].b)
							+ (color_map[color_index].g << 8)
							+ (color_map[color_index].r << 16)
							+ 0xFF000000; // bgra
						image->pixels[i] = color_value;
					}
					break;
				case 16: // 
					for (int i = 0; i < image->width*image->height; i++) {
						uint32_t color_index = 0;
						color_index += image_raw[(*cp)++];
						color_index += image_raw[(*cp)++] << 8;
						uint32_t color_value = (color_map[color_index-1].b)
							+ (color_map[color_index].g << 8)
							+ (color_map[color_index].r << 16)
							+ 0xFF000000; // bgra
						image->pixels[i] = color_value;
					}
					break;
				default:
					fprintf(stderr, "[ERR]Unsupport colormap entry size: %u", color_map_size);
					exit(1);
					break;
			}
			break;
		case 8:
			switch (color_map_size) {
				case 32:
					for (int i = 0; i < image->width*image->height; i++) {
						uint32_t color_index = 0;
						color_index += image_raw[(*cp)++];
						uint32_t color_value = (color_map[color_index-1].b)
							+ (color_map[color_index].g << 8)
							+ (color_map[color_index].r << 16)
							+ (color_map[color_index].a << 24); // bgra
						image->pixels[i] = color_value;
					}
					break;
				case 24:
					for (int i = 0; i < image->width*image->height; i++) {
						uint32_t color_index = 0;
						color_index += image_raw[(*cp)++];
						uint32_t color_value = (color_map[color_index].b)
							+ (color_map[color_index].g << 8)
							+ (color_map[color_index].r << 16)
							+ 0xFF000000; // bgra
						image->pixels[i] = color_value;
					}
					break;
				case 16: // 
					for (int i = 0; i < image->width*image->height; i++) {
						uint32_t color_index = 0;
						color_index += image_raw[(*cp)++];
						uint32_t color_value = (color_map[color_index-1].b)
							+ (color_map[color_index].g << 8)
							+ (color_map[color_index].r << 16)
							+ 0xFF000000; // bgra
						image->pixels[i] = color_value;
					}
					break;
				default:
					fprintf(stderr, "[ERR]Unsupport colormap entry size: %u", color_map_size);
					exit(1);
					break;
			}
			break;
		default:
			assert(false);
			break;
	}
}

void _load_color_map(tt_image* image, uint8_t* image_raw, tt_color* color_map, long* cp) {
	uint8_t color_map_entry_size = image_raw[7];
	uint16_t color_map_length = image_raw[5] +
			(image_raw[6] << 8);

	switch (color_map_entry_size) {
		case 16:
			// gggbbbbb arrrrrgg
			for (int i = 0; i < color_map_length; i++) {
				uint16_t color_value = 0;
				color_value += image_raw[(*cp)++];
				color_value += image_raw[(*cp)++] << 8;
				// arrrrrgg gggbbbbb
				color_map[i].a = (color_value >> 15) & 0x0001;
				color_map[i].r = (color_value >> 10) & 0x001F;
				color_map[i].g = (color_value >> 5) & 0x001F;
				color_map[i].b = color_value & 0x001F;
			}
			break;
		case 24:
			// bgr in order
			for (int i = 0; i < color_map_length; i++) {
				color_map[i].b = image_raw[(*cp)++];
				color_map[i].g = image_raw[(*cp)++];
				color_map[i].r = image_raw[(*cp)++];
			}
			break;
		case 32:
			// bgra in order
			for (int i = 0; i < color_map_length; i++) {
				color_map[i].b = image_raw[(*cp)++];
				color_map[i].g = image_raw[(*cp)++];
				color_map[i].r = image_raw[(*cp)++];
				color_map[i].a = image_raw[(*cp)++];
			}
			break;
		default: /* 15 and more */
			fprintf(stderr, "[ERR]Unsupport colormap entry size: %u\n", color_map_entry_size);
			exit(1);
			break;
	}
}

void _handle_pixels(tt_image* image, uint8_t* image_raw, tt_color* color_map, long* cp) {
	switch (image->image_type) {
		case NO_IMAGE_DATA:		/* No Image Data Included */
			image->pixels = NULL;
			break;
		case COLOR_MAPPED_U:		// Uncompressed, Color mapped image
			image->pixels = (uint32_t*)malloc(sizeof(uint32_t)*image->height*image->width);
			_colormap_to_true_color(image, image_raw, color_map, cp);
			break;
		case TRUE_COLOR_U:		/* Uncompressed, True Color Image */
			image->pixels = (uint32_t*)malloc(sizeof(uint32_t)*image->height*image->width);
			switch (image->pixel_depth) {
				case 32: // bbbbbbbbggggggggrrrrrrrraaaaaaaa
					for (int i = 0; i < image->width*image->height; i++) {
						uint32_t color_value = 0;
						color_value += image_raw[(*cp)++];
						color_value += image_raw[(*cp)++] << 8;
						color_value += image_raw[(*cp)++] << 16;
						color_value += image_raw[(*cp)++] << 24;
						image->pixels[i] = color_value;
					}
					break;
				case 24: // bbbbbbbbggggggggrrrrrrrr
					for (int i = 0; i < image->width*image->height; i++) {
						uint32_t color_value = 0;
						color_value += image_raw[(*cp)++];
						color_value += image_raw[(*cp)++] << 8;
						color_value += image_raw[(*cp)++] << 16;
						image->pixels[i] = color_value + 0xFF000000;
					}
					break;
				case 16: // arrrrrgg gggbbbbb -> gggbbbbb arrrrrgg
					// TODO: handle 16 bit pixel true color
					assert(false); // not support yet
	 				for (int i = 0; i < image->width*image->height; i++) {
						uint32_t color_value = 0;
						color_value += image_raw[(*cp)++] << 8;
						color_value += image_raw[(*cp)++];
						image->pixels[i] = color_value;
					}
					break;
				case 8:
					// TODO: handle 8 bit pixel true color
					assert(false);
					for (int i = 0; i < image->width*image->height; i++) {
						image->pixels[i] = image_raw[(*cp)++];
					}
					break;
				default:
					assert(false);
					break;
			}
			break;
        case TRUE_COLOR_R:
            // TODO: run length encoded true color, 10
            assert(false && "Run-length-encoded, but broken");
			image->pixels = (uint32_t*)malloc(sizeof(uint32_t)*image->height*image->width);
            int total_pixel = image->width * image->height;
            int pixel_count = 0;
            uint8_t header;
            uint8_t continuous_count;
            switch (image->pixel_depth) {
                case 32:
                    while (pixel_count < total_pixel) {
                        header = image_raw[(*cp)++];
                        if (header < 128) { // raw
                            continuous_count = (header & 0x7F) + 1;
                            for (int i = 0; i < continuous_count; i++) {
                                uint32_t color_value = 0;
                                color_value += image_raw[(*cp)++];
                                color_value += image_raw[(*cp)++] << 8;
                                color_value += image_raw[(*cp)++] << 16;
                                color_value += image_raw[(*cp)++] << 24;   

                                image->pixels[pixel_count++] = color_value;
                            }
                        } else { // run-length-encoded
                            uint32_t color_value = 0;
                            color_value += image_raw[(*cp)++];
                            color_value += image_raw[(*cp)++] << 8;
                            color_value += image_raw[(*cp)++] << 16;
                            color_value += image_raw[(*cp)++] << 24;

                            continuous_count = (header & 0x7F) + 1;
                            while (continuous_count--) {
                                image->pixels[pixel_count++] = color_value;
                            }
                        }

                    }
                    break;
                case 24:
                    while (pixel_count < total_pixel) {
                        header = image_raw[(*cp)++];
                        if (header < 128) { // raw
                            continuous_count = (header & 0x7F) + 1;
                            for (int i = 0; i < continuous_count; i++) {
                                uint32_t color_value = 0;
                                color_value += image_raw[(*cp)++];
                                color_value += image_raw[(*cp)++] << 8;
                                color_value += image_raw[(*cp)++] << 16;
                                color_value += 0xFF000000;   

                                image->pixels[pixel_count++] = color_value;
                            }
                        } else {
                            uint32_t color_value = 0;
                            color_value += image_raw[(*cp)++];
                            color_value += image_raw[(*cp)++] << 8;
                            color_value += image_raw[(*cp)++] << 16;
                            color_value += 0xFF000000;

                            continuous_count = header & 0x7F;
                            while (continuous_count--) {
                                image->pixels[pixel_count++] = color_value;
                            }
                        }
                        
                    }
                    break;
                default:
                    assert(false);
                    break;
            }

            break;
		case COLOR_MAPPED_R:		// Run-length encoded, Color mapped image
			// not support yet
			assert(false);
			break;
		case BLACK_AND_WHITE_C:	// Run-Length encoded, Black and white image
			// not support yet
			break;
		default:
			printf("[ERR]Unsupport image type: %u\n", image->image_type);
			exit(1);
			break;
	}
}

/*
 * Load TGA file and convert to "standerd" tga file, meaning 32 bit true color one
 * */
tt_image* tt_load_from_file(const char *file_path) {
	FILE *image_stream = fopen(file_path, "rb");
	if (image_stream == NULL) {
		// fprintf(stderr, "[ERR]File %s not found\n", file_path);
		fclose(image_stream);
		return NULL;
	}
	/* get image file length */
	fseek(image_stream, 0, SEEK_END);
	long image_size = ftell(image_stream);
	// printf("[DEBUG]Image size: %ld\n", image_size);
	fseek(image_stream, 0, SEEK_SET);
	/* read image file */
	uint8_t *image_raw = (uint8_t*)malloc(image_size);
	fread(image_raw, sizeof(uint8_t), image_size, image_stream);
	/* 18 bytes header */

	tt_image *image = (tt_image*)malloc(sizeof(tt_image));

	uint8_t id_length = image_raw[0];
	uint8_t color_map_type = image_raw[1];
	image->image_type = image_raw[2];
	uint8_t *color_map_specification = (uint8_t*)malloc(5); // the length of field 4 is 5 bytes
	for (int i = 0; i < 5; i++) { color_map_specification[i] = image_raw[i+3]; }
	uint8_t *image_specification = (uint8_t*)malloc(10);	 // the length of field 5 is 10 bytes
	for (int i = 0; i < 10; i++) { image_specification[i] = image_raw[i+8]; }
	uint8_t *image_id = NULL;



	long cp = HEADER_LENGTH; /* current_position */

	/* ID */
	if (id_length != 0) {
		image_id = (uint8_t*)malloc(id_length);
		for (int i = 0; i < id_length; i++) {
			image_id[i] = image_raw[cp+i];
		}
		cp += id_length;
	}

	/* color map */
	tt_color *color_map = NULL;
	if (color_map_type == 1) {
		uint16_t color_map_length = color_map_specification[2] +
			(color_map_specification[3] << 8);
		uint8_t color_map_entry_size = color_map_specification[4];

		color_map = (tt_color*)malloc(sizeof(tt_color)*color_map_length);
		
		_load_color_map(image, image_raw, color_map, &cp);

	}

	/* image */
	image->width = image_specification[4] +
		(image_specification[5] << 8);
	image->height = image_specification[6] +
		(image_specification[7] << 8);
	image->pixel_depth = image_specification[8];

	_handle_pixels(image, image_raw, color_map, &cp);


	// dev_log(image, id_length, color_map_type, color_map_specification, image_specification, image_id, color_map);
	if (image_raw != NULL) free(image_raw);
	if (image_specification != NULL) free(image_specification);
	if (color_map_specification != NULL) free(color_map_specification);
	if (color_map != NULL) free(color_map);
	if (image_id != NULL) free(image_id);
	fclose(image_stream);
	return image;
}


void tt_save(tt_image* image, const char* filename) {
	assert(image != NULL);
	FILE *file = fopen(filename, "wb");
	/* header */
	uint8_t header[] = "\0\0\2\0\0\0\0\0\0\0\0\0"; /* TRUE_COLOR_U header */
	uint8_t width[2], height[2], others[2];
	width[0] = image->width & 0x00FF;
	width[1] = (image->width >> 8) & 0x00FF;
	height[0] = image->height & 0x00FF;
	height[1] = (image->height >> 8) & 0x00FF;
	others[0] = 32;
	others[1] = 0;
	fwrite(header, sizeof(uint8_t), 12, file);
	fwrite(width, sizeof(uint8_t), 2, file);
	fwrite(height, sizeof(uint8_t), 2, file);
	fwrite(others, sizeof(uint8_t), 2, file);
	/* header end */
	
	/* pixels */
	fwrite(image->pixels, sizeof(uint32_t), image->width*image->height, file);

	fclose(file);
}

/* Create single-color tga file variable */
tt_image* tt_create(uint16_t w, uint16_t h, tt_color color) {
	tt_image *image = (tt_image*)malloc(sizeof(tt_image));
	image->width = w, image->height = h;
	image->image_type = TRUE_COLOR_U;
	image->pixel_depth = 32;
	if (w*h == 0) {
		image->pixels = NULL;
	} else {
		image->pixels = (uint32_t*)malloc(sizeof(uint32_t)*w*h);
		uint32_t pixel_value = 0;// argb saved as bgra
		pixel_value = tt_get_color_value(color);
		for (int i = 0; i < w*h; i++) {
			image->pixels[i] = pixel_value;
		}
	}
	
	return image;
}

/* Destroy tt_image variable */
void tt_destroy(tt_image *image) {
	if (image == NULL) {
		return ;
	}
	if (image->pixels != NULL) {
		free(image->pixels);
	}
	free(image);
}

/* set a dot to certain color */
void tt_set_color(tt_image* image, int w, int h, tt_color color) {
	assert(w < image->width && h < image->height);
	uint32_t color_value = tt_get_color_value(color);

	uint16_t x = w;
	uint16_t y = image->height- 1 - h;

	image->pixels[y*image->width+x] = color_value;
}

/* get color from color value */
tt_color tt_make_color(uint32_t color_value) {
	tt_color color;
	color.b = color_value & 0xFF; color_value >>= 8;
	color.g = color_value & 0xFF; color_value >>= 8;
	color.r = color_value & 0xFF; color_value >>= 8;
	color.a = color_value & 0xFF;
	
	return color;
}

/* get color value from color */
uint32_t tt_get_color_value(tt_color color) {
	uint32_t color_value = 0;
	color_value += color.a << 24;
	color_value += color.r << 16;
	color_value += color.g << 8;
	color_value += color.b;
	return color_value;
}


void tt_flip_vertically(tt_image *image) {
	uint32_t *pixels = (uint32_t*)malloc(sizeof(uint32_t)*image->width*image->height);
	memcpy(pixels, image->pixels, sizeof(uint32_t)*image->width*image->height);
	// flip
	for (int i = 0; i < image->height; i++) {
		for (int j = 0; j < image->width; j++) {
			int y = image->height - 1 - i;
			int x = j;
			image->pixels[i*image->width+j] = pixels[y*image->width+x];
		}
	}
	free(pixels);

}
