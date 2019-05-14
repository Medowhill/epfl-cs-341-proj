#ifndef SCENE_H
#define SCENE_H

#include "vec3.h"
#include "Image.h"
#include "Camera.h"
#include "Light.h"
#include "Material.h"
#include "DE.h"
#include "TexMap.h"

#include "json.hpp"
#include <random>

using json = nlohmann::json;

class Scene {
private:
    Camera &camera;
    const std::vector<Light> &lights;
    const DE &de;
    const TexMap &tex_map;
    const vec3 background;
    const vec3 ambience;
    const int max_depth;
    const int max_ray_steps;
    const int occlusion_steps;
    const float min_distance;
    const float max_distance;
    const float normal_distance;
    const float shadow_margin;
    const bool debug;
    const bool shadow;
    const bool ambient_occlusion;

    std::default_random_engine generator;
    std::uniform_real_distribution<double> distribution;

    vec3 trace(const Ray &_ray, int _depth);
    bool intersect(const Ray &_ray, float &_distance) const;
    vec3 estimate_normal(const vec3 &point) const;
    double occlusion(const vec3 &_point, const vec3 &_normal);
    vec3 lighting(const vec3 &_point, const vec3 &_normal, const vec3 &_view, const Material &_material);

public:
    Scene(Camera &_camera, const std::vector<Light> &_lights, const DE &_de, const TexMap &_tex_map,
        const json &_j, bool _debug, bool _shadow, bool _ambient_occlusion);
    Image render();
};

#endif
