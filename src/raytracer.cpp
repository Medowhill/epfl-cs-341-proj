#include "Scene.h"
#include "Image.h"
#include "Light.h"
#include "StopWatch.h"
#include "Object.h"

#include "json.hpp"
#include <opencv2/core.hpp>
#include <opencv2/videoio.hpp>
#include <iostream>
#include <fstream>
#include <vector>
#include <unistd.h>

using json = nlohmann::json;

static void print_help();

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

    // Create a camera
    Camera camera(config["camera"]);
    bool write_video = camera.duration > 0;

    // Print information
    std::string output_ = config["out"];
    if (!output) output = output_.c_str();
    std::cout << "Input file: " << input << std::endl;
    std::string image_name = std::string(output) + ".png";
    std::string video_name = std::string(output) + ".avi";
    std::cout << "Output files: " << (write_video ? video_name : image_name) << std::endl;
    std::cout << "Debug mode " << (debug ? "enabled" : "disabled") << std::endl;

    // Create lights
    std::vector<Light> lights;
    for (const json &light : config["lights"])
        lights.push_back(Light(light));
    if (lights.empty()) lights.push_back(Light(camera.eye_position(), vec3(1)));

    // Create objects
    std::vector<Object *> objects;
    for (const json &obj : config["objects"])
        objects.push_back(create_object(obj));

    // Render images
    Scene s(camera, lights, objects, config["scene"], debug, shadow, occlusion);
    StopWatch timer;
    cv::VideoWriter video;
    if (write_video) {
        int codec = cv::VideoWriter::fourcc('M','J','P','G');
        if (!video.open(video_name, codec, 20, cv::Size(camera.width, camera.height), true)) {
            std::cerr << "Fail to write the video to " << video_name << '\n' << std::flush;
            exit(1);
        }
    }
    double time = 0;
    do {
        unsigned long t = camera.current_time();
        timer.start();
        Image *image = s.render();
        timer.stop();
        std::cout << '(' << t << '/' << camera.duration << ") Time elapsed: " << timer << std::endl;
        time += timer.elapsed();
        if (write_video)
            image->write_to_video(video);
        else if (!image->write(image_name))
            std::cerr << "Fail to write the image to " << image_name << '\n' << std::flush;
        delete image;
    } while (camera.move());
    std::cout << "Average time per a frame: " << (time / (camera.duration + 1)) << " ms/frame" << std::endl;
    for (Object *obj : objects) delete obj;
}

static void print_help() {
    std::cerr << "Usage: build/raytracer -i [name].json\n";
    std::cerr << "       -a           enable ambient occlusion\n";
    std::cerr << "       -d           debug mode\n";
    std::cerr << "       -h           print help message\n";
    std::cerr << "       -o [name]    specify output paths\n";
    std::cerr << "       -s           enable shadows\n";
    std::cerr << "       -S           enable soft shadows\n";
    std::cerr << std::flush;
}
