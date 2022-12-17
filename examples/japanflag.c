#include "sgl.h"

uint32_t pixels[800 * 600];

int main()
{
    const char* filepath = "japanflag.ppm";


    SGL_canvas canvas = {
        .width = 800,
        .height = 600,
        .pixels = pixels
    };

    SGL_fill_canvas(&canvas, SGL_COLOR_WHITE);

    SGL_circle circle = {
        .cx = 400,
        .cy = 300,
        .radius = 100
    };

    SGL_fill_circle(&canvas, &circle, SGL_COLOR_RED);

    int err = SGL_save_as_ppm(&canvas, filepath);
    if (err != 0) return -1;
    
    return 0;
}