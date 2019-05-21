#include "Material.h"

#include <vector>

Material::Material(const vec3 &_ambient, const vec3 &_diffuse, const vec3 &_specular, double _shininess, double _mirror, bool _transparent, double _ior) :
    ambient(_ambient), diffuse(_diffuse), specular(_specular), shininess(_shininess), mirror(_mirror), transparent(_transparent), ior(_ior) {}

Material::Material(const json &_j) :
    ambient(_j["ambient"]), diffuse(_j["diffuse"]), specular(_j["specular"]),
    shininess(_j["shininess"]), mirror(_j["mirror"]), transparent(_j["transparent"]), ior(_j["ior"]) {}
