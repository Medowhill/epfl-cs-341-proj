#ifndef TEXMAP_H
#define TEXMAP_H

#include "vec3.h"
#include "Texture.h"
#include "Material.h"

#include "json.hpp"
#include <vector>

class TexMap {
private:
    struct Constraint {
        bool x, y, z;
        float min_x, max_x, min_y, max_y, min_z, max_z;
        bool include(const vec3 &_point) const;
    };

    const Texture &default_tex;
    std::vector<Constraint> constraints;
    std::vector<const Texture*> textures;

public:
    TexMap(const json &_j);
    ~TexMap();
    const Texture &get_texture(const vec3 &_point) const;
    Material get_material(const vec3 &_point) const;
};

#endif
