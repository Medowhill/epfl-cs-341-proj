#include "Light.h"

#include <vector>

Light::Light(const vec3 &_position, const vec3 &_color) :
    position(_position), color(_color) {}

Light::Light(const json &_j) : position(static_cast<const json &>(_j["position"])), color(_j["color"]) {}
