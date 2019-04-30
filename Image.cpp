#include "Image.h"
#include "lvandeve/lodepng.h"

#include <iostream>
#include <fstream>
#include <assert.h>

Image::Image(unsigned int _width, unsigned int _height) {
    resize(_width, _height);
}

void Image::resize(unsigned int _width, unsigned int _height) {
    width = _width;
    height = _height;
    pixels_.resize(width * height);
}

vec3& Image::operator()(unsigned int _x, unsigned int _y) {
    assert(_x < width);
    assert(_y < height);
    return pixels_[_y*static_cast<unsigned int>(width) + _x];
}

const vec3& Image::operator()(unsigned int _x, unsigned int _y) const {
    assert(_x < width);
    assert(_y < height);
    return pixels_[_y*static_cast<unsigned int>(width) + _x];
}

bool Image::write(const std::string &_filename) const {
    std::vector<uint8_t> image_data(pixels_.size() * 3);
    for (unsigned int y = 0; y < height; ++y) {
        for (unsigned int x = 0; x < width; ++x) {
            unsigned int row = height - 1 - y;
            for (uint8_t c = 0; c < 3; ++c)
                image_data[3 * (row * width + x) + c] = static_cast<unsigned char>(255.0 * (*this)(x, y)[c]);
        }
    }
    return lodepng::encode(_filename, image_data, width, height, LCT_RGB);
}
