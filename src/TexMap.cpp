#include "TexMap.h"

#define TEXMAP_CONSTR_PARSE(a) c.a = j.find(#a) != j.end(); \
                        if (c.a) { \
                            std::vector<float> fs = j[#a]; \
                            c.min_##a = fs[0]; \
                            c.max_##a = fs[1]; \
                        }

bool TexMap::Constraint::include(const vec3 &_point) const {
    return (!x || (min_x <= _point[0] && _point[0] <= max_x)) &&
           (!y || (min_y <= _point[1] && _point[1] <= max_y)) &&
           (!z || (min_z <= _point[2] && _point[2] <= max_z));
}

TexMap::TexMap(const json &_j) : default_tex(create_texture(_j["default"])) {
    for (const json &j : _j["constrained"]) {
        Constraint c;
        TEXMAP_CONSTR_PARSE(x)
        TEXMAP_CONSTR_PARSE(y)
        TEXMAP_CONSTR_PARSE(z)
        constraints.push_back(c);
        textures.push_back(&create_texture(j["texture"]));
    }
}

TexMap::~TexMap() {
    delete &default_tex;
    for (const Texture *t : textures)
        delete t;
}

const Texture &TexMap::get_texture(const vec3 &_point) const {
    for (int i = 0; i < constraints.size(); i++)
        if (constraints[i].include(_point))
            return *(textures[i]);
    return default_tex;
}

Material TexMap::get_material(const vec3 &_point) const {
    return get_texture(_point).get_material(_point);
}
