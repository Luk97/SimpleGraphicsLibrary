#include "../sgl.h"

#define WINDOW_WIDTH 800
#define WINDOW_HEIGHT 600

uint32_t pixels[WINDOW_WIDTH*WINDOW_HEIGHT];



int main(int argc, char** argv)
{
    const char* filepath = "./test.ppm";

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

    for (int i = 0; i < 6; ++i) {
        for (int j = 0; j < 8; ++j) {
            rect.y = 100 * i;
            rect.x = 100 * j;
            uint32_t color = (i + j & 1) ? SGL_COLOR_GREY : SGL_COLOR_WHITE;
            SGL_fill_rect(&canvas, &rect, color);
        }
    }

    int err = SGL_save_as_ppm(&canvas, filepath);
    if (err == 0) return -1;
    return 0;
}    