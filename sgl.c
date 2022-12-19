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

void fill_canvas(uint32_t* pixels, int width, int height, uint32_t color)
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

void draw_rect(uint32_t* pixels, int canvas_width, int canvas_height,
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

void draw_circle(uint32_t* pixels, int canvas_width, int canvas_height,
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



// ------------- TRIANGLE -------------

void order_points_by_y(SGL_Triangle* src, SGL_Triangle* dst)
{
    dst->x1 = src->x1;
    dst->y1 = src->y1;
    dst->x2 = src->x2;
    dst->y2 = src->y2;
    dst->x3 = src->x3;
    dst->y3 = src->y3;

    if (dst->y1 > dst->y2) {
        SGL_SWAP(int, dst->x1, dst->x2);
        SGL_SWAP(int, dst->y1, dst->y2);
    }

    if (dst->y2 > dst->y3) {
        SGL_SWAP(int, dst->x2, dst->x3);
        SGL_SWAP(int, dst->y2, dst->y3);
    }

    if (dst->y1 > dst->y2) {
        SGL_SWAP(int, dst->x1, dst->x2);
        SGL_SWAP(int, dst->y1, dst->y2);
    }
}

void draw_triangle(uint32_t* pixels, int canvas_width, int canvas_height,
                    int x1, int y1, int x2, int y2, int x3, int y3, uint32_t color)
{
    SGL_Canvas canvas = {
        .width = canvas_width,
        .height = canvas_height,
        .pixels = pixels
    };
    SGL_Triangle tri = {
        .x1 = x1,
        .y1 = y1,
        .x2 = x2,
        .y2 = y2,
        .x3 = x3,
        .y3 = y3
    };
    draw_triangle_structs(&canvas, &tri, color);
}

void draw_triangle_structs(SGL_Canvas* canvas, SGL_Triangle* param_tri, uint32_t color)
{
    if (!check_canvas_bounds(canvas)) return;

    SGL_Triangle tri;
    order_points_by_y(param_tri, &tri);

    int dx12 = tri.x2 - tri.x1;
    int dy12 = tri.y2 - tri.y1;
    int dx13 = tri.x3 - tri.x1;
    int dy13 = tri.y3 - tri.y1;

    for (int y = tri.y1; y <= tri.y2; ++y) {
        if (0 <= y && y < canvas->height) {
            int x_left = (dy12 != 0) ? (y - tri.y1) * dx12 / dy12 + tri.x1 : tri.x1;
            int x_right = (dy13 != 0) ? (y - tri.y1) * dx13 / dy13 + tri.x1 : tri.x1;
            if (x_left > x_right) SGL_SWAP(int, x_left, x_right);

            for (int x = x_left; x <= x_right; ++x) {
                if (0 <= x && x < canvas->width) {
                    canvas->pixels[y * canvas->width + x] = color;
                }
            }
        }
    }

    int dx32 = tri.x2 - tri.x3;
    int dy32 = tri.y2 - tri.y3;
    int dx31 = tri.x1 - tri.x3;
    int dy31 = tri.y1 - tri.y3;

    for (int y = tri.y2; y <= tri.y3; ++y) {
        if (0 <= y && y < canvas->height) {
            int x_left  = (dy32 != 0) ? (y - tri.y3) * dx32 / dy32 + tri.x3 : tri.x3;
            int x_right = (dy13 != 0) ? (y - tri.y3) * dx31 / dy31 + tri.x3 : tri.x3;
            if (x_left > x_right) SGL_SWAP(int, x_left, x_right);

            for (int x = x_left; x <= x_right; ++x) {
                if (0 <= x && x < canvas->width) {
                    canvas->pixels[y * canvas->width + x] = color;
                }
            }
        }
    }
}



// ------------- LINE -------------

bool check_line_bounds(SGL_Canvas* canvas, SGL_Line* line)
{
    if (line->x1 < 0 && line->x2 < 0) return false;
    if (line->x1 >= canvas->width && line->x2 >= canvas->width) return false;
    if (line->y1 < 0 && line->y2 < 0) return false;
    if (line->y1 >= canvas->height && line->y2 >= canvas->height) return false;

    return true;
}

void draw_line_structs(SGL_Canvas* canvas, SGL_Line* line, uint32_t color)
{
    if (!check_canvas_bounds(canvas)) return;
    if (!check_line_bounds(canvas, line)) return;

    int x1 = line->x1, y2 = line->y1;
    int x2 = line->x2 ,y2 = line->y2;

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

void draw_line_(uint32_t* pixels, int width, int height,
                int x1, int y1, int x2, int y2, uint32_t color)
{
    SGL_Canvas canvas = {
        .width = width,
        .height = height,
        .pixels = pixels
    };
    SGL_Line line = {
        .x1 = x1,
        .y1 = y1,
        .x2 = x2,
        .y2 = y2
    };
    draw_line_structs(&canvas, &line, color);
}
