#include "Camera.h"

Camera::Camera(const vec3& _eye, const vec3& _center, const vec3& _up, double _fovy, unsigned int _width, unsigned int _height) :
    eye(_eye), center(_center), up(_up), fovy(_fovy), width(_width), height(_height) {
    init();
}

void Camera::init() {
    vec3 view = normalize(center - eye);
    double dist = distance(center, eye);

    double w = width;
    double h = height;
    double image_height = 2.0 * dist * tan(0.5 * fovy / 180.0 * M_PI);
    double image_width  = w / h * image_height;

    x_dir = normalize( cross(view, up) ) * image_width / w;
    y_dir = normalize( cross(x_dir, view) ) * image_height / h;

    lower_left = center - 0.5f * w * x_dir - 0.5f * h * y_dir;
}

Ray Camera::primary_ray(unsigned int _x, unsigned int _y) const {
    return Ray(eye, lower_left + static_cast<double>(_x) * x_dir + static_cast<double>(_y) * y_dir - eye);
}
