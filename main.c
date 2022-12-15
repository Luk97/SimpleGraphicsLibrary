
#include "sgl.h"

#define WINDOW_WIDTH 800
#define WINDOW_HEIGHT 600

#define RED 0x000000FF
#define GREEN 0x0000FF00
#define BLUE 0x00FF0000
#define BACKGROUND_COLOR 0x00202020


int main(int argc, char** argv)
{
    char* filepath = "./test.ppm";
    uint32_t pixels[WINDOW_WIDTH*WINDOW_HEIGHT];
    sgl_canvas canvas = {
        .pixels = pixels,
        .width = WINDOW_WIDTH,
        .height = WINDOW_HEIGHT
    };

    sgl_rect rect = {
        .x = 0,
        .y = 0,
        .width = 100,
        .height = 100
    };

    sgl_circle circle = {
        .cx = WINDOW_WIDTH / 2,
        .cy = WINDOW_HEIGHT / 2,
        .radius = 50
    };
    
    sgl_fill_canvas(&canvas, BACKGROUND_COLOR);

    for (int i = 0; i < 7; ++i) {
        for (int j = 0; j < 9; ++j) {
            circle.cy = 100 * i - 50;
            circle.cx = 100 * j - 50;
            uint32_t color = (i + j & 1) ? BLUE : RED;
            sgl_fill_circle(&canvas, &circle, color);
        }
    }

    int err = sgl_save_as_ppm(&canvas, filepath);
    return 0;
}    