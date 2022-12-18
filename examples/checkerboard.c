#include "../sgl.h"

#define WINDOW_WIDTH 800
#define WINDOW_HEIGHT 600

#define COLS (WINDOW_WIDTH / 100)
#define ROWS (WINDOW_HEIGHT / 100)

uint32_t pixels[WINDOW_WIDTH * WINDOW_HEIGHT];

int main(int argc, char** argv)
{
    // create filepath for ppm file
    const char* filepath = "./checkerboard.ppm";

    // canvas which can be drawn on
    SGL_Canvas canvas = {
        .pixels = pixels,
        .width = WINDOW_WIDTH,
        .height = WINDOW_HEIGHT
    };

    // a rect containing x and y position of top left corner and
    // its width and height
    SGL_Rect rect = {
        .x = 0,
        .y = 0,
        .width = 100,
        .height = 100
    };

    // iterate through the canvas
    for (int i = 0; i < ROWS; ++i) {
        for (int j = 0; j < COLS; ++j) {
            rect.y = 100 * i;
            rect.x = 100 * j;

            // choose color depending on position on board
            uint32_t color = (i + j & 1) ? SGL_COLOR_GREY : SGL_COLOR_RED;

            // draw a rect onto the canvas with a given color
            SGL_draw_rect(&canvas, &rect, color);
        }
    }

    // save the canvas in a ppm file
    int err = SGL_save_as_ppm(&canvas, filepath);
    if (err == 0) return -1;
    return 0;
}    