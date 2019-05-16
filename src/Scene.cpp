#include "Scene.h"
#include "Ray.h"

#include <cmath>
#include <math.h>
#include <vector>

Scene::Scene(Camera &_camera, const std::vector<Light> &_lights, const std::vector<Object *> &_objects,
    const json &_j, bool _debug, shadow_type _shadow, bool _ambient_occlusion) :
    camera(_camera), lights(_lights), objects(_objects), debug(_debug), shadow(_shadow), ambient_occlusion(_ambient_occlusion),
    distribution(0.0, 1.0), background(_j["background"]), ambience(_j["ambience"]), max_depth(_j["max_depth"]), max_ray_steps(_j["max_ray_steps"]),
    monte_carlo_iter(_j["monte_carlo_iter"]), min_distance(_j["min_distance"]), max_distance(_j["max_distance"]), normal_distance(_j["normal_distance"]),
    shadow_margin(_j["shadow_margin"]), light_radius(_j["light_radius"]) {}

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
    int ind;
    if (intersect(_ray, dist, ind)) {
        if (debug) return vec3(1);

        vec3 point = _ray(dist);
        const Material &material = objects[ind]->material;

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

float Scene::de(const vec3 &_point, int &_ind) const {
    float min = max_distance + 1;
    for (int i = 0; i < objects.size(); i++) {
        float d = objects[i]->de(_point);
        if (d < min) {
            min = d;
            _ind = i;
        }
    }
    return min;
}

float Scene::de(const vec3 &_point) const {
    int i;
    return de(_point, i);
}

bool Scene::intersect(const Ray &_ray, float &_distance, int &_ind) const {
    _distance = 0;
    int steps;
    for (steps = 0; steps < max_ray_steps; steps++) {
        float d = de(_ray(_distance), _ind);
        _distance += d;
        if (_distance > max_distance) return false;
        if (d < min_distance) break;
    }
    return steps != max_ray_steps;
}

bool Scene::intersect(const Ray &_ray, float &_distance) const {
    int i;
    return intersect(_ray, _distance, i);
}

bool Scene::intersect(const Ray &_ray) const {
    float d;
    return intersect(_ray, d);
}

vec3 Scene::estimate_normal(const vec3 &_point) const {
    vec3 x(normal_distance, 0, 0), y(0, normal_distance, 0), z(0, 0, normal_distance);
    return normalize(vec3(
        de(_point + x) - de(_point - x),
        de(_point + y) - de(_point - y),
        de(_point + z) - de(_point - z)));
}

void Scene::orthonormal_vectors(const vec3 &z, vec3 &x, vec3 &y) const {
    int max_i = 0;
    double max_v = std::abs(z[0]);
    for (int i = 1; i < 3; i++) {
        double v = std::abs(z[i]);
        if (max_v < v) {
            max_v = v;
            max_i = i;
        }
    }
    x = vec3(1);
    x[max_i] = -(z[(max_i + 1) % 3] + z[(max_i + 2) % 3]) / z[max_i];
    x = normalize(x);
    y = normalize(cross(x, z));
}

double Scene::rand(double max) {
    return distribution(generator) * max;
}

double Scene::soft_shadow(const vec3 &_light, const vec3 &_point) {
    vec3 x, y;
    const vec3 z = normalize(_light - _point);
    orthonormal_vectors(z, x, y);

    double sum = 0;
    float t;
    for (int i = 0; i < monte_carlo_iter; i++) {
        double r = rand(light_radius);
        double theta = rand(2 * M_PI);
        const vec3 light = _light + x * r * cos(theta) + y * r * sin(theta);
        vec3 dir = light - _point;
        double distance = norm(dir);
        dir /= distance;
        Ray ray(_point + shadow_margin * dir, dir);
        if (intersect(ray, t) && t < distance) sum += r;
    }

    double res = 2 * sum / monte_carlo_iter / light_radius;
    return std::min(res, 1.0);
}

double Scene::occlusion(const vec3 &_point, const vec3 &_normal) {
    vec3 x, y;
    const vec3 &z = _normal;
    orthonormal_vectors(z, x, y);

    double sum = 0.0;
    for (int i = 0; i < monte_carlo_iter; i++) {
        double phi = distribution(generator) * M_PI * 2.0;
        double theta = distribution(generator) * M_PI * 0.5;
        double s = sin(theta);
        vec3 dir = x * s * cos(phi) + y * s * sin(phi) + z * cos(theta);
        Ray ray(_point + shadow_margin * dir, dir);
        if (intersect(ray)) sum += s;
    }
    double res = sum * M_PI / 2 / monte_carlo_iter;
    return std::min(res, 1.0);
}

vec3 Scene::lighting(const vec3 &_point, const vec3 &_normal, const vec3 &_view, const Material &_material) {
    vec3 color =
         (ambient_occlusion ? (1.0 - occlusion(_point, _normal)) : 1.0) *
         ambience * _material.ambient;

    for (const Light &light: lights) {
        vec3 l = normalize(light.position - _point);
        vec3 r = normalize(mirror(l, _normal));

        float t;
        double dd = dot(_normal, l), ds = dot(_view, r);
        double coeff =
            (shadow == none)   ? 1 : (
            (shadow == simple) ? ((intersect(Ray(_point + shadow_margin * l, l), t) && t < norm(_point - light.position)) ? 0 : 1) :
                                 (1.0 - soft_shadow(light.position, _point)));

        color += light.color * coeff * (
            _material.diffuse * std::max(dd, 0.0) +
            _material.specular * pow(std::max(ds, 0.0), _material.shininess)
        );
    }

    return color;
}
