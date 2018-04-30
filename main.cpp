#include "tgaimage.h"

void line(int, int, int, int, TGAImage &, TGAColor);

const TGAColor white = TGAColor(255, 255, 255, 255);
const TGAColor red = TGAColor(255, 0, 0, 255);
const TGAColor green = TGAColor(0, 255, 0, 255);
const TGAColor blue = TGAColor(0, 0, 255, 255);

int main(int argc, char** argv) {
    TGAImage image(100, 100, TGAImage::RGB);

    line(25, 25, 95, 95, image, white);
    line(20, 13, 40, 80, image, red);
    line(40, 80, 20, 13, image, green); // Note that this green line will overwrite the last red line
    line(10, 10, 30, 80, image, blue);


    image.flip_vertically();  // Origin should be in the left bottom corner.
    image.write_tga_file("output.tga");
    return 0;
}

void line(int x0, int y0, int x1, int y1, TGAImage &image, TGAColor color) {

    // Make sure we compute the number of pixels correctly for lines which are higher than whide, i.e., steep.
    bool is_line_steep = std::abs(x0 - x1) < std::abs(y0 - y1);
    if(is_line_steep) { // Swap x and y coords for steep lines. We have to keep this in mind when drawing the points for the line later.
        std::swap(x0, y0);
        std::swap(x1, y1);
    }

    // Make sure the first point is left of the second one, as we assume that later on.
    if(x0 > x1) {
        std::swap(x0, x1);
        std::swap(y0, y1);
    }

    // Set the color for each pixel.
    int deltax = x1 - x0;
    int deltay = y1 - y0;
    int derror2 = std::abs(deltay) * 2; // We could use the error per pixel, but want to avoid floats.
                                        // And to avoid having to compare this value to deltax/2 (another float) later, we multiply it by 2
                                        // and can thus compare it to deltax then.
    int current_error2 = 0;
    int y = y0;
    for (int x = x0; x <= x1; x++) {
        if(is_line_steep) {
            image.set(y, x, color);   // If we transposed the coords, adapt the positions here accordingly.
        }
        else {
            image.set(x, y, color);
        }
        current_error2 += derror2;
        if (current_error2 > deltax) {
            y += (y1 > y0 ? 1 : -1);      // We know that the line is not steep because that is handled before, so we never need to change by more than 1.
            current_error2 -= deltax * 2;
        }
    }
}
