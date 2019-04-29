#include "Scene.h"
#include "Image.h"
#include "Light.h"

#include <string>
#include <vector>

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
    Scene s(camera, lights, depth, background, ambience);

    Image img = s.render();
    img.write(std::string("test.tga"));
}
