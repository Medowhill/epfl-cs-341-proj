#ifndef VEC3_H
#define VEC3_H

#include <iostream>
#include <assert.h>
#include <math.h>
#include <vector>

class vec3 {
private:
    double data_[3];

public:
    vec3();
    vec3(double _x);
    vec3(double _x, double _y, double _z);
    vec3(std::vector<double> _v);

    double& operator[](unsigned int _i);
    const double operator[](unsigned int _i) const;
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
inline const double dot(const vec3& v0, const vec3& v1) {
    return (v0[0]*v1[0] + v0[1]*v1[1] + v0[2]*v1[2]);
}
inline const double norm(const vec3& v) {
    return sqrt(dot(v,v));
}
inline const vec3 normalize(const vec3& v) {
    const double n = norm(v);
    if (n != 0.0)
        return vec3(v[0] / n, v[1] / n, v[2] / n);
    return v;
}
inline const double distance(const vec3& v0, const vec3& v1) {
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
inline std::ostream& operator<<(std::ostream& os, const vec3& v) {
    os << '(' << v[0] << ", " << v[1] << ", " << v[2] << ')';
    return os;
}

#endif
