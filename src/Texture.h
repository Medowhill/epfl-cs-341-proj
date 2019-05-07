#ifndef TEXTURE_H
#define TEXTURE_H

#include "vec3.h"
#include "Material.h"

#include "json.hpp"
#include <stdexcept>

using json = nlohmann::json;

class Texture {
public:
    virtual ~Texture() {}
    virtual Material get_material(const vec3 &_point) const { throw std::logic_error("Unimplemented"); }
};

class SingleMaterialTexture : public Texture {
private:
    Material material;
public:
    SingleMaterialTexture(const json &_j);
    virtual Material get_material(const vec3 &_point) const;
};

inline const Texture &create_texture(const json &_j) {
    int id = _j["id"];
    switch(id) {
        case 0: return *(new SingleMaterialTexture(_j));
        default: throw std::logic_error("Unknown texture ID");
    }
}

#endif
