#include "Scene.h"
#include "Image.h"
#include "Light.h"

#include <string>
#include <vector>

float sphere_de(const vec3 &_point);
float spheres_3_de(const vec3 &_point);
float spheres_lots_de(const vec3 &_point);

int main() {
    vec3 eye(1, 3, 8), center(1, 1, 0), up(0, 1, 0);
    double fovy = 45, width = 500, height = 500;
    Camera camera(eye, center, up, fovy, width, height);

    std::vector<Light> lights;
    vec3 pos1(0, 50, 0), pos2(50, 50, 50), pos3(-50, 50, 50), color(0.35);
    lights.push_back(Light(pos1, color));
    lights.push_back(Light(pos2, color));
    lights.push_back(Light(pos3, color));

    int depth = 5;
    vec3 ambience(0.2), background(0);
    Scene s(camera, lights, sphere_de, depth, background, ambience);

    Image img = s.render();
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

float spheres_lots_de(const vec3 &_point) {
    vec3 np(fmod(_point[0], 1.0), _point[1], fmod(_point[2], 1.0));
    return norm(np) - 0.3;
}
