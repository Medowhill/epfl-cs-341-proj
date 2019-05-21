#ifndef CAMERA_H
#define CAMERA_H

#include "vec3.h"
#include "Ray.h"

#include "json.hpp"
#include <vector>

using json = nlohmann::json;

class Camera {
private:
    std::vector<vec3> control_polygon;
    vec3 eye;
    vec3 x_dir;
    vec3 y_dir;
    vec3 lower_left;
    unsigned long time;

    void compute_vecs();

public:
    const vec3 center;
    const vec3 up;
    const double fovy;
    const unsigned int width;
    const unsigned int height;
    const unsigned long duration;

    Camera(const std::vector<vec3> &_control_polygon, const vec3 &_center, const vec3 &_up,
        double _fovy, unsigned int _width, unsigned int _height, unsigned long _duration);
    Camera(const json &_j);
    bool move();
    Ray primary_ray(unsigned int _x, unsigned int _y) const;
    const vec3 &eye_position() const;
    unsigned long current_time() const;
};

#endif
