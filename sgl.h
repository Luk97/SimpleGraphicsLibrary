#ifndef SGL_H_
#define SGL_H_

#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>

// --- SGL Headers ---
#include "coloring/color_defs.h"


// --- Helper Macros ---
#define SGL_SWAP(T, a, b) do { T t = a; a = b; b = t; } while (0)
#define SGL_ABS(a) ((a) < 0 ? -(a) : (a))
#define SGL_CLAMP(a, l, u) do { if (a < l) a = l; if (a > u) a = u; } while (0)

// --- Error handling ---
#include <errno.h>
#define Errno int
#define errno_return(val) do { result = val; goto defer; } while (0)

typedef struct {
    uint32_t* pixels;
    int width;
    int height;
} SGL_Canvas;

typedef struct {
    int x;
    int y;
    int width;
    int height;
} SGL_Rect;

typedef struct {
    int cx;
    int cy;
    int radius;
} SGL_Circle;

typedef struct {
    int x1;
    int y1;
    int x2;
    int y2;
    int x3;
    int y3;
} SGL_Triangle;

typedef struct {
    int x1;
    int y1;
    int x2;
    int y2;
} SGL_Line;

/*
    Creates a ppm file with given file path. Overwrites that file, if it already exists.
    Image will be of size width * height and be filled in by pixel values.
    Returns 0 on success and set errno otherwise, if an error occured during file handling.
*/
Errno SGL_save_as_ppm(const SGL_Canvas* canvas, const char* filepath);

// ---------------- CANVAS ----------------

/*
    Fills the whole canvas with one color of type uint32_t.
    If the sizes are invalid, the function returns.
*/

#define SGL_fill_canvas(first_arg, args...) \
    _Generic(first_arg, SGL_Canvas*: fill_canvas_structs, uint32_t*: fill_canvas) \
    (first_arg, args)

void fill_canvas_structs(const SGL_Canvas* canvas, uint32_t color);

void fill_canvas(uint32_t* pixels, int width, int height, uint32_t color);



// ---------------- RECTANGLE ----------------

/*
Draws a rectangle onto the image. The x and y coordiantes are those of the top left corner.
Color can be provided as an uint32_t.
If the rectangle would not be visible or has invalid values, the function returns.
*/

#define SGL_draw_rect(first_arg, args...) \
    _Generic(first_arg, SGL_Canvas*: draw_rect_structs, uint32_t*: draw_rect) \
    (first_arg, args)

void draw_rect_structs(const SGL_Canvas* canvas, const SGL_Rect* rect, uint32_t color);

void draw_rect(uint32_t* pixels, int canvas_width, int canvas_height,
                           int rect_x, int rect_y, int rect_width, int rect_height, uint32_t color);



// ---------------- CIRCLE ----------------

/*
    Draws a circle onto the image. The x and y coordinates are those of the center of the circle.
    Color can be provided as an uint32_t.
    If the circle would not be visible or has invalid values, the function returns.
*/

#define SGL_draw_circle(first_arg, args...) \
    _Generic(first_arg, SGL_Canvas*: draw_circle_structs, uint32_t*: draw_circle) \
    (first_arg, args)

void draw_circle_structs(const SGL_Canvas* canvas, const SGL_Circle* circle, uint32_t color);

void draw_circle(uint32_t* pixels, int canvas_width, int canvas_height,
                int cx, int cy, int radius, uint32_t color);



// ---------------- TRIANGLE ---------------

/*
    Draws a triangle onto the image. The x and y coordinates are those of the three corners of
    the triangle. Color can be provided as an uint32_t.
    If the triangle would not be visible or has invalid values, the function returns. 
*/

#define SGL_draw_triangle(first_arg, args...) \
    _Generic(first_arg, SGL_Canvas*: draw_triangle_structs, uint32_t*: draw_triangle) \
    (first_arg, args)

void draw_triangle(uint32_t* pixels, int canvas_width, int canvas_height,
                    int x1, int y1, int x2, int y2, int x3, int y3, uint32_t color);

void draw_triangle_structs(SGL_Canvas* canvas, SGL_Triangle* tri, uint32_t color);



// ---------------- LINE ---------------

/*
    Draws a line onto the image. The x and y coordinates are those of the two endpoints of
    the line. Color can be provided as an uint32_t.
    If the line would ne not visible or has invalid values, the function return.
*/

#define SGL_draw_line(first_arg, args...) \
    _Generic(first_arg, SGL_Canvas*: draw_line_structs, uint32_t*: draw_line) \
    (first_arg, args)

void draw_line(uint32_t* pixels, int width, int height,
                int x1, int y1, int x2, int y2, uint32_t color);

void draw_line_structs(SGL_Canvas* canvas, SGL_Line* line, uint32_t color);

#endif