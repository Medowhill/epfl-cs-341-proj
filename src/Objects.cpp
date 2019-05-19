#include "Object.h"

#include <cmath>

Plane::Plane(const json &_j) : Object(_j), normal(normalize(vec3(_j["normal"]))), distance(_j["distance"]) {}

float Plane::de(const vec3 &_point) const {
    return std::abs(dot(_point, normal) - distance);
}

Sphere::Sphere(const json &_j) : Object(_j), radius(_j["radius"]), center(_j["center"]) {}

float Sphere::de(const vec3 &_point) const {
    return norm(_point - center) - radius;
}

InfiniteSpheres::InfiniteSpheres(const json &_j) : Object(_j), radius(_j["radius"]), distance(_j["distance"]) {}

float InfiniteSpheres::de(const vec3 &_point) const {
    vec3 np(round(_point[0] / distance) * distance, 0, round(_point[2] / distance) * distance);
    return norm(np - _point) - radius;
}

Mandelbulb::Mandelbulb(const json &_j) : Object(_j),
    iter(_j["iter"]), power(_j["power"]), bail_out(_j["bail_out"]), min_distance(_j["min_distance"]) {}

float Mandelbulb::de(const vec3 &_point) const {
    vec3 p = _point;
    float dr = 1, r;

    for (int i = 0; i < iter; i++) {
        r = norm(p);
        if (r > bail_out || r < min_distance) break;

        float theta = acos(p[2] / r);
        float phi = atan2(p[1], p[0]);
        dr = power * pow(r, power - 1) * dr + 1;

        theta *= power;
        phi *= power;
        float st = sin(theta), ct = cos(theta), sp = sin(phi), cp = cos(phi);
        p = pow(r, power) * vec3(st * cp, st * sp, ct) + _point;
    }

    return 0.5 * log(r) * r / dr;
}

Julia::Julia(const json &_j) : Object(_j),
    iter(_j["iter"]), bail_out(_j["bail_out"]), min_distance(_j["min_distance"]), k(_j["k"]), d(_j["d"]) {}

float Julia::de(const vec3 &_point) const {
    quat q = quat(_point, k);
    quat dq = quat(1, 0, 0, 0);
    double r;
    for (int i = 0; i < iter; i++) {
        dq = 2 * q * dq;
        q = q * q + d;
        r = size(q);
        if (r > bail_out || r < min_distance) break;
    }
    return 0.5 * log(r) * r / size(dq);
}
