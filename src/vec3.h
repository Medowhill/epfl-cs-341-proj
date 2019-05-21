#ifndef VEC3_H
#define VEC3_H

#include "json.hpp"
#include <iostream>
#include <assert.h>
#include <math.h>
#include <cmath>
#include <vector>
#include <algorithm>

using json = nlohmann::json;

class vec3 {
private:
    double data_[3];

public:
    vec3();
    vec3(double _x);
    vec3(double _x, double _y, double _z);
    vec3(const json &_j);

    double& operator[](unsigned int _i);
    double operator[](unsigned int _i) const;
    vec3& operator*=(const double s);
    vec3& operator/=(const double s);
    vec3& operator*=(const vec3& v);
    vec3& operator-=(const vec3& v);
    vec3& operator+=(const vec3& v);
};

inline const vec3 operator-(const vec3& v) {
    return vec3(-v[0], -v[1], -v[2]);
}
inline const vec3 operator*(const double s, const vec3& v ) {
    return vec3(s * v[0], s * v[1], s * v[2]);
}
inline const vec3 operator*(const vec3& v, const double s) {
    return vec3(s * v[0], s * v[1], s * v[2]);
}
inline const vec3 operator*(const vec3& v0, const vec3& v1) {
    return vec3(v0[0] * v1[0], v0[1] * v1[1], v0[2] * v1[2]);
}
inline const vec3 operator/(const vec3& v, const double s) {
    return vec3(v[0] / s, v[1] / s, v[2] / s);
}
inline const vec3 operator+(const vec3& v0, const vec3& v1) {
    return vec3(v0[0] + v1[0], v0[1] + v1[1], v0[2] + v1[2]);
}
inline const vec3 operator-(const vec3& v0, const vec3& v1) {
    return vec3(v0[0] - v1[0], v0[1] - v1[1], v0[2] - v1[2]);
}
inline const vec3 min(const vec3& v0, const vec3& v1) {
    return vec3(std::min(v0[0], v1[0]), std::min(v0[1], v1[1]), std::min(v0[2], v1[2]));
}
inline const vec3 max(const vec3& v0, const vec3& v1) {
    return vec3(std::max(v0[0], v1[0]), std::max(v0[1], v1[1]), std::max(v0[2], v1[2]));
}
inline double dot(const vec3& v0, const vec3& v1) {
    return (v0[0]*v1[0] + v0[1]*v1[1] + v0[2]*v1[2]);
}
inline double norm(const vec3& v) {
    return sqrt(dot(v,v));
}
inline const vec3 normalize(const vec3& v) {
    const double n = norm(v);
    if (n != 0.0)
        return vec3(v[0] / n, v[1] / n, v[2] / n);
    return v;
}
inline double distance(const vec3& v0, const vec3& v1) {
    return norm(v0-v1);
}
inline const vec3 cross(const vec3& v0, const vec3& v1) {
    return vec3(v0[1]*v1[2] - v0[2]*v1[1],
                v0[2]*v1[0] - v0[0]*v1[2],
                v0[0]*v1[1] - v0[1]*v1[0]);
}
inline const vec3 reflect(const vec3& v, const vec3& n) {
    return v - (2.0 * dot(n,v)) * n;
}
inline const vec3 mirror(const vec3& v, const vec3& n) {
    return (2.0 * dot(n,v)) * n - v;
}
inline const vec3 refract(const vec3 &v, const vec3 &n, float ior) {
    float cosi = std::max(std::min(dot(v, n), 1.0), -1.0);
    float etai = 1, etat = ior;
    vec3 normal_ref = n;
    if (cosi < 0) cosi = -cosi;
    else { etat = 1; etai = ior; normal_ref = -n; }
    float eta = etai / etat;
    float k = 1 - eta * eta * (1 - cosi * cosi);
    return (k < 0) ? vec3(0) : normalize(eta * v + (eta * cosi - sqrt(k)) * n);
}
inline float fresnel(const vec3 &v, const vec3 &n, float ior) {
    float cosi = std::max(std::min(dot(v, n), 1.0), -1.0);
    float etai = 1, etat = ior;
    if (cosi > 0) { etat = 1; etai = ior; }
    float sint = etai / etat * sqrt(std::max(1 - cosi * cosi, 0.f));
    if (sint >= 1.f) return 1.f;
    else {
        float cost = sqrt(std::max(1 - sint * sint, 0.f));
        cosi = std::abs(cosi);
        float Rs = ((etat * cosi) - (etai * cost)) / ((etat * cosi) + (etai * cost));
        float Rp = ((etai * cosi) - (etat * cost)) / ((etai * cosi) + (etat * cost));
        return (Rs * Rs + Rp * Rp) / 2;
    }
}
inline const vec3 mix(const vec3& v0, const vec3& v1, float alpha) {
    return v0 * (1 - alpha) + v1 * alpha;
}
inline std::ostream& operator<<(std::ostream& os, const vec3& v) {
    os << '(' << v[0] << ", " << v[1] << ", " << v[2] << ')';
    return os;
}

#endif
