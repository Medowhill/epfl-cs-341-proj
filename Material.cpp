#include "Material.h"

#include <vector>

Material::Material(const vec3 &_ambient, const vec3 &_diffuse, const vec3 &_specular, double _shininess, double _mirror) :
    ambient(_ambient), diffuse(_diffuse), specular(_specular), shininess(_shininess), mirror(_mirror) {}

Material::Material(const json &_j) : shininess(_j["shininess"]), mirror(_j["mirror"]) {
    std::vector<double> _ambient = _j["ambient"];
    std::vector<double> _diffuse = _j["diffuse"];
    std::vector<double> _specular = _j["specular"];
    ambient = vec3(_ambient);
    diffuse = vec3(_diffuse);
    specular = vec3(_specular);
}
