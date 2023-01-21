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
		uint16_t color_map_length = image->header.color_map_specification[2]
			+ (image->header.color_map_specification[3] << 8);
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

	switch (image->header.image_type) {
		case 0:		// No Image Data Included
			break;
		case 1:		// Uncompressed, Color mapped image
			break;
		case 2:		// Uncompressed, True Color Image
			break;
		case 9:		// Run-length encoded, Color mapped image
			break;
		case 11:	// Run-Length encoded, Black and white image
			break;
		default:
			assert(false);
			break;
	}


	dev_log(image);
	fclose(image_stream);
	return image;
}
