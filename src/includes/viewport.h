#ifndef _VIEWPORT_H_
#define _VIEWPORT_H_

#include "stdlib.h"
#include "string.h"
#include "math.h"

#include "mytypes.h"
#include "terminal_functions.h"

typedef struct viewport_info_ {
  uint width, height;
  float aspect_ratio;
} viewport_info;

typedef void * viewport_handle;

void viewport_clear(viewport_handle handle);

viewport_info viewport_get_info(viewport_handle handle);

viewport_handle viewport_init(uint cols, uint rows, float aspect);

void viewport_delete(viewport_handle handle);

void viewport_draw_pixel(viewport_handle handle, uint x, uint y, float color);

void viewport_render(viewport_handle handle);

void viewport_draw_rectangle(viewport_handle handle,
  uint x, uint y, uint w, uint h, float color);

void viewport_draw_line(viewport_handle handle, 
  float px0, float py0, float px1, float py1, float color);

void viewport_draw_ellipse(viewport_handle handle, float x, float y, float a, float b, float color);

#endif