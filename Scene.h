#ifndef SCENE_H
#define SCENE_H

#include "Image.h"
#include "Camera.h"
#include "Light.h"
#include "Material.h"

class Scene {
public:
    Scene(Camera &_camera, std::vector<Light> &_lights, int _max_depth, vec3 &_background, vec3 &_ambience);
    Image render();
    vec3 trace(const Ray& _ray, int _depth);
    bool intersect(const Ray& _ray, float &_distance);
    float estimate_distance(const vec3& point);
    vec3 estimate_normal(const vec3& point);
    vec3 lighting(const vec3& _point, const vec3& _normal, const vec3& _view);

private:
    Camera camera;
    std::vector<Light> lights;
    int max_depth;
    vec3 background;
    vec3 ambience;

    int max_ray_steps = 200;
    float min_distance = 0.0001f;
    float normal_distance = 0.0001f;
    Material material = Material(vec3(0.5), vec3(0.5), vec3(1), 100, 0.3);
};

#endif
