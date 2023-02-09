#include <iostream>
#include <vector>
#include "BitmapPlusPlus/include/BitmapPlusPlus.hpp"
#include "argh/argh.h"
#include <fstream>
#include <filesystem>
#include "array_calculator.h"

using namespace argh;
using namespace std;
using namespace bmp;
using namespace filesystem;

struct color {
    uint8_t r;
    uint8_t g;
    uint8_t b;
};

int main(int argc, char **argv) {

    parser cmdl(argv);

    string image_name;
    if (!(cmdl("image") >> image_name)) {
        cerr << "You MUST specify image name!!" << endl;
        return EXIT_FAILURE;
    } else {
        cout << "Image name: " << image_name << endl;
    }
    if (exists(image_name)) {
        cout << "File exist." << endl;
    } else {
        cerr << "File not exist!! " << image_name << endl;
        return EXIT_FAILURE;
    }
    float rotate;
    int move_horizontal, move_vertical;
    float scale;
    cmdl({"scale", "s"}, 1.0f) >> scale;
    cmdl({"move_horizontal", "x"}, 0) >> move_horizontal;
    cmdl({"move_vertical", "y"}, 0) >> move_vertical;
    cmdl({"rotate", "r"}, 0.0f) >> rotate;

    Bitmap image;
    image.load(image_name);
    auto height = image.height();
    auto width = image.width();
    vector<vector<color>> image_vec(height);
    for (int h = 0; h < height; ++h) {
        image_vec.at(h).resize(width);
        for (int w = 0; w < width; ++w) {
            auto pixel = image.get(w, h);
            auto &i = image_vec.at(h).at(w);
            i.r = pixel.r;
            i.g = pixel.g;
            i.b = pixel.b;
        }
    }
    cout << unsigned(image_vec.at(100).at(100).r) << endl;
}