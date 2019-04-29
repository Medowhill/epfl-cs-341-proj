#include "Material.h"

Material::Material(vec3 _ambient, vec3 _diffuse, vec3 _specular, double _shininess, double _mirror) :
    ambient(_ambient), diffuse(_diffuse), specular(_specular), shininess(_shininess), mirror(_mirror) {}
