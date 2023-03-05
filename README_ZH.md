# tinytga

[English](./README.md) | 简体中文

tinytga 是一个功能简单的、针对 [tga](https://zh.wikipedia.org/wiki/Truevision_TGA) 图像文件的、头文件形式的 C 语言库，用于 tga 文件格式的读取，创建和保存。

## 支持

对 tga 文件类型支持情况的细节如下

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

其中，对 pixel 深度和 colormap 深度的支持如下

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

## 示例

> 待更新

见[文件夹](./examples)

## TODOs

* 更新示例文件
* 改成 header—only 形式的库

## 外部链接

更多 tga 格式细节可以参考

* [TGA specification](https://www.gamers.org/dEngine/quake3/TGA.txt)
* [Truevision TGA FILE FORMAT SPECIFICATION Version 2.0](https://www.dca.fee.unicamp.br/~martino/disciplinas/ea978/tgaffs.pdf)
