#include "Ray.h"

Ray::Ray(const vec3& _origin, const vec3& _direction) :
    origin(_origin), direction(normalize(_direction)) {}

vec3 Ray::operator()(double _t) const {
    return origin + _t * direction;
}
