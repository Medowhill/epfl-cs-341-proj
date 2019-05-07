#ifndef LIGHT_H
#define LIGHT_H

#include "vec3.h"

#include "json.hpp"

using json = nlohmann::json;

class Light {
public:
    const vec3 position;
    const vec3 color;

    Light(const vec3 &_position, const vec3 &_color);
    Light(const json &_j);
};

#endif
