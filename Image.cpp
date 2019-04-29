#include "Image.h"

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
    std::ofstream file(_filename, std::fstream::binary);
    if (!file) return false;

    file.put(0); //id length
    file.put(0); //no color map
    file.put(2); //uncompressed image
    file.put(0); //offset color map table
    file.put(0); //
    file.put(0); //number of entries
    file.put(0); //
    file.put(0); //bits per pixel
    file.put(0); //abs coordinate lower left display in x direction
    file.put(0); //
    file.put(0); //abs coordinate lower left display in y direction
    file.put(0); //
    file.put((width  & 0x00FF)); //width in pixels
    file.put((width  & 0xFF00)/ 256);
    file.put((height & 0x00FF)); //height in pixels
    file.put((height & 0xFF00)/ 256);
    file.put(24); //bits per pixel
    file.put(0); //image descriptor

    for (const vec3 &color : pixels_) {
        file.put(static_cast<unsigned char>(255.0 * color[2]));
        file.put(static_cast<unsigned char>(255.0 * color[1]));
        file.put(static_cast<unsigned char>(255.0 * color[0]));
    }

    file.close();
    return true;
}
