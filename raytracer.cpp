#include "Scene.h"
#include "Image.h"
#include "Light.h"
#include "StopWatch.h"
#include "tetra.h"

#include <string>
#include <vector>
#include <array>

#define TETRA_HELPER(a) d = norm(p - a); \
                        if (d < dist) { \
                            c = a; \
                            dist = d; \
                        }

float sphere_de(const vec3 &_point);
float spheres_3_de(const vec3 &_point);
float spheres_many_de(const vec3 &_point);
float tetra_de(const vec3 &_point);

int main(int argc, char **argv) {
    if (argc <= 1) return 1;

    vec3 eye(3, 4, 5), center(0, 0, 0), up(0, 1, 0);
    double fovy = 45, width = 500, height = 500;
    Camera camera(eye, center, up, fovy, width, height);

    std::vector<Light> lights;
    vec3 pos1(0, 50, 0), pos2(50, 50, 50), pos3(-50, 50, 50), color(0.33);
    lights.push_back(Light(pos1, color));
    lights.push_back(Light(pos2, color));
    lights.push_back(Light(pos3, color));

    int depth = 5;
    vec3 ambience(0.2), background(0);
    std::array<DE, 4> des = { sphere_de, spheres_3_de, spheres_many_de, tetra_de };
    Scene s(camera, lights, des[argv[1][0] - '0'], depth, background, ambience, argc > 2);

    StopWatch timer;
    timer.start();

    Image img = s.render();

    timer.stop();
    std::cout << timer << "\n";

    img.write(std::string("test.tga"));
}

float sphere_de(const vec3 &_point) {
    return norm(_point) - 1.5;
}

float spheres_3_de(const vec3 &_point) {
    float d1 = norm(_point - vec3(0, 1, 0)) - 1;
    float d2 = norm(_point - vec3(-1, 0.5, 2)) - 0.5;
    float d3 = norm(_point - vec3(3, 2, 1.5)) - 2;
    float d4 = dot(_point, vec3(0, 1, 0));
    return std::min(d1, std::min(d2, std::min(d3, d4)));
}

float spheres_many_de(const vec3 &_point) {
    vec3 np(round(_point[0]), 0, round(_point[2]));
    return norm(np - _point) - 0.3;
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
