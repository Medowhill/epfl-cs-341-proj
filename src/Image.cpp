#include "Image.h"

#include "lodepng.h"
#include <opencv2/core.hpp>
#include <iostream>
#include <fstream>
#include <assert.h>

Image::Image(unsigned int _width, unsigned int _height) {
    resize(_width, _height);
}

void Image::resize(unsigned int _width, unsigned int _height) {
    width = _width;
    height = _height;

    unsigned int size = width * height;
    rs.resize(size);
    gs.resize(size);
    bs.resize(size);
}

void Image::set_pixel(unsigned int _x, unsigned int _y, const vec3 &_color) {
    assert(_x < width);
    assert(_y < height);

    unsigned int index = _y * width + _x;
    rs[index] = uint8_t(255.0 * _color[0]);
    gs[index] = uint8_t(255.0 * _color[1]);
    bs[index] = uint8_t(255.0 * _color[2]);
}

bool Image::write(const std::string &_filename) const {
    std::vector<uint8_t> pixels;
    unsigned int size = width * height;
    pixels.resize(size * 3);
    for (unsigned int y = 0; y < height; y++) {
        for (unsigned int x = 0; x < width; x++) {
            unsigned int i = y * width + x;
            unsigned int pi = 3 * ((height - 1 - y) * width + x);
            pixels[pi] = rs[i];
            pixels[pi + 1] = gs[i];
            pixels[pi + 2] = bs[i];
        }
    }
    return !lodepng::encode(_filename, pixels, width, height, LCT_RGB);
}

void Image::write_to_video(cv::VideoWriter &_video) const {
    double size = height * width;
    cv::Mat mat, rm(height, width, CV_8UC1), gm(height, width, CV_8UC1), bm(height, width, CV_8UC1);
    memcpy(rm.data, rs.data(), size);
    memcpy(gm.data, gs.data(), size);
    memcpy(bm.data, bs.data(), size);
    std::vector<cv::Mat> channels { bm, gm, rm };
    merge(channels, mat);
    _video << mat;
}
