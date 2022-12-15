#ifndef SGL_H_
#define SGL_H_

#include <stdio.h>
#include <stdint.h>

typedef struct {
    uint32_t* pixels;
    int width;
    int height;
} sgl_canvas;

typedef struct {
    int x;
    int y;
    int widht;
    int height;
} sgl_rect;

/*
    Create a ppm-file with given filepath if it doesn't already exist.
    The image will have given width and height and be filled with one color.
*/
int sgl_create_ppm_file(sgl_canvas* canvas, const char* filepath, uint32_t color);

int sgl_create_rect(sgl_canvas* canvas, int x, int y, int width, int height, uint32_t color);

#endif