#include "sgl.h"


int sgl_create_ppm_file(sgl_canvas* canvas, const char* filepath, uint32_t color)
{
    FILE* f = fopen(filepath, "wb");
    if (f == NULL) return -1;

    fprintf(f, "P6 %d %d 255\n", canvas->width, canvas->height);

    for (size_t i = 0; i < canvas->width*canvas->height; ++i)
    {
        canvas->pixels[i] = color;
        uint8_t pixel_bytes[3] = {
            canvas->pixels[i] >> (8*0), // red
            canvas->pixels[i] >> (8*1), // green
            canvas->pixels[i] >> (8*2)  // blue
        };
        fwrite(pixel_bytes, sizeof(pixel_bytes), 1, f);
    }

    fclose(f);
    return 0;
}

int sgl_create_rect(sgl_canvas* canvas, int x, int y, int width, int height, uint32_t color)
{
    for (size_t i = 0; i < canvas->width*canvas->height; ++i)
    {
        
    }
    return 0;
}