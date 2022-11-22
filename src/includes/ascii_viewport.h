#ifndef _ASCII_VIEWPORT_H_
#define _ASCII_VIEWPORT_H_

#include "stdlib.h"
#include "string.h"
#include "math.h"

#include "mytypes.h"
#include "terminal_functions.h"

typedef struct ascii_viewport_info_ {
  uint width, height;
  float aspect_ratio;
} ascii_viewport_info;

typedef void * viewport_handle;

void ascii_viewport_clear(viewport_handle handle);

ascii_viewport_info ascii_viewport_get_info(viewport_handle handle);

viewport_handle ascii_viewport_init(uint cols, uint rows, float aspect);

void ascii_viewport_delete(viewport_handle handle);

void ascii_viewport_draw_pixel(viewport_handle handle, uint x, uint y, float color);

void ascii_viewport_render(viewport_handle handle);

void ascii_viewport_draw_rectangle(viewport_handle handle,
  uint x, uint y, uint w, uint h, float color);

void ascii_viewport_draw_line(viewport_handle handle, 
  float px0, float py0, float px1, float py1, float color);

#endif