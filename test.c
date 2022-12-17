#include "sgl.h"

#define WIDTH 800
#define HEIGHT 600

uint32_t pixels[WIDTH * HEIGHT];

int main()
{
    SGL_canvas canvas = {
        .width = WIDTH,
        .height = HEIGHT,
        .pixels = pixels
    };

    SGL_fill_canvas(&canvas, SGL_COLOR_GREY);
    int err = SGL_save_as_ppm(&canvas, "test.ppm");
    return 0;   
}