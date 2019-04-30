#include "Light.h"

#include <vector>

Light::Light(const vec3 &_position, const vec3 &_color) :
    position(_position), color(_color) {}

Light::Light(const json &_j) {
    std::vector<double> _position = _j["position"];
    std::vector<double> _color = _j["color"];
    position = vec3(_position);
    color = vec3(_color);
}
