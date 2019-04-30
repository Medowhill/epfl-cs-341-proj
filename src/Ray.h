#ifndef RAY_H
#define RAY_H

#include "vec3.h"

class Ray {

public:
    Ray(const vec3& _origin, const vec3& _direction);
    vec3 origin;
    vec3 direction;
    vec3 operator()(double _t) const;
};

#endif
