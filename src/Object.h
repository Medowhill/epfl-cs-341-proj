#ifndef DE_H
#define DE_H

#include "vec3.h"
#include "quat.h"
#include "Material.h"

#include "json.hpp"
#include <vector>
#include <stdexcept>

using json = nlohmann::json;

class Object {
public:
    const Material material;

    Object(const json &_j) : material(_j["material"]) {}
    virtual ~Object() {}
    virtual float de(const vec3 &_point) const { throw std::logic_error("Unimplemented"); }
};

class Plane : public Object {
private:
    const vec3 normal;
    const float distance;
public:
    Plane(const json &_j);
    virtual float de(const vec3 &_point) const;
};

class Sphere : public Object {
private:
    const vec3 center;
    const float radius;
public:
    Sphere(const json &_j);
    virtual float de(const vec3 &_point) const;
};

class InfiniteSpheres : public Object {
private:
    const float radius, distance;
public:
    InfiniteSpheres(const json &_j);
    virtual float de(const vec3 &_point) const;
};

class Tetra : public Object {
private:
    const int iter;
    const float plane_z;
    vec3 a1, a2, a3, a4;
public:
    Tetra(const json &_j);
    virtual float de(const vec3 &_point) const;
};

class Mandelbulb : public Object {
private:
    const int iter, power;
    const double bail_out, min_distance;
public:
    Mandelbulb(const json &_j);
    virtual float de(const vec3 &_point) const;
};

class Julia : public Object {
private:
    const int iter;
    const double bail_out, min_distance, k;
    const quat d;
public:
    Julia(const json &_j);
    virtual float de(const vec3 &_point) const;
};

inline Object *create_object(const json &_j) {
    int id = _j["id"];
    switch(id) {
        case 0: return new Plane(_j);
        case 1: return new Sphere(_j);
        case 2: return new InfiniteSpheres(_j);
        case 3: return new Tetra(_j);
        case 4: return new Mandelbulb(_j);
        case 5: return new Julia(_j);
        default: throw std::logic_error("Unknwon DE ID");
    }
}

#endif
