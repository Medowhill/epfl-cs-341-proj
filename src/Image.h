#ifndef IMAGE_H
#define IMAGE_H

#include "vec3.h"

#include <vector>

class Image {
private:
    unsigned int width;
    unsigned int height;
    std::vector<vec3> pixels_;

public:
    Image(unsigned int _width, unsigned int _height);
    void resize(unsigned int _width, unsigned int _height);
    vec3& operator()(unsigned int _x, unsigned int _y);
    const vec3& operator()(unsigned int _x, unsigned int _y) const;
    bool write(const std::string &_filename) const;
};

#endif
