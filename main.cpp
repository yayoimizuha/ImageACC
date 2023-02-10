#include <iostream>
#include "BitmapPlusPlus/include/BitmapPlusPlus.hpp"
#include "argh/argh.h"
#include <filesystem>
#include "array_calculator.h"
#include <cmath>

using namespace argh;
using namespace std;
using namespace bmp;
using namespace filesystem;

struct color {
    [[maybe_unused]] uint8_t r = 0;
    [[maybe_unused]] uint8_t g = 0;
    [[maybe_unused]] uint8_t b = 0;
};

inline double **produce_array(double **array_A, double array_B[3][1]);

double PI = atan(1.0) * 4;

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
    cout << "scale: " << scale << endl;
    cout << "move_horizontal: " << move_horizontal << endl;
    cout << "move_vertical: " << move_vertical << endl;
    cout << "rotate: " << rotate << endl;

    Bitmap image;
    image.load(image_name);
    auto height = image.height();
    auto width = image.width();
    color **image_arr;
    image_arr = (color **) malloc(sizeof(color *) * height);
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

    const vector<vector<double>> pos = {{1, 0, width / 2.0},
                                        {0, 1, height / 2.0},
                                        {0, 0, 1}};
    const vector<vector<double>> scale_vec = {{scale, 0,     0},
                                              {0,     scale, 0},
                                              {0,     0,     1}};
    const vector<vector<double>> mov_vec = {{1, 0, static_cast<double>(move_horizontal)},
                                            {0, 1, -static_cast<double>(move_vertical)},
                                            {0, 0, 1}};
    const auto rot = rotate * 2.0 * PI / 360.0;
    const vector<vector<double>> rot_vec = {{cos(rot), -sin(rot), 0},
                                            {sin(rot), cos(rot),  0},
                                            {0,        0,         1}};
    const vector<vector<double>> de_pos = {{1, 0, -width / 2.0},
                                           {0, 1, -height / 2.0},
                                           {0, 0, 1}};
    const auto affine = Produce_array(Produce_array(Produce_array(Produce_array(
            pos, scale_vec), mov_vec), rot_vec), de_pos);
    for (auto &row: affine) {
        for (auto &item: row) {
            cout << item << ' ';
        }
        cout << endl;
    }
    auto affine_inv = Inverse_array(affine);
    for (auto &row: affine_inv) {
        for (auto &item: row) {
            cout << item << ' ';
        }
        cout << endl;
    }
    cout << "inv" << endl;
    double **affine_inv_arr;
    affine_inv_arr = (double **) malloc(sizeof(double *) * 3);
    for (int i = 0; i < 3; ++i) {
        affine_inv_arr[i] = affine_inv.at(i).data();
    }
    color **dest_arr;
    dest_arr = (color **) malloc(sizeof(color *) * height);
    for (int i = 0; i < height; ++i) {
        dest_arr[i] = (color *) malloc(sizeof(color) * width);
    }

    Bitmap dest_img(width, height);
#pragma omp parallel for
    for (int i = 0; i < height; ++i) {
        for (int j = 0; j < width; ++j) {
            // auto src_pos = Produce_array(affine_inv,
            //                              {{static_cast<double>(j)},
            //                               {static_cast<double>(i)},
            //                               {1}});
            double dest_pos[3][1] = {{(double) j},
                                     {(double) i},
                                     {1.0}};
            auto src_pos_arr = produce_array(affine_inv_arr, dest_pos);
            int32_t src_x = min(max(0, (int32_t) src_pos_arr[0][0]), width - 1);
            int32_t src_y = min(max(0, (int32_t) src_pos_arr[1][0]), height - 1);
            dest_arr[i][j] = image_arr[src_y][src_x];
            //dest_img.set(j, i, image.get(src_x, src_y));

        }
    }
    for (int i = 0; i < height; ++i) {
        for (int j = 0; j < width; ++j) {
            Pixel pixel;
            pixel.r = dest_arr[i][j].r;
            pixel.g = dest_arr[i][j].g;
            pixel.b = dest_arr[i][j].b;
            dest_img.set(j, i, pixel);
        }
    }
    dest_img.save("dest.bmp");
}

inline double **produce_array(double **array_A, double array_B[3][1]) {
    double **array_C;
    array_C = (double **) malloc(sizeof(double *) * 3);
    for (int i = 0; i < 3; ++i) {
        array_C[i] = (double *) malloc(sizeof(double) * 1);
    }
    for (int i = 0; i < 3; ++i) {
        for (int j = 0; j < 1; ++j) {
            double inner_product = 0.0;
            for (int k = 0; k < 3; ++k) {
                inner_product += array_A[i][k] * array_B[k][j];
            }
            array_C[i][j] = inner_product;
        }
    }
    return array_C;
}