#include "Scene.h"
#include "Image.h"
#include "Light.h"
#include "TexMap.h"
#include "StopWatch.h"
#include "DE.h"

#include "json.hpp"
#include <iostream>
#include <fstream>
#include <vector>
#include <unistd.h>

using json = nlohmann::json;

void print_help();

int main(int argc, char **argv) {
    // Parse command line arguments
    int c;
    bool debug = false, occlusion = false;
    shadow_type shadow = none;
    const char *input = NULL, *output = NULL;
    while((c = getopt(argc, argv, "adhsSi:o:")) != -1) {
        switch(c) {
            case 'a': occlusion = true; break;
            case 'd': debug = true;     break;
            case 'h': print_help();     exit(0);
            case 's': shadow = simple;  break;
            case 'S': shadow = soft;    break;
            case 'i': input = optarg;   break;
            case 'o': output = optarg;  break;
        }
    }
    if (!input) {
        std::cerr << "An input JSON file is not given.\n";
        print_help();
        exit(1);
    }

    // Read an input JSON file
    std::ifstream i(input);
    json config;
    i >> config;

    // Print information
    std::string output_ = config["out"];
    if (!output) output = output_.c_str();
    std::cout << "Input file: " << input << std::endl;
    std::cout << "Output files: " << output << "[n].png" << std::endl;
    std::cout << "Debug mode " << (debug ? "enabled" : "disabled") << std::endl;

    // Create a camera
    Camera camera(config["camera"]);

    // Create lights
    std::vector<Light> lights;
    for (const json &light : config["lights"])
        lights.push_back(Light(light));
    if (lights.empty()) lights.push_back(Light(camera.eye_position(), vec3(1)));

    // Select a distance estimator
    const DE &de = create_DE(config["estimator"]);

    // Create a texture map
    TexMap tex_map(config["textures"]);

    // Render an image
    Scene s(camera, lights, de, tex_map, config["scene"], debug, shadow, occlusion);
    std::vector<Image> images;
    do {
        StopWatch timer;
        timer.start();
        images.push_back(s.render());
        timer.stop();
        std::cout << '(' << camera.current_time() << '/' << camera.duration << ") Time elapsed: " << timer << std::endl;
    } while (camera.move());
    delete &de;

    // Write the image to a file
    int n = 0;
    for (const Image &image : images) {
        std::string name = output + std::to_string(n++) + ".png";
        if (image.write(name))
            std::cout << "Succeed to write the image to " << name << std::endl;
        else {
            std::cerr << "Fail to write the image to " << name << '\n' << std::flush;
            exit(1);
        }
    }
}

void print_help() {
    std::cerr << "Usage: build/raytracer -i [name].json\n";
    std::cerr << "       -a           enable ambient occlusion\n";
    std::cerr << "       -d           debug mode\n";
    std::cerr << "       -h           print help message\n";
    std::cerr << "       -o [name]    specify output paths";
    std::cerr << "       -s           enable shadows\n";
    std::cerr << "       -S           enable soft shadows\n";
    std::cerr << std::flush;
}
