#include "sgl.h"

Errno SGL_save_as_ppm(const SGL_Canvas* canvas, const char* filepath)
{
    FILE* f = NULL;
    Errno result = 0;

    f = fopen(filepath, "wb");
    if (f == NULL) errno_return(errno);

    fprintf(f, "P6 %d %d 255\n", canvas->width, canvas->height);

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



// ------------- CANVAS -------------

bool check_canvas_bounds(const SGL_Canvas* canvas)
{
    return canvas->width > 0 && canvas->height > 0;
}

void fill_canvas_structs(const SGL_Canvas* canvas, uint32_t color)
{
    if (!check_canvas_bounds(canvas)) return;

    for (size_t y = 0; y < canvas->height; ++y) {
        for (size_t x = 0; x < canvas->width; ++x) {
            canvas->pixels[y * canvas->width + x] = color;
        }
    }
}

void fill_canvas_singles(uint32_t* pixels, int width, int height, uint32_t color)
{
    SGL_Canvas canvas = {
        .width = width,
        .height = height,
        .pixels = pixels
    };
    fill_canvas_structs(&canvas, color);
}



// ------------- RECTANGLE -------------

bool adjust_rect_bounds(const SGL_Canvas* canvas, const SGL_Rect* src, SGL_Rect* dst)
{
    // not visible
    if (src->width <= 0 || src->height <= 0) return false;

    // convert to two points representation
    int x1 = src->x;
    int y1 = src->y;
    int x2 = src->x + src->width;
    int y2 = src->y + src->height;

    // out of bounds
    if (x1 >= canvas->width || y1 >= canvas->height) return false;
    if (x2 < 0 || y2 < 0) return false;

    // clamp values
    SGL_CLAMP(x1, 0 , canvas->width);
    SGL_CLAMP(y1, 0 , canvas->height);
    SGL_CLAMP(x2, 0 , canvas->width);
    SGL_CLAMP(y2, 0 , canvas->height);

    // assign values
    dst->x = x1;
    dst->y = y1;
    dst->width = x2 - x1;
    dst->height = y2 -y1;

    return true;
}


void draw_rect_structs(const SGL_Canvas* canvas, const SGL_Rect* param_rect, uint32_t color)
{
    SGL_Rect rect;
    if (!check_canvas_bounds(canvas)) return;
    if (!adjust_rect_bounds(canvas, param_rect, &rect)) return;

    for (int y = rect.y; y < rect.y + rect.height; ++y) {
        for (int x = rect.x; x < rect.x + rect.width; ++x) {
            canvas->pixels[y * canvas->width + x] = color;
        }
    }
}

void draw_rect_singles(uint32_t* pixels, int canvas_width, int canvas_height,
                           int rect_x, int rect_y, int rect_width, int rect_height, uint32_t color)
{
    SGL_Canvas canvas = {
        .width = canvas_width,
        .height = canvas_height,
        .pixels = pixels
    };
    SGL_Rect rect = {
        .x = rect_x,
        .y = rect_y,
        .width = rect_width,
        .height = rect_height
    };
    draw_rect_structs(&canvas, &rect, color);
}



// ------------- CIRCLE -------------

bool check_circle_bounds(const SGL_Canvas* canvas, const SGL_Circle* c)
{
    // not visible
    if (c->radius <= 0) return false;

    // out of bounds
    if (c->cx - c->radius >= canvas->width || c->cx + c->radius < 0) return false;
    if (c->cy - c->radius >= canvas->height || c->cy + c->radius < 0) return false;

    return true;
}

void draw_circle_singles(uint32_t* pixels, int canvas_width, int canvas_height,
                             int cx, int cy, int radius, uint32_t color)
{
    SGL_Canvas canvas = {
        .width = canvas_width,
        .height = canvas_height,
        .pixels = pixels
    };
    const SGL_Circle circle = {
        .cx = cx,
        .cy = cy,
        .radius = radius
    };
    draw_circle_structs(&canvas, &circle, color);
}

void draw_circle_structs(const SGL_Canvas* canvas, const SGL_Circle* c, uint32_t color)
{
    if (!check_canvas_bounds(canvas)) return;
    if (!check_circle_bounds(canvas, c)) return;

    int x1 = c->cx - c->radius;
    int y1 = c->cy - c->radius;
    int x2 = c->cx + c->radius;
    int y2 = c->cy + c->radius;

    for (int y = y1; y <= y2; ++y) {
        if (0 <= y && y < canvas->height) {
            for (int x = x1; x <= x2; ++x) {
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





void SGL_draw_line(SGL_Canvas* canvas, int x1, int y1, int x2, int y2, uint32_t color)
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
