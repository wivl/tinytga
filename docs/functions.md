# Functions

## Image

### tt_image* tt_create(uint16_t w, uint16_t h, tt_color color)

Create a **w** * **h** image and fill it with **color**.

```C
tt_color black = tt_make_color(0xFF000000);
tt_image *image = tt_create(800, 600, black);
```

### void tt_save(tt_image* image, const char* filename)

Save **image** to **filename**.

```C
const char *filename = "demo.tga";
tt_save(image, filename);
```

### tt_image* tt_load_from_file(const char* file_path)

Load .tga file.

```C
const char *file_path = "demo.tga";
tt_image *image = tt_load_from_file(file_path);
```

### void tt_destroy(tt_image* image)

Destroy image.

### void tt_flip_vertically(tt_image *image)

Flip **image** vertically.


## Color

### tt_color tt_make_color(uint32_t color_value)

Make color from an uint32_t **color value** and return.

> The color value is in the form of **0xAARRGGBB**.

```C
tt_color black = tt_make_color(0xFF000000);
```

### void tt_set_color(tt_image* image, int w, int h, tt_color color)

Set dot (**w**, **h**) to **color**.

> w and h starts from 0.

```C
tt_set_color(image, 200, 300, black);
```

### uint32_t tt_get_color_value(tt_color color)

Get color value from a tt_color variable.

```C
tt_color color = tt_make_color(0xFF986BEB);
uint32_t color_value = tt_get_color_value(color);
```

### uint32_t tt_get_color_value_from(tt_image *image, int w, int h)

Get color value of pixel (**w**, **h**) from **image**.

```C
uint32_t color_value = tt_get_color_value_from(image, 200, 300);
```

### tt_color tt_get_color_from(tt_image *image, int w, int h)

Get color of pixel (**w**, **h**) from **image**.

```C
tt_color color = tt_get_color_from(image, 200, 300);
```

### void tt_color_intensity(tt_color *color, float intensity)

Set color intensity, by setting the r, g and b value to r*intensity, g*intensity, b*intensity, and a value remains

```C
tt_color = tt_make_color(0xFFFFFFFF);
tt_color_intensity(&color, 0.5); // color = (255, 127, 127, 127) ARGB
```
