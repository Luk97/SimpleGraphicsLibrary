#include "sgl.h"

Errno sgl_save_as_ppm(const sgl_canvas* canvas, const char* filepath)
{
    FILE* f = NULL;
    Errno result = 0;

    f = fopen(filepath, "wb");
    if (f == NULL) errno_return(errno);

    fprintf(f, "P6 %zu %zu 255\n", canvas->width, canvas->height);

    for (size_t i = 0; i < canvas->width * canvas->height; ++i)
    {
        uint8_t pixel_bytes[3] = {
            canvas->pixels[i] >> (8*0), // red
            canvas->pixels[i] >> (8*1), // green
            canvas->pixels[i] >> (8*2)  // blue
        };
        fwrite(pixel_bytes, sizeof(pixel_bytes), 1, f);
        if (ferror(f)) errno_return(errno);
    }

    errno_return(0);

defer:
    if (f) fclose(f);
    return result;
}

void sgl_fill_canvas(sgl_canvas* canvas, uint32_t color)
{
    for (size_t y = 0; y < canvas->height; ++y) {
        for (size_t x = 0; x < canvas->width; ++x) {
            canvas->pixels[y * canvas->width + x] = color;
        }
    }
}

void sgl_fill_rect(sgl_canvas* canvas, const sgl_rect* rect, uint32_t color)
{
    for (size_t y = rect->y; y < rect->y + rect->height; ++y) {
        for (size_t x = rect->x; x < rect->x + rect->width; ++x) {
            canvas->pixels[y * canvas->width + x] = color;
        }
    }
}

void sgl_fill_circle(sgl_canvas* canvas, const sgl_circle* c, uint32_t color)
{
    int x1 = c->cx - c->radius;
    int y1 = c->cy - c->radius;
    int x2 = c->cx + c->radius;
    int y2 = c->cy + c->radius;

    for (size_t y = y1; y < y2; ++y) {
        if (0 <= y && y < canvas->height) {
            for (size_t x = x1; x < x2; ++x) {
                if (0 <= x && x < canvas->width) {
                    int dx = x - c->cx;
                    int dy = y - c->cy;
                    if ((dx * dx) + (dy * dy) <= c->radius * c->radius) {
                        canvas->pixels[y * canvas->width + x] = color;
                    }
                }
            }
        }
    }
}