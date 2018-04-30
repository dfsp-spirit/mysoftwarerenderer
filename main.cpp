#include "tgaimage.h"

void line(int, int, int, int, TGAImage &, TGAColor);

const TGAColor white = TGAColor(255, 255, 255, 255);
const TGAColor red = TGAColor(255, 0, 0, 255);
const TGAColor green = TGAColor(0, 255, 0, 255);
const TGAColor blue = TGAColor(0, 0, 255, 255);

int main(int argc, char** argv) {
    TGAImage image(100, 100, TGAImage::RGB);

    line(13, 20, 80, 40, image, white);
    line(20, 13, 40, 80, image, red);
    line(80, 40, 13, 20, image, green);


    image.flip_vertically();  // Origin should be in the left bottom corner.
    image.write_tga_file("output.tga");
    return 0;
}

void line(int x0, int y0, int x1, int y1, TGAImage &image, TGAColor color) {

    // Make sure we compute the number of pixels correctly for lines which are higher than whide, i.e., steep.
    bool isLineSteep = std::abs(x0 - x1) < std::abs(y0 - y1);
    if(isLineSteep) { // Swap x and y coords for steep lines. We have to keep this in mind when drawing the points for the line later.
        std::swap(x0, y0);
        std::swap(x1, y1);
    }

    // Make sure the first point is left of the second one, as we assume that later on.
    if(x0>x1) {
        std::swap(x0, x1);
        std::swap(y0, y1);
    }

    // Set the color for each pixel.
    for (int x = x0; x <= x1; x++) {
        float t = (x - x0) / (float) (x1 - x0);
        int y = y0 * (1. - t) + y1 * t;
        if(isLineSteep) {
            image.set(y, x, color);   // If we transposed the coords, adapt the positions here accordingly.
        }
        else {
            image.set(x, y, color);
        }
    }
}
