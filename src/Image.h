#ifndef IMAGE_H
#define IMAGE_H

#include "vec3.h"

#include <opencv2/videoio.hpp>
#include <vector>

class Image {
private:
    unsigned int width;
    unsigned int height;
    std::vector<uint8_t> rs, gs, bs;

public:
    Image(unsigned int _width, unsigned int _height);
    void resize(unsigned int _width, unsigned int _height);
    void set_pixel(unsigned int _x, unsigned int _y, const vec3 &_color);
    bool write(const std::string &_filename) const;
    void write_to_video(cv::VideoWriter &_video) const;
};

#endif
