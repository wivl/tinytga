#include "tinytga.h"
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

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
}

tt_image* tt_load_from_file(const char *file_path) {
	FILE *image_stream = fopen(file_path, "rb");
	if (image_stream == NULL) {
		fprintf(stderr, "[ERR]File %s not found\n", file_path);
		exit(1);
	}
	tt_image *image = (tt_image*)malloc(sizeof(tt_image));
	// 18 bytes header
	uint8_t header[18];
	fread(header, sizeof(uint8_t), HEADER_LENGTH, image_stream);
	image->header.id_length = header[0];
	image->header.color_map_type = header[1];
	image->header.image_type = header[2];
	image->header.color_map_specification = (uint8_t*)malloc(5); // the length of field 4 is 5 bytes
	for (int i = 0; i < 5; i++) { image->header.color_map_specification[i] = header[i+3]; }
	image->header.image_specification = (uint8_t*)malloc(10);	 // the length of field 5 is 10 bytes
	for (int i = 0; i < 10; i++) { image->header.image_specification[i] = header[i+8]; }

	dev_log(image);

	// ID, color map and pixels
	fread(image->image_id, sizeof(uint8_t), image->header.id_length, image_stream);


	fclose(image_stream);
	return image;
}
