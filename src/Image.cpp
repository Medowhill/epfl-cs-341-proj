#include "Image.h"

#include "lodepng.h"
#include <iostream>
#include <fstream>
#include <assert.h>

Image::Image(unsigned int _width, unsigned int _height) {
    resize(_width, _height);
}

void Image::resize(unsigned int _width, unsigned int _height) {
    width = _width;
    height = _height;
    pixels.resize(width * height * 3);
}

void Image::set_pixel(unsigned int _x, unsigned int _y, const vec3 &_color) {
    assert(_x < width);
    assert(_y < height);

    unsigned int index = ((height - 1 - _y) * width + _x) * 3;
    for (unsigned int i = 0; i < 3; i++)
       pixels[index + i] = uint8_t(255.0 * _color[i]);
}

bool Image::write(const std::string &_filename) const {
    return !lodepng::encode(_filename, pixels, width, height, LCT_RGB);
}
