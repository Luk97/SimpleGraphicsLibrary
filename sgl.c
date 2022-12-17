#include "sgl.h"

Errno SGL_save_as_ppm(const SGL_canvas* canvas, const char* filepath)
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

void SGL_fill_canvas(SGL_canvas* canvas, uint32_t color)
{
    for (size_t y = 0; y < canvas->height; ++y) {
        for (size_t x = 0; x < canvas->width; ++x) {
            canvas->pixels[y * canvas->width + x] = color;
        }
    }
}

void SGL_fill_rect(SGL_canvas* canvas, const SGL_rect* rect, uint32_t color)
{
    for (size_t y = rect->y; y < rect->y + rect->height; ++y) {
        for (size_t x = rect->x; x < rect->x + rect->width; ++x) {
            canvas->pixels[y * canvas->width + x] = color;
        }
    }
}

void SGL_fill_circle(SGL_canvas* canvas, const SGL_circle* c, uint32_t color)
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

void SGL_draw_line(SGL_canvas* canvas, int x1, int y1, int x2, int y2, uint32_t color)
{
    int dx = x2 - x1;
    int dy = y2 - y1;

    // same coordinates
    if (dx == 0 && dy == 0)
    {
        canvas->pixels[y1 * canvas->width + x1] = color;
    }
    // vertical line
    else if (dx == 0)
    {    
        // return if line would be invisible
        if (x1 < 0 || x1 > canvas->width) return;

        // clamp
        if (y1 > y2) SGL_SWAP(int, y1, y2);
        if (y1 < 0) y1 = 0;
        if (y2 > canvas->height) y2 = canvas->height;

        for (size_t y = y1;  y < y2; ++y) {
            canvas->pixels[y * canvas->width + x1] = color;
        }
    }
    // all other lines
    else
    {
        int b = y1 - dy * x1 / dx;
        if (x1 > x2) SGL_SWAP(int, x1, x2);

        for (int x = x1; x <= x2; ++x) {
            if (0 <= x && x < canvas->width) {
                int cur_y = dy * x / dx + b;
                int nxt_y = dy * (x+1) / dx + b;
                if (cur_y > nxt_y) SGL_SWAP(int, cur_y, nxt_y);

                for (int y = cur_y; y <= nxt_y; ++y) {
                    if (0 <= y && y < canvas->height) {
                        canvas->pixels[y * canvas->width + x] = color;
                    } 
                }
            }
        }
    }
}
