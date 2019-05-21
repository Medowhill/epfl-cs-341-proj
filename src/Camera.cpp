#include "Camera.h"

#include <vector>

Camera::Camera(const std::vector<vec3> &_control_polygon, const vec3 &_center, const vec3 &_up,
    double _fovy, unsigned int _width, unsigned int _height, unsigned long _duration) :
    control_polygon(_control_polygon), center(_center), up(_up), fovy(_fovy), width(_width), height(_height), duration(_duration), time(0) {
    compute_vecs();
}

Camera::Camera(const json &_j) :
    center(_j["center"]), up(_j["up"]), fovy(_j["fovy"]), width(_j["width"]), height(_j["height"]), duration(_j["duration"]), time(0) {
    std::vector<json> _control_polygon = _j["control_polygon"];
    for (const json &j : _control_polygon)
        control_polygon.push_back(vec3(j));
    compute_vecs();
}

void Camera::compute_vecs() {
    assert(!control_polygon.empty());
    assert(time <= duration);

    std::vector<vec3> points = control_polygon;
    double t = double(time) / duration;
    int length = points.size();
    while (length-- > 1)
        for (int i = 0; i < length; i++)
            points[i] = mix(points[i], points[i + 1], t);
    eye = points[0];

    vec3 view = normalize(center - eye);
    double dist = distance(center, eye);

    double w = width;
    double h = height;
    double image_height = 2.0 * dist * tan(0.5 * fovy / 180.0 * M_PI);
    double image_width  = w / h * image_height;

    x_dir = normalize(cross(view, up)) * image_width / w;
    y_dir = normalize(cross(x_dir, view)) * image_height / h;
    lower_left = center - 0.5f * w * x_dir - 0.5f * h * y_dir;
}

bool Camera::move() {
    time++;
    if (time > duration) return false;
    compute_vecs();
    return true;
}

Ray Camera::primary_ray(unsigned int _x, unsigned int _y) const {
    return Ray(eye, lower_left + static_cast<double>(_x) * x_dir + static_cast<double>(_y) * y_dir - eye);
}

const vec3 &Camera::eye_position() const {
    return eye;
}

const unsigned long Camera::current_time() const {
    return time;
}
