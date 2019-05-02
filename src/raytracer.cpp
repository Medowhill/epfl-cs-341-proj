#include "Scene.h"
#include "Image.h"
#include "Light.h"
#include "StopWatch.h"
#include "DE.h"

#include "json.hpp"
#include <iostream>
#include <fstream>
#include <vector>
#include <unistd.h>

using json = nlohmann::json;

int main(int argc, char **argv) {
    // Parse command line arguments
    int c;
    bool debug = false;
    const char *input = NULL, *output = NULL;
    while((c = getopt(argc, argv, "di:o:")) != -1) {
        switch(c) {
            case 'd':
                debug = true;
                break;
            case 'i':
                input = optarg;
                break;
            case 'o':
                output = optarg;
                break;
            case '?':
                break;
        }
    }
    if (!input) {
        std::cerr << "An input JSON file is not given.\n";
        std::cerr << "Usage: build/raytracer -i [name].json\n";
        std::cerr << "Usage: build/raytracer -i [name].json -o [name].png\n";
        std::cerr << "Usage: build/raytracer -i [name].json -d\n";
        std::cerr << std::flush;
        return 1;
    }

    // Read an input JSON file
    std::ifstream i(input);
    json config;
    i >> config;

    // Print information
    std::string output_ = config["out"];
    if (!output) output = output_.c_str();
    std::cout << "Input file: " << input << std::endl;
    std::cout << "Output file: " << output << std::endl;
    std::cout << "Debug mode " << (debug ? "enabled" : "disabled") << std::endl;

    // Create a camera
    Camera camera(config["camera"]);
//    vec3 eye(-1.5, 0, 3), center(0, 0, 0), up(0, 1, 0);

    // Create lights
    std::vector<Light> lights;
    for (const json &light : config["lights"])
        lights.push_back(Light(light));
    if (lights.empty()) lights.push_back(Light(camera.eye, vec3(1)));

    // Select a distance estimator
    const json &estimator_config = config["estimator"];
    DE *de = get_DE(estimator_config["id"]);
    de->set_params(estimator_config);

    // Create a material
    Material material(config["material"]);

    // Render an image
    Scene s(camera, lights, *de, config["scene"], material, debug);
    StopWatch timer;
    timer.start();
    Image img = s.render();
    timer.stop();
    std::cout << "Time elapsed: " << timer << std::endl;
    delete de;

    // Write the image to a file
    bool success = img.write(output);
    if (success) {
        std::cout << "Succeed to write the image to " << output << std::endl;
        return 0;
    } else {
        std::cerr << "Fail to write the image to " << output << '\n' << std::flush;
        return 1;
    }
}
