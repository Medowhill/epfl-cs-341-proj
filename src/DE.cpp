#include "DE.h"

float SpheresDE::operator()(const vec3 &_point) const {
    float d = abs(dot(_point, plane_normal) - plane_z);
    for (const sphere &s : spheres) {
        float d1 = norm(_point - s.center) - s.radius;
        d = std::min(d, d1);
    }
    return d;
}

void SpheresDE::set_params(const json &_j) {
    std::vector<json> _spheres = _j["spheres"];
    for (const json &j : _spheres) {
        sphere s;
        s.radius = j["radius"];
        std::vector<double> _c = j["center"];
        s.center = vec3(_c);
        spheres.push_back(s);
    }
    plane_z = _j["plane_z"];
    std::vector<double> _n = _j["plane_normal"];
    plane_normal = vec3(_n);
}

float InfiniteSpheresDE::operator()(const vec3 &_point) const {
    vec3 np(round(_point[0] / distance) * distance, 0, round(_point[2] / distance) * distance);
    float d0 = norm(np - _point) - radius;
    float d1 = abs(dot(_point, vec3(0, 1, 0)) + radius);
    return std::min(d0, d1);
}

void InfiniteSpheresDE::set_params(const json &_j) {
    radius = _j["radius"];
    distance = _j["distance"];
}

float MandelbulbDE::operator()(const vec3 &_point) const {
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

void MandelbulbDE::set_params(const json &_j) {
    iter = _j["iter"];
    power = _j["power"];
    bail_out = _j["bail_out"];
    min_distance = _j["min_distance"];
}

float JuliaDE::operator()(const vec3 &_point) const {
    quat q = quat(_point, k);
    float dr = 1, r;

    for (int i = 0; i < iter; i++) {
        r = size(q);
        if (r > bail_out || r < min_distance) break;
        dr = 2 * r * dr;
        q *= q + d;
    }

    return 0.5 * log(r) * r / dr;
}

void JuliaDE::set_params(const json &_j) {
    iter = _j["iter"];
    bail_out = _j["bail_out"];
    min_distance = _j["min_distance"];
    k = _j["k"];
    std::vector<double> _d = _j["d"];
    d = quat(_d);
}
