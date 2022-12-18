#include "sgl.h"

#define WIDTH 800
#define HEIGHT 600

uint32_t pixels[WIDTH * HEIGHT];

int main()
{
    SGL_Canvas canvas = {
        .width = WIDTH,
        .height = HEIGHT,
        .pixels = pixels
    };
    SGL_fill_canvas(&canvas, SGL_COLOR_GREY);

    SGL_Circle circle = {
        .cx = -20,
        .cy = HEIGHT / 2,
        .radius = 100
    };
    SGL_Rect rect = {
        .x = WIDTH / 4,
        .y = 100,
        .width = 200,
        .height = 200
    };

    SGL_draw_circle(&canvas, &circle, SGL_COLOR_PINK);
    SGL_draw_rect(&canvas, &rect, SGL_COLOR_WHITE);
    
    int err = SGL_save_as_ppm(&canvas, "test.ppm");
    return 0;   
}