#include <iostream>
#include <vector>
#include "BitmapPlusPlus/include/BitmapPlusPlus.hpp"
#include "argh/argh.h"
#include <fstream>
#include <filesystem>

using namespace argh;
using namespace std;
using namespace bmp;
using namespace filesystem;

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
    Bitmap image;
    image.load(image_name);
}