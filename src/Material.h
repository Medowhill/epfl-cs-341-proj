#ifndef MATERIAL_H
#define MATERIAL_H

#include "vec3.h"

#include "json.hpp"

using json = nlohmann::json;

class Material {
public:
    const vec3 ambient;
    const vec3 diffuse;
    const vec3 specular;
    const double shininess;
    const double mirror;
    const bool transparent;
    const double ior;

    Material(const vec3 &_ambient, const vec3 &_diffuse, const vec3 &_specular,
      double _shininess, double _mirror, bool _transparent, double _ior);
    Material(const json &_j);
};

#endif
