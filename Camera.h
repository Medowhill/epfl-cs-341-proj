#ifndef CAMERA_H
#define CAMERA_H

#include "vec3.h"
#include "Ray.h"

class Camera {
private:
    vec3 x_dir;
    vec3 y_dir;
    vec3 lower_left;

public:
    vec3 eye;
    vec3 center;
    vec3 up;
    double fovy;
    unsigned int width;
    unsigned int height;

    Camera(const vec3& _eye, const vec3& _center, const vec3& _up, double _fovy, unsigned int _width, unsigned int _height);
    void init();
    Ray primary_ray(unsigned int _x, unsigned int _y) const;
};

#endif
