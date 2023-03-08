# tinytga

English | [简体中文](./README_ZH.md)

tinytga is a simple C library in the form of a header file for reading, creating and saving [tga](https://en.wikipedia.org/wiki/Truevision_TGA) file formats.

## Support

Details of the support for tga file types are as follows:

| image type | value | support |
|------------|-------|---------|
|No image data included|0|✅|
|Uncompressed, color-mapped images|1|✅|
|Uncompressed, RGB images|2|✅|
|Uncompressed, black and white images|3|✅|
|Runlength encoded color-mapped images|9|❌|
|Runlength encoded RGB images|10|❌|
|Compressed, black and white images|11|❌|
|Compressed color-mapped data, using Huffman, Delta, and runlength encoding|32|❌|
|Compressed color-mapped data, using Huffman, Delta, and runlength encoding.  4-pass quadtree-type process|33|❌|

The support for pixel depth and colormap depth is as follows:

|colormap entry size|support|
|-------------------|-------|
|15|❌|
|16|✅|
|24|✅|
|32|✅|

|imagetype\pixeldepth|32|24|16|8|
|--------------------|--|--|--|-|
|Uncompressed, color-mapped images|✅|✅|✅|✅|
|Uncompressed, RGB images|✅|✅|❌|❌|
|Uncompressed, black and white images|-|-|-|✅|

## Examples

```C
#include "tinytga.h"
#include <assert.h>
#include <stdlib.h>

#define WIDTH 800
#define HEIGHT 600

int main(void) {
    tt_color black = tt_make_color(0xFF000000);
    tt_color white = tt_make_color(0xFFFFFFFF);

    tt_image *image = tt_create(WIDTH, HEIGHT, black);

    assert(image != NULL);

    // w and h starts from 0
    for (int h = 200-1; h <= 400-1; h++) {
        for (int w = 200-1; w <= 600-1; w++) {
            tt_set_color(image, w, h, white);
        }
    }

    const char *filename = "demo.tga";
    tt_save(image, filename);

    tt_destroy(image);

    return 0;
}
```

Here's the result:

(For showing in the README file I transform tga to png)

![demo.tga](./assets/demo.png)

More examples see [./examples](./examples).

## Document

See [./docs](./docs).

## TODOs

* Change to header-only library

## Links

More details on tga format can be found at:

* [TGA specification](https://www.gamers.org/dEngine/quake3/TGA.txt)
* [Truevision TGA FILE FORMAT SPECIFICATION Version 2.0](https://www.dca.fee.unicamp.br/~martino/disciplinas/ea978/tgaffs.pdf)
