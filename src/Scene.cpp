#include "Scene.h"
#include "Ray.h"

#include <cmath>
#include <math.h>
#include <vector>

Scene::Scene(Camera &_camera, const std::vector<Light> &_lights, const DE &_de, const TexMap &_tex_map,
    const json &_j, bool _debug, bool _shadow, bool _ambient_occlusion) :
    camera(_camera), lights(_lights), de(_de), tex_map(_tex_map), debug(_debug), shadow(_shadow), ambient_occlusion(_ambient_occlusion), distribution(0.0, 1.0),
    background(_j["background"]), ambience(_j["ambience"]), max_depth(_j["max_depth"]), max_ray_steps(_j["max_ray_steps"]), occlusion_steps(_j["occlusion_steps"]),
    min_distance(_j["min_distance"]), max_distance(_j["max_distance"]), normal_distance(_j["normal_distance"]), shadow_margin(_j["shadow_margin"]) {}

Image Scene::render() {
    Image img(camera.width, camera.height);

    auto raytrace_column = [&img, this](int x) {
        for (int y = 0; y < int(camera.height); y++) {
            Ray ray = camera.primary_ray(x, y);
            img(x,y) = min(trace(ray, 0), vec3(1));
        }
    };

#if defined(_OPENMP)
#pragma omp parallel for
#endif
    for (int x = 0; x < int(camera.width); x++)
        raytrace_column(x);

    return img;
}

vec3 Scene::trace(const Ray &_ray, int _depth) {
    if (_depth > max_depth) return vec3(0);

    float dist;
    if (intersect(_ray, dist)) {
        if (debug) return vec3(1);

        vec3 point = _ray(dist);
        Material material = tex_map.get_material(point);

        vec3 normal = estimate_normal(point);
        vec3 color = lighting(point, normal, normalize(camera.eye_position() - point), material);

        if (material.mirror != 0) {
            vec3 direction = reflect(_ray.direction, normal);
            vec3 origin = point + direction * 0.01;
            Ray reflected_ray(origin, direction);
            return (1 - material.mirror) * color + material.mirror * trace(reflected_ray, _depth + 1);
        } else return color;
    } else return background;
}

bool Scene::intersect(const Ray &_ray, float &_distance) const {
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

vec3 Scene::estimate_normal(const vec3 &point) const {
    vec3 x(normal_distance, 0, 0), y(0, normal_distance, 0), z(0, 0, normal_distance);
    return normalize(vec3(
        de(point + x) - de(point - x),
        de(point + y) - de(point - y),
        de(point + z) - de(point - z)));
}

double Scene::occlusion(const vec3 &_point, const vec3 &_normal) {
    vec3 z = _normal;
    int max_i = 0;
    double max_v = std::abs(z[0]);
    for (int i = 1; i < 3; i++) {
        double v = std::abs(z[i]);
        if (max_v < v) {
            max_v = v;
            max_i = i;
        }
    }
    vec3 x(1);
    x[max_i] = -(z[(max_i + 1) % 3] + z[(max_i + 2) % 3]) / z[max_i];
    x = normalize(x);
    vec3 y = normalize(cross(x, z));

    int n = 0;
    float t;
    for (int i = 0; i < occlusion_steps; i++) {
        double phi = distribution(generator) * M_PI * 2.0;
        double theta = distribution(generator) * M_PI * 0.5;
        vec3 dir = x * sin(theta) * cos(phi) + y * sin(theta) * sin(phi) + z * cos(theta);
        Ray ray(_point + shadow_margin * dir, dir);
        if (intersect(ray, t)) n++;
    }
    return double(n) / occlusion_steps;
}

vec3 Scene::lighting(const vec3 &_point, const vec3 &_normal, const vec3 &_view, const Material &_material) {
    vec3 color =
         (ambient_occlusion ? (1.0 - occlusion(_point, _normal)) : 1.0) *
         ambience * _material.ambient;

    for (const Light &light: lights) {
      vec3 l = normalize(light.position - _point);
      vec3 r = normalize(mirror(l, _normal));
      Ray shadow_ray(_point + shadow_margin * l, l);

      float t;
      if (!shadow || !(intersect(shadow_ray, t) && t < norm(_point - light.position))) {
          double dd = dot(_normal, l);
          if (dd > 0) color += light.color * _material.diffuse * dd;
          double ds = dot(_view, r);
          if (ds > 0) color += light.color * _material.specular * pow(ds, _material.shininess);
      }
    }

    return color;
}
