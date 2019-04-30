#ifndef MATERIAL_H
#define MATERIAL_H

#include "vec3.h"

#include "nlohmann/json.hpp"

using json = nlohmann::json;

class Material {
public:
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    double shininess;
    double mirror;

    Material(const vec3 &_ambient, const vec3 &_diffuse, const vec3 &_specular, double _shininess, double _mirror);
    Material(const json &_j);
};

#endif
