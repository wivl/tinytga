#include "tinytga.h"
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <stdbool.h>

#define HEADER_LENGTH 18

void dev_log(tt_image* image) {
	if (image == NULL) {
		fprintf(stderr, "[DEV]Image pointer value is NULL\n");
		exit(1);
	}
	printf("[DEV]Header\n");
	printf("[DEV]Field 1:\t\t%u\n", image->header.id_length);
	printf("[DEV]Field 2:\t\t%u\n", image->header.color_map_type);
	printf("[DEV]Field 3:\t\t%u\n", image->header.image_type);
	printf("[DEV]Field 4:\t\t");
	for(int i = 0; i < 5; i++) { printf("%u ", image->header.color_map_specification[i]); }
	puts("");
	printf("[DEV]Field 5:\t\t");
	for(int i = 0; i < 10; i++) { printf("%u ", image->header.image_specification[i]); }
	puts("");
	printf("[DEV]Field 6:\t\t");
	for (int i = 0; i < image->header.id_length; i++) { printf("%u ", image->image_id[i]); }
	puts("");
	printf("[DEV]Field 7:\t\t\n");
	uint16_t color_map_length = image->header.color_map_specification[2]
			+ (image->header.color_map_specification[3] << 8);

	for (int i = 0; i < color_map_length; i++) {
		printf("\tColormap %d\t%u %u %u %u\n", i,
				image->color_map[i].b,
				image->color_map[i].g,
				image->color_map[i].a,
				image->color_map[i].r);
	}



	printf("[DEV]width:\t\t%u\n", image->width);
	printf("[DEV]height:\t\t%u\n", image->height);
	printf("[DEV]pixels:\t\t%u\n", image->pixels[0]);
	for (uint32_t i = 0; i < image->height; i++) {
		for (uint32_t j = 0; j < image->width; j++) {
			printf("w%uh%u:\t\t%u\n", i, j, image->pixels[i*image->width+j]);
		}
		puts("");
	}
}

