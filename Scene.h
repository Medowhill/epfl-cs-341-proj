#ifndef SCENE_H
#define SCENE_H

#include "vec3.h"
#include "Image.h"
#include "Camera.h"
#include "Light.h"
#include "Material.h"

typedef float (*DE)(const vec3&);

class Scene {
public:
    bool debug = false;

    Scene(Camera &_camera, std::vector<Light> &_lights, DE _de, int _max_depth, vec3 &_background, vec3 &_ambience);
    Image render();
    vec3 trace(const Ray& _ray, int _depth);
    bool intersect(const Ray& _ray, float &_distance);
    vec3 estimate_normal(const vec3& point);
    vec3 lighting(const vec3& _point, const vec3& _normal, const vec3& _view);

private:
    Camera camera;
    std::vector<Light> lights;
    DE de;
    int max_depth;
    vec3 background;
    vec3 ambience;

    int max_ray_steps = 500;
    float min_distance = 0.0001f;
    float normal_distance = 0.0001f;
    Material material = Material(vec3(0.5), vec3(0.5), vec3(1), 100, 0.3);
};

#endif
