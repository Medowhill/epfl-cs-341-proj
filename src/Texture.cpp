#include "Texture.h"

SingleMaterialTexture::SingleMaterialTexture(const json &_j) : material(Material(_j["material"])) {}

Material SingleMaterialTexture::get_material(const vec3 &_point) const {
    return material;
}
