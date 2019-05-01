#ifndef SCENE_H
#define SCENE_H

#include "vec3.h"
#include "Image.h"
#include "Camera.h"
#include "Light.h"
#include "Material.h"
#include "DE.h"

#include "json.hpp"

using json = nlohmann::json;

class Scene {
public:
    Scene(const Camera &_camera, const std::vector<Light> &_lights, const DE &_de, const json &_j, const Material &_material, bool _debug);
    Image render();
    vec3 trace(const Ray &_ray, int _depth);
    bool intersect(const Ray &_ray, float &_distance);
    vec3 estimate_normal(const vec3 &point);
    vec3 lighting(const vec3 &_point, const vec3 &_normal, const vec3 &_view);

private:
    Camera camera;
    std::vector<Light> lights;
    const DE &de;
    vec3 background;
    vec3 ambience;
    Material material;
    int max_depth;
    int max_ray_steps;
    float min_distance;
    float max_distance;
    float normal_distance;

    bool debug;
};

#endif
