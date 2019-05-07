#ifndef TEXTURE_H
#define TEXTURE_H

#include "vec3.h"
#include "Material.h"

#include "json.hpp"
#include <vector>
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

class PerlinTexture : public Texture {
private:
    std::vector<vec3> gradients;
    const float freq_multiplier;
    const float ampl_multiplier;
    const int num_octaves;

    float noise(const vec3 &point) const;

protected:
    float fbm(const vec3 &point) const;
    float turbulence(const vec3 &point) const;

public:
    PerlinTexture(float _freq_multiplier, float _ampl_multiplier, int _num_octaves);
    virtual Material get_material(const vec3 &_point) const { throw std::logic_error("Unimplemented"); }
};

class MarbleTexture : public PerlinTexture {
private:
    const vec3 dark, light;

public:
    MarbleTexture(const json &_j);
    virtual Material get_material(const vec3 &_point) const;
};

inline const Texture &create_texture(const json &_j) {
    int id = _j["id"];
    switch(id) {
        case 0: return *(new SingleMaterialTexture(_j));
        case 1: return *(new MarbleTexture(_j));
        default: throw std::logic_error("Unknown texture ID");
    }
}

#endif
