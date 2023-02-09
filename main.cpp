#include <iostream>
//#include <vector>
#include "BitmapPlusPlus/include/BitmapPlusPlus.hpp"
#include "argh/argh.h"
#include <filesystem>
#include "array_calculator.h"

using namespace argh;
using namespace std;
using namespace bmp;
using namespace filesystem;

struct color {
    [[maybe_unused]] uint8_t r = 0;
    [[maybe_unused]] uint8_t g = 0;
    [[maybe_unused]] uint8_t b = 0;
};

int main([[maybe_unused]] int argc, char **argv) {

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
    //vector<vector<color>> image_vec(height);
    color **image_arr;
    image_arr = (color **) malloc(sizeof(color) * height);
    for (int i = 0; i < height; ++i) {
        image_arr[i] = (color *) malloc(sizeof(color) * width);
        for (int j = 0; j < width; ++j) {
            auto pixel = image.get(j, i);
            auto &item = image_arr[i][j];
            item.r = pixel.r;
            item.g = pixel.g;
            item.b = pixel.b;
        }
    }
    cout << unsigned(image_arr[100][100].r) << endl;
    //for (int h = 0; h < height; ++h) {
    //    image_vec.at(h).resize(width);
    //    for (int w = 0; w < width; ++w) {
    //        auto pixel = image.get(w, h);
    //        auto &i = image_vec.at(h).at(w);
    //        i.r = pixel.r;
    //        i.g = pixel.g;
    //        i.b = pixel.b;
    //    }
    //}
    //for (int i = 0; i < height; ++i) {
    //    image_arr[i] = image_vec.at(i).data();
    //}
    //cout << unsigned(image_vec.at(100).at(100).r) << endl;

}