#include "Scene.h"

#include "Ray.h"

Scene::Scene(Camera &_camera, std::vector<Light> &_lights, int _max_depth, vec3 &_background, vec3 &_ambience) :
    camera(_camera), lights(_lights), max_depth(_max_depth), background(_background), ambience(_ambience) {}

Image Scene::render() {
    Image img(camera.width, camera.height);

    for (int x = 0; x < int(camera.width); x++) {
        for (int y = 0; y < int(camera.height); y++) {
            Ray ray = camera.primary_ray(x, y);
            img(x,y) = min(trace(ray, 0), vec3(1));
        }
    }
    return img;
}

vec3 Scene::trace(const Ray& _ray, int _depth) {
    if (_depth > max_depth) return vec3(0);

    float dist;
    if (intersect(_ray, dist)) {
        vec3 point = _ray(dist);
        vec3 normal = estimate_normal(point);
        vec3 color = lighting(point, normal, normalize(camera.eye - point));

        if (material.mirror != 0) {
            vec3 direction = reflect(_ray.direction, normal);
            vec3 origin = point + direction * 0.001;
            Ray reflected_ray(origin, direction);
            return (1 - material.mirror) * color + material.mirror * trace(reflected_ray, _depth + 1);
        } else return color;
    } else return background;
}

bool Scene::intersect(const Ray& _ray, float &_distance) {
    _distance = 0;
    int steps;
    for (steps = 0; steps < max_ray_steps; steps++) {
        float d = estimate_distance(_ray(_distance));
        _distance += d;
        if (d < min_distance) break;
    }
    return steps != max_ray_steps;
}

float Scene::estimate_distance(const vec3& point) {
    float d1 = norm(point - vec3(0, 1, 0)) - 1;
    float d2 = norm(point - vec3(-1, 0.5, 2)) - 0.5;
    float d3 = norm(point - vec3(3, 2, 1.5)) - 2;
    float d4 = dot(point, vec3(0, 1, 0));
    return std::min(d1, std::min(d2, std::min(d3, d4)));
}

vec3 Scene::estimate_normal(const vec3& point) {
    vec3 x(normal_distance, 0, 0), y(0, normal_distance, 0), z(0, 0, normal_distance);
    return normalize(vec3(
        estimate_distance(point + x) - estimate_distance(point - x),
        estimate_distance(point + y) - estimate_distance(point - y),
        estimate_distance(point + z) - estimate_distance(point - z)));
}

vec3 Scene::lighting(const vec3& _point, const vec3& _normal, const vec3& _view) {
    vec3 color = ambience * material.ambient;
    
    for (Light &light: lights) {
      vec3 l = normalize(light.position - _point);
      vec3 r = mirror(l, _normal);
      Ray shadow_ray(_point + 0.001 * l, l);

      float t;
      if (!(intersect(shadow_ray,  t) && t < norm(_point - light.position))) {
          double dd = dot(_normal, l);
          if (dd > 0) color += light.color * material.diffuse * dd;
          double ds = dot(_view, r);
          if (ds > 0) color += light.color * material.specular * pow(ds, material.shininess);
      }
    }

    return color;
}
