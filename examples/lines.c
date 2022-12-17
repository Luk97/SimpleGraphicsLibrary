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

    // orthogonal lines
    SGL_draw_line(&canvas, WIDTH/2, 0, WIDTH/2, HEIGHT, SGL_COLOR_YELLOW);
    SGL_draw_line(&canvas, 0, HEIGHT/2, WIDTH, HEIGHT/2, SGL_COLOR_YELLOW);

    // diagonal lines
    SGL_draw_line(&canvas, 0, 0, WIDTH, HEIGHT, SGL_COLOR_PINK);
    SGL_draw_line(&canvas, WIDTH, 0, 0, HEIGHT, SGL_COLOR_PINK);

    // other lines
    SGL_draw_line(&canvas, 0, 0, WIDTH/4, HEIGHT, SGL_COLOR_GREEN);
    SGL_draw_line(&canvas, WIDTH/4, 0, 0, HEIGHT, SGL_COLOR_GREEN);
    SGL_draw_line(&canvas, 0, HEIGHT/4, WIDTH, 0, SGL_COLOR_CYAN);
    SGL_draw_line(&canvas, 0, 0, WIDTH, HEIGHT/4, SGL_COLOR_CYAN);

    // same coordinates
    SGL_draw_line(&canvas, 10, 10, 10, 10, SGL_COLOR_RED);

    // overflow discard
    SGL_draw_line(&canvas, -10, HEIGHT/3, WIDTH + 10, HEIGHT/3, SGL_COLOR_RED);
    SGL_draw_line(&canvas, WIDTH/4*3, -10, WIDTH/4*2, HEIGHT + 20, SGL_COLOR_RED);

    int err = SGL_save_as_ppm(&canvas, "lines.ppm");
    return 0;   
}