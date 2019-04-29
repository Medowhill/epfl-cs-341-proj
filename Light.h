#ifndef LIGHT_H
#define LIGHT_H

#include "vec3.h"

class Light {
public:
    vec3 position;
    vec3 color;
    Light(vec3 &_position, vec3 &_color);
};

#endif
