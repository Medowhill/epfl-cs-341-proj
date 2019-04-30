#include "Scene.h"
#include "Image.h"
#include "Light.h"
#include "StopWatch.h"
#include "tetra.h"

#include "nlohmann/json.hpp"
#include <iostream>
#include <fstream>
#include <vector>

#define TETRA_HELPER(a) d = norm(p - a); \
                        if (d < dist) { \
                            c = a; \
                            dist = d; \
                        }

float sphere_de(const vec3 &_point);
float spheres_3_de(const vec3 &_point);
float spheres_many_de(const vec3 &_point);
float tetra_de(const vec3 &_point);
float mandelbulb_de(const vec3 &_point);
float julia_de(const vec3 &_point);

using json = nlohmann::json;

int main(int argc, char **argv) {
    if (argc <= 1) return 1;

    std::ifstream i(argv[1]);
    json config;
    i >> config;

//    vec3 eye(-1.5, 0, 3), center(0, 0, 0), up(0, 1, 0);
    Camera camera(config["camera"]);

    std::vector<Light> lights;
    for (const json &light : config["lights"])
        lights.push_back(Light(light));
    if (lights.empty()) lights.push_back(Light(camera.eye, vec3(1)));

    std::vector<DE> des = { sphere_de, spheres_3_de, spheres_many_de, tetra_de, mandelbulb_de, julia_de };
    int id = config["estimator"]["id"];
    DE de = des[id];

    Material material(config["material"]);

    Scene s(camera, lights, de, config["scene"], material, argc > 2);

    StopWatch timer;
    timer.start();

    Image img = s.render();

    timer.stop();
    std::cout << timer << "\n";

    img.write(config["out"]);
}

float sphere_de(const vec3 &_point) {
    float d0 = norm(_point) - 1.5;
    float d1 = abs(dot(_point, vec3(0, 1, 0)) + 1.5);
    return std::min(d0, d1);
}

float spheres_3_de(const vec3 &_point) {
    float d1 = norm(_point - vec3(0, 1, 0)) - 1;
    float d2 = norm(_point - vec3(-1, 0.5, 2)) - 0.5;
    float d3 = norm(_point - vec3(3, 2, 1.5)) - 2;
    float d4 = abs(dot(_point, vec3(0, 1, 0)));
    return std::min(d1, std::min(d2, std::min(d3, d4)));
}

float spheres_many_de(const vec3 &_point) {
    vec3 np(round(_point[0]), 0, round(_point[2]));
    float d0 = norm(np - _point) - 0.3;
    float d1 = abs(dot(_point, vec3(0, 1, 0)) + 0.3);
    return std::min(d0, d1);
}

float tetra_de(const vec3 &_point) {
    const int iter = 4;

    vec3 p = _point;
    vec3 a1(1, 1, 1), a2(-1, -1, 1), a3(1, -1, -1), a4(-1, 1, -1);

    for (int i = 0; i < iter; i++) {
        vec3 c = a1;
        float dist = norm(p - a1), d;
        TETRA_HELPER(a2);
        TETRA_HELPER(a3);
        TETRA_HELPER(a4);
        p = 2 * p - c;
    }

    float d0 = tetra_distance(a1, a2, a3, a4, p) * pow(2, float(-iter));
    float d1 = abs(dot(_point, vec3(0, 1, 0)) + 2);
    return std::min(d0, d1);
}

float mandelbulb_de(const vec3 &_point) {
    const int iter = 20;
    const int power = 8;
    const int bail_out = 10000;

    vec3 p = _point;
    float dr = 1, r;

    for (int i = 0; i < iter; i++) {
        r = norm(p);
        if (r > bail_out) break;
        if (r < 0.0001f) break;

        float theta = acos(p[2] / r);
        float phi = atan2(p[1], p[0]);
        dr = power * pow(r, power - 1) * dr + 1;

        theta *= power;
        phi *= power;
        float st = sin(theta), ct = cos(theta), sp = sin(phi), cp = cos(phi);
        p = pow(r, power) * vec3(st * cp, st * sp, ct) + _point;
    }

    return 0.5 * log(r) * r / dr;
}

float julia_de(const vec3 &_point) {
    const int iter = 20;
    const int bail_out = 100;
    const vec3 d(0.6, -0.8 ,0.4);

    vec3 p = _point;
    float dr = 1, r;

    for (int i = 0; i < iter; i++) {
        r = norm(p);
        if (r > bail_out) break;
        if (r < 0.0001f) break;

        float theta = acos(p[2] / r);
        float phi = atan2(p[1], p[0]);
        dr = 2 * r * dr;

        theta *= 2;
        phi *= 2;
        float st = sin(theta), ct = cos(theta), sp = sin(phi), cp = cos(phi);
        p = r * r * vec3(st * cp, st * sp, ct) + d;
    }

    return 0.5 * log(r) * r / dr;
}
