
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
   
    int result = sgl_create_ppm_file(&canvas, filepath, BACKGROUND_COLOR);   
    return 0;
}