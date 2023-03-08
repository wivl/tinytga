## Structs

### tt_image

Definition:

```C
typedef struct {
	uint8_t image_type;   // simply ignore this
	uint16_t width;       // image width
	uint16_t height;      // image height
	uint8_t pixel_depth;  // pixel depth, if you create this image using tt_create and the value is 32
	uint32_t *pixels;     // pixel data (color map), true color
} tt_image;
```


### tt_color 

Definition:

```C
typedef struct {
	uint8_t b;
	uint8_t g;
	uint8_t r;
	uint8_t a;
} tt_color;
```

The tt_color can be transformed to color value (uint32_t) using tt_make_color and tt_get_color_value function, see [functions](./functions.md).