tt_image* tt_load_from_file(const char *file_path) {
	FILE *image_stream = fopen(file_path, "rb");
	if (image_stream == NULL) {
		fprintf(stderr, "[ERR]File %s not found\n", file_path);
		exit(1);
	}
	// get image file length
	fseek(image_stream, 0, SEEK_END);
	long image_size = ftell(image_stream);
	fseek(image_stream, 0, SEEK_SET);
	// read image file
	uint8_t *image_raw = (uint8_t*)malloc(image_size);
	fread(image_raw, sizeof(uint8_t), image_size, image_stream);
	// 18 bytes header
	tt_image *image = (tt_image*)malloc(sizeof(tt_image));
	image->header.id_length = image_raw[0];
	image->header.color_map_type = image_raw[1];
	image->header.image_type = image_raw[2];
	image->header.color_map_specification = (uint8_t*)malloc(5); // the length of field 4 is 5 bytes
	for (int i = 0; i < 5; i++) { image->header.color_map_specification[i] = image_raw[i+3]; }
	image->header.image_specification = (uint8_t*)malloc(10);	 // the length of field 5 is 10 bytes
	for (int i = 0; i < 10; i++) { image->header.image_specification[i] = image_raw[i+8]; }


	long cp = HEADER_LENGTH; // current_position
	// ID, color map and pixels
	// ID
	// fread(image->image_id, sizeof(uint8_t), image->header.id_length, image_stream);
	if (image->header.id_length != 0) {
		image->image_id = (uint8_t*)malloc(image->header.id_length);
		for (int i = 0; i < image->header.id_length; i++) {
			image->image_id[i] = image_raw[cp+i];
		}
		cp += image->header.id_length;
	}

	// color map
	if (image->header.color_map_type == 1) {
		uint16_t color_map_length = image->header.color_map_specification[2] +
			(image->header.color_map_specification[3] << 8);
		uint8_t color_map_entry_size = image->header.color_map_specification[4];
		assert(color_map_entry_size == 32); // TODO: add more colormap entry size support, currently 32 bit only
		image->color_map = (tt_color*)malloc(sizeof(tt_color)*color_map_length);
		// bgra in order
		for (int i = 0; i < color_map_length; i++) {
			image->color_map[i].b = image_raw[cp++];
			image->color_map[i].g = image_raw[cp++];
			image->color_map[i].r = image_raw[cp++];
			image->color_map[i].a = image_raw[cp++];
		}
	}

	// image
	image->width = image->header.image_specification[4] +
		(image->header.image_specification[5] << 8);
	image->height = image->header.image_specification[6] +
		(image->header.image_specification[7] << 8);

	uint8_t pixel_depth = image->header.image_specification[8];
	switch (image->header.image_type) {
		case 0:		// No Image Data Included
			break;
		case 1:		// Uncompressed, Color mapped image
			// TODO: color map to true color
			image->pixels = (uint32_t*)malloc(sizeof(uint32_t)*image->height*image->width);
			switch (pixel_depth) {
				case 32:
					for (int i = 0; i < image->width*image->height; i++) {
						uint32_t color_index = 0;
						color_index += image_raw[cp++];
						color_index += image_raw[cp++] << 8;
						color_index += image_raw[cp++] << 16;
						color_index += image_raw[cp++] << 24;
						// TODO: color map entry size support more
						assert(image->header.color_map_specification[4] == 32);
						uint32_t color_value = (image->color_map[color_index-1].b << 24)
							+ (image->color_map[color_index-1].g << 16)
							+ (image->color_map[color_index-1].r << 8)
							+ (image->color_map[color_index-1].a); // bgra
						image->pixels[i] = color_value;
					}
					break;
				case 24:
					for (int i = 0; i < image->width*image->height; i++) {
						uint32_t color_index = 0;
						color_index += image_raw[cp++];
						color_index += image_raw[cp++] << 8;
						color_index += image_raw[cp++] << 16;
						// TODO: color map entry size support more
						assert(image->header.color_map_specification[4] == 32);
						uint32_t color_value = (image->color_map[color_index-1].b << 24)
							+ (image->color_map[color_index-1].g << 16)
							+ (image->color_map[color_index-1].r << 8)
							+ (image->color_map[color_index-1].a); // bgra
						image->pixels[i] = color_value;
					}
					break;
				case 16:
					for (int i = 0; i < image->width*image->height; i++) {
						uint32_t color_index = 0;
						color_index += image_raw[cp++];
						color_index += image_raw[cp++] << 8;
						// TODO: color map entry size support more
						assert(image->header.color_map_specification[4] == 32);
						uint32_t color_value = (image->color_map[color_index-1].b << 24)
							+ (image->color_map[color_index-1].g << 16)
							+ (image->color_map[color_index-1].r << 8)
							+ (image->color_map[color_index-1].a); // bgra
						image->pixels[i] = color_value;
					}
					break;
				case 8:
					for (int i = 0; i < image->width*image->height; i++) {
						uint32_t color_index = image_raw[cp++];
						// TODO: color map entry size support more
						assert(image->header.color_map_specification[4] == 32);
						uint32_t color_value = (image->color_map[color_index-1].b << 24)
							+ (image->color_map[color_index-1].g << 16)
							+ (image->color_map[color_index-1].r << 8)
							+ (image->color_map[color_index-1].a); // bgra
						image->pixels[i] = color_value;
					}
					break;
				default:
					assert(false);
					break;
			}
			break;
		case 2:		// Uncompressed, True Color Image
			// TODO: true color
			image->pixels = (uint32_t*)malloc(sizeof(uint32_t)*image->height*image->width);
			switch (pixel_depth) {
				case 32:
					for (int i = 0; i < image->width*image->height; i++) {
						uint32_t color_value = 0;
						color_value += image_raw[cp++] << 24;
						color_value += image_raw[cp++] << 16;
						color_value += image_raw[cp++] << 8;
						color_value += image_raw[cp++];
						image->pixels[i] = color_value;
					}
					break;
				case 24:
					for (int i = 0; i < image->width*image->height; i++) {
						uint32_t color_value = 0;
						color_value += image_raw[cp++] << 16;
						color_value += image_raw[cp++] << 8;
						color_value += image_raw[cp++];
						image->pixels[i] = color_value;
					}
					break;
				case 16:
					for (int i = 0; i < image->width*image->height; i++) {
						uint32_t color_value = 0;
						color_value += image_raw[cp++] << 8;
						color_value += image_raw[cp++];
						image->pixels[i] = color_value;
					}
					break;
				case 8:
					for (int i = 0; i < image->width*image->height; i++) {
						image->pixels[i] = image_raw[cp++];
					}
					break;
				default:
					assert(false);
					break;
			}
			break;
		case 9:		// Run-length encoded, Color mapped image
			break;
		case 11:	// Run-Length encoded, Black and white image
			break;
		default:
			printf("[ERR]Unsupport image type: %u\n", image->header.image_type);
			assert(false);
			break;
	}


	dev_log(image);
	fclose(image_stream);
	return image;
}
