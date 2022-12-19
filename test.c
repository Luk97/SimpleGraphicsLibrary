#include "sgl.h"

#define WIDTH 800
#define HEIGHT 600

uint32_t pixels[WIDTH * HEIGHT];

int main()
{

    int x1 = WIDTH/2, y1 = HEIGHT/8;
    int x2 = WIDTH/6, y2 = HEIGHT/2;
    int x3 = WIDTH/8*7, y3 = HEIGHT/8*7;

    int radius = 10;

    SGL_Canvas canvas = {
        .width = WIDTH,
        .height = HEIGHT,
        .pixels = pixels
    };
    SGL_fill_canvas(&canvas, SGL_COLOR_GREY);

    SGL_Triangle tri = {
        .x1 = x1,
        .y1 = y1,
        .x2 = x2,
        .y2 = y2,
        .x3 = x3,
        .y3 = y3
    };

    SGL_draw_triangle(&canvas, &tri, SGL_COLOR_CYAN);
    SGL_draw_triangle(pixels, WIDTH, HEIGHT, WIDTH/5, HEIGHT/7, WIDTH/5, HEIGHT/8*7, WIDTH/4*3, HEIGHT/2, SGL_COLOR_GREEN);
    SGL_draw_triangle(pixels, WIDTH, HEIGHT, 10, 10, 100, 10, 50, 50, SGL_COLOR_PINK);
    
    int err = SGL_save_as_ppm(&canvas, "test.ppm");
    return 0;   
}