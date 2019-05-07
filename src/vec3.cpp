#include "vec3.h"

vec3::vec3() {}
vec3::vec3(double _x) {
    data_[0] = _x;
    data_[1] = _x;
    data_[2] = _x;
}
vec3::vec3(double _x, double _y, double _z) {
    data_[0] = _x;
    data_[1] = _y;
    data_[2] = _z;
}
vec3::vec3(const json &_j) {
    std::vector<double> _v = _j;
    assert(_v.size() == 3);
    data_[0] = _v[0];
    data_[1] = _v[1];
    data_[2] = _v[2];
}

double& vec3::operator[](unsigned int _i) {
    assert(_i < 3);
    return data_[_i];
}
const double vec3::operator[](unsigned int _i) const {
    assert(_i < 3);
    return data_[_i];
}
vec3& vec3::operator*=(const double s) {
    for (int i=0; i<3; ++i) data_[i] *= s;
    return *this;
}
vec3& vec3::operator/=(const double s) {
    for (int i=0; i<3; ++i) data_[i] /= s;
    return *this;
}
vec3& vec3::operator*=(const vec3& v) {
    for (int i=0; i<3; ++i) data_[i] *= v[i];
    return *this;
}
vec3& vec3::operator-=(const vec3& v) {
    for (int i=0; i<3; ++i) data_[i] -= v[i];
    return *this;
}
vec3& vec3::operator+=(const vec3& v) {
    for (int i=0; i<3; ++i) data_[i] += v[i];
    return *this;
}
