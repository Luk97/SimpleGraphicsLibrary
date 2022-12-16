
#include "sgl.h"

#define WINDOW_WIDTH 800
#define WINDOW_HEIGHT 600

int main(int argc, char** argv)
{
    char* filepath = "./test.ppm";
    uint32_t pixels[WINDOW_WIDTH*WINDOW_HEIGHT];
    SGL_canvas canvas = {
        .pixels = pixels,
        .width = WINDOW_WIDTH,
        .height = WINDOW_HEIGHT
    };

    SGL_rect rect = {
        .x = 0,
        .y = 0,
        .width = 100,
        .height = 100
    };

    SGL_circle circle = {
        .cx = WINDOW_WIDTH / 2,
        .cy = WINDOW_HEIGHT / 2,
        .radius = 50
    };
    
    SGL_fill_canvas(&canvas, SGL_COLOR_YELLOW);

    /*for (int i = 0; i < 7; ++i) {
        for (int j = 0; j < 9; ++j) {
            circle.cy = 100 * i - 50;
            circle.cx = 100 * j - 50;
            uint32_t color = (i + j & 1) ? BLUE : RED;
            SGL_fill_circle(&canvas, &circle, color);
        }
    }*/

    int err = SGL_save_as_ppm(&canvas, filepath);
    return 0;
}    