#include "Scene.h"
#include "Ray.h"

#include <vector>

Scene::Scene(const Camera &_camera, const std::vector<Light> &_lights, const DE &_de, const json &_j, const Material &_material, bool _debug) :
    camera(_camera), lights(_lights), de(_de), debug(_debug), max_depth(_j["max_depth"]), max_ray_steps(_j["max_ray_steps"]),
    min_distance(_j["min_distance"]), max_distance(_j["max_distance"]), normal_distance(_j["normal_distance"]), material(_material) {
    std::vector<double> _background = _j["background"];
    std::vector<double> _ambience = _j["ambience"];
    background = vec3(_background);
    ambience = vec3(_ambience);
}

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

vec3 Scene::trace(const Ray &_ray, int _depth) {
    if (_depth > max_depth) return vec3(0);

    float dist;
    if (intersect(_ray, dist)) {
        if (debug) return vec3(1);

        vec3 point = _ray(dist);
        vec3 normal = estimate_normal(point);
        vec3 color = lighting(point, normal, normalize(camera.eye - point));

        if (material.mirror != 0) {
            vec3 direction = reflect(_ray.direction, normal);
            vec3 origin = point + direction * 0.01;
            Ray reflected_ray(origin, direction);
            return (1 - material.mirror) * color + material.mirror * trace(reflected_ray, _depth + 1);
        } else return color;
    } else return background;
}

bool Scene::intersect(const Ray &_ray, float &_distance) {
    _distance = 0;
    int steps;
    for (steps = 0; steps < max_ray_steps; steps++) {
        float d = de(_ray(_distance));
        _distance += d;
        if (_distance > max_distance) return false;
        if (d < min_distance) break;
    }
    return steps != max_ray_steps;
}

vec3 Scene::estimate_normal(const vec3 &point) {
    vec3 x(normal_distance, 0, 0), y(0, normal_distance, 0), z(0, 0, normal_distance);
    return normalize(vec3(
        de(point + x) - de(point - x),
        de(point + y) - de(point - y),
        de(point + z) - de(point - z)));
}

vec3 Scene::lighting(const vec3 &_point, const vec3 &_normal, const vec3 &_view) {
    vec3 color = ambience * material.ambient;

    for (Light &light: lights) {
      vec3 l = normalize(light.position - _point);
      vec3 r = mirror(l, _normal);
      Ray shadow_ray(_point + min_distance * 10 * l, l);

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
