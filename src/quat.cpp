#include "quat.h"

#include <assert.h>

quat::quat() : r(0), i(0), j(0), k(0) {}
quat::quat(double _r, double _i, double _j, double _k) : r(_r), i(_i), j(_j), k(_k) {}
quat::quat(const vec3 &v, double _k) : r(v[0]), i(v[1]), j(v[2]), k(_k) {}
quat::quat(const std::vector<double> &v) {
    assert(v.size() == 4);
    r = v[0];
    i = v[1];
    j = v[2];
    k = v[3];
}

quat& quat::operator+=(const quat &q) {
    r += q.r;
    i += q.i;
    j += q.j;
    k += q.k;
    return *this;
}
quat& quat::operator-=(const quat &q) {
    r -= q.r;
    i -= q.i;
    j -= q.j;
    k -= q.k;
    return *this;
}
quat& quat::operator*=(const quat &q) {
    double a1 = r, b1 = i, c1 = j, d1 = k;
    double a2 = q.r, b2 = q.i, c2 = q.j, d2 = q.k;
    r = a1 * a2 - b1 * b2 - c1 * c2 - d1 * d2;
    i = a1 * b2 + b1 * a2 + c1 * d2 - d1 * c2;
    j = a1 * c2 - b1 * d2 + c1 * a2 + d1 * b2;
    k = a1 * d2 + b1 * c2 - c1 * b2 + d1 * a2;
    return *this;
}
quat& quat::operator*=(const double s) {
    r *= s;
    i *= s;
    j *= s;
    k *= s;
    return *this;
}
quat& quat::operator/=(const double s) {
    r /= s;
    i /= s;
    j /= s;
    k /= s;
    return *this;
}
