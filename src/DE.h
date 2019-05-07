#ifndef DE_H
#define DE_H

#include "vec3.h"
#include "quat.h"

#include "json.hpp"
#include <vector>
#include <stdexcept>

using json = nlohmann::json;

class DE {
public:
    virtual ~DE() {}
    virtual float operator()(const vec3 &_point) const { throw std::logic_error("Unimplemented"); }
};

class SpheresDE : public DE {
private:
    struct sphere {
        vec3 center;
        float radius;
    };
    std::vector<sphere> spheres;
    const float plane_z;
    const vec3 plane_normal;
public:
    SpheresDE(const json &_j);
    virtual float operator()(const vec3 &_point) const;
};

class InfiniteSpheresDE : public DE {
private:
    float radius;
    float distance;
public:
    InfiniteSpheresDE(const json &_j);
    virtual float operator()(const vec3 &_point) const;
};

class TetraDE : public DE {
private:
    int iter;
    float plane_z;
    vec3 a1, a2, a3, a4;
public:
    TetraDE(const json &_j);
    virtual float operator()(const vec3 &_point) const;
};

class MandelbulbDE : public DE {
private:
    int iter, power;
    double bail_out, min_distance;
public:
    MandelbulbDE(const json &_j);
    virtual float operator()(const vec3 &_point) const;
};

class JuliaDE : public DE {
private:
    int iter;
    double bail_out, min_distance, k;
    quat d;
public:
    JuliaDE(const json &_j);
    virtual float operator()(const vec3 &_point) const;
};

inline const DE &create_DE(const json &_j) {
    int id = _j["id"];
    switch(id) {
        case 0: return *(new SpheresDE(_j));
        case 1: return *(new InfiniteSpheresDE(_j));
        case 2: return *(new TetraDE(_j));
        case 3: return *(new MandelbulbDE(_j));
        case 4: return *(new JuliaDE(_j));
        default: throw std::logic_error("Unknwon DE ID");
    }
}

#endif
