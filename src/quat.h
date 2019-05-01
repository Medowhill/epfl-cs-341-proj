#ifndef QUAT_H
#define QUAT_H

#include "vec3.h"

#include <vector>

class quat {
public:
    double r, i, j, k;

    quat();
    quat(double _r, double _i, double _j, double _k);
    quat(const vec3 &v, double _k);
    quat(const std::vector<double> &v);

    quat& operator+=(const quat &q);
    quat& operator-=(const quat &q);
    quat& operator*=(const double s);
    quat& operator*=(const quat &q);
    quat& operator/=(const double s);
};

inline const quat operator-(const quat& q) {
    return quat(-q.r, -q.i, -q.j, -q.k);
}
inline const quat operator+(const quat& q0, const quat& q1) {
    return quat(q0.r + q1.r, q0.i + q1.i, q0.j + q1.j, q0.k + q1.k);
}
inline const quat operator-(const quat& q0, const quat& q1) {
    return quat(q0.r - q1.r, q0.i - q1.i, q0.j - q1.j, q0.k - q1.k);
}
inline const quat operator*(const double s, const quat& q) {
    return quat(q.r * s, q.i * s, q.j * s, q.k * s);
}
inline const quat operator*(const quat& q, const double s) {
    return quat(q.r * s, q.i * s, q.j * s, q.k * s);
}
inline const quat operator*(const quat& q1, const quat& q2) {
    double a1 = q1.r, b1 = q1.i, c1 = q1.j, d1 = q1.k;
    double a2 = q2.r, b2 = q2.i, c2 = q2.j, d2 = q2.k;
    return quat(a1 * a2 - b1 * b2 - c1 * c2 - d1 * d2,
                a1 * b2 + b1 * a2 + c1 * d2 - d1 * c2,
                a1 * c2 - b1 * d2 + c1 * a2 + d1 * b2,
                a1 * d2 + b1 * c2 - c1 * b2 + d1 * a2);
}
inline const quat operator/(const quat& q, const double s) {
    return quat(q.r / s, q.i / s, q.j / s, q.k / s);
}
inline const double size(const quat& q) {
    return sqrt(q.r * q.r + q.i * q.i + q.j * q.j + q.k * q.k);
}
inline std::ostream& operator<<(std::ostream& os, const quat& q) {
    os << q.r << "+" << q.i << "i+" << q.j << "j+" << q.k << 'k';
    return os;
}

#endif
