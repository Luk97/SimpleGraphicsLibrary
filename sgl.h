#ifndef SGL_H_
#define SGL_H_

#include <stdio.h>
#include <stdint.h>

// --- Error handling ---
#include <errno.h>
#define Errno int
#define errno_return(value) do { result = value; goto defer; } while (0)

typedef struct {
    uint32_t* pixels;
    size_t width;
    size_t height;
} sgl_canvas;

typedef struct {
    size_t x;
    size_t y;
    size_t width;
    size_t height;
} sgl_rect;

typedef struct {
    size_t cx;
    size_t cy;
    size_t radius;
} sgl_circle;

/*
    Creates a ppm file with given file path. Overwrites that file, if it already exists.
    Image will be of size width * height and be filled in by pixel values.
    Returns 0 on success and set errno otherwise, if an error occured during file handling.
*/
Errno sgl_save_as_ppm(const sgl_canvas* canvas, const char* filepath);

/*
    Fills whole canvas with one color.
*/
void sgl_fill_canvas(sgl_canvas* canvas, uint32_t color);

/*
    Fills rect with given color. Position of rect is top left corner.
*/
void sgl_fill_rect(sgl_canvas* canvas, const sgl_rect* rect, uint32_t color);

/*
    Fills circle with given color. Position of circle is the center.
*/
void sgl_fill_circle(sgl_canvas* canvas, const sgl_circle* circle, uint32_t color);

#endif