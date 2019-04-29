#ifndef MATERIAL_H
#define MATERIAL_H

#include "vec3.h"

class Material {
public:
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    double shininess;
    double mirror;

    Material(vec3 _ambient, vec3 _diffuse, vec3 _specular, double _shininess, double _mirror);
};

#endif
