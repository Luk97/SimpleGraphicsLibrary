#include "sgl.h"

uint32_t pixels[800 * 600];

int main()
{
    SGL_canvas canvas = {
        .width = 800,
        .height = 600,
        .pixels = pixels
    };

    SGL_fill_canvas(&canvas, SGL_COLOR_GREY);
    SGL_draw_line(&canvas, 0, 0, 200, 600, SGL_COLOR_PINK);


    int err = SGL_save_as_ppm(&canvas, "test.ppm");
    return 0;   
}