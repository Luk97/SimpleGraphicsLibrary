#ifndef SGL_H_
#define SGL_H_

#include <stdio.h>
#include <stdint.h>

// --- SGL Headers ---
#include "coloring/color_defs.h"

// --- Macros ---
#define SGL_SWAP(T, a, b) do { T t = a; a = b; b = t; } while (0)
#define SGL_ABS(a) ((a) < 0 ? -(a) : (a))

// --- Error handling ---
#include <errno.h>
#define Errno int
#define errno_return(val) do { result = val; goto defer; } while (0)

typedef struct {
    uint32_t* pixels;
    size_t width;
    size_t height;
} SGL_canvas;

typedef struct {
    size_t x;
    size_t y;
    size_t width;
    size_t height;
} SGL_rect;

typedef struct {
    size_t cx;
    size_t cy;
    size_t radius;
} SGL_circle;

/*
    Creates a ppm file with given file path. Overwrites that file, if it already exists.
    Image will be of size width * height and be filled in by pixel values.
    Returns 0 on success and set errno otherwise, if an error occured during file handling.
*/
Errno SGL_save_as_ppm(const SGL_canvas* canvas, const char* filepath);

/*
    Fills whole canvas with one color.
*/
void SGL_fill_canvas(SGL_canvas* canvas, uint32_t color);

/*
    Fills rect with given color. Position of rect is top left corner.
*/
void SGL_fill_rect(SGL_canvas* canvas, const SGL_rect* rect, uint32_t color);

/*
    Fills circle with given color. Position of circle is the center.
*/
void SGL_fill_circle(SGL_canvas* canvas, const SGL_circle* circle, uint32_t color);

void SGL_draw_line(SGL_canvas* canvas, int x1, int y1, int x2, int y2, uint32_t color);

#endif