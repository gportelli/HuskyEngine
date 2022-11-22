#include "includes/ascii_viewport.h"

#include "stdio.h"

typedef struct ascii_viewport_ {
  float aspect_ratio;
  uint cols, rows;
  float *framebuffer;
} ascii_viewport;

void ascii_viewport_clear(viewport_handle handle)
{
  ascii_viewport* vp = (ascii_viewport*)handle;

  const uint fb_size = sizeof(float) * vp->cols * vp->rows;
  memset(vp->framebuffer, 0, fb_size);
}

ascii_viewport_info ascii_viewport_get_info(viewport_handle handle)
{
  ascii_viewport* vp = (ascii_viewport*)handle;

  ascii_viewport_info info;

  info.width = vp->cols;
  info.height = vp->rows / vp->aspect_ratio;
  info.aspect_ratio = vp->aspect_ratio;

  return info;
}

viewport_handle ascii_viewport_init(uint cols, uint rows, float aspect)
{
  ascii_viewport* viewport = malloc(sizeof(ascii_viewport));

  viewport->cols = cols;
  viewport->rows = rows * aspect;
  viewport->aspect_ratio = aspect;

  const uint fb_size = sizeof(float) * cols * rows;
  viewport->framebuffer = malloc(fb_size);
  
  ascii_viewport_clear(viewport);

  return (viewport_handle)viewport;
}

void ascii_viewport_delete(viewport_handle handle)
{
  ascii_viewport* vp = (ascii_viewport*)handle;

  free(vp->framebuffer);
  free(vp);
}

void ascii_viewport_draw_pixel(
  viewport_handle handle, uint x, uint y, float color)
{
  ascii_viewport* vp = (ascii_viewport*)handle;

  vp->framebuffer[y * vp->cols + x] = color;
}

void ascii_viewport_render(viewport_handle handle)
{
  ascii_viewport* vp = (ascii_viewport*)handle;

  //terminal_clear_screen();
  terminal_move_cursor(1,1);

  uint index = 0;
  for(uint y=0; y<vp->rows; y++)
  {  
    for(uint x=0; x<vp->cols; x++)
    {
      terminal_set_background_color_gray_24(23 * vp->framebuffer[index]);
      printf(" ");

      index++;
    }

    printf("\n");
  }
}

void ascii_viewport_draw_rectangle(viewport_handle handle,
  uint x, uint y, uint w, uint h, float color)
{
  ascii_viewport* vp = (ascii_viewport*)handle;

  y *= vp->aspect_ratio;
  h *= vp->aspect_ratio;

  if(x >= vp->cols) return;
  if(y >= vp->rows) return;

  for(uint i=x; i<x+w && x<vp->cols; i++)
  {
    ascii_viewport_draw_pixel(vp, i, y, color);
  }

  for(uint i=y; i<y+h && y<vp->rows; i++)
  {
    ascii_viewport_draw_pixel(vp, x, i, color);
  }

  const uint x1 = x+w-1;
  if(x1 > x && x1 < vp->cols)
  {
    for(uint i=y; i<y+h; i++)
    {
      ascii_viewport_draw_pixel(vp, x1, i, color);
    }
  }

  const uint y1 = y+h-1;
  if(y1 > y && y1 < vp->rows)
  {
    for(uint i=x; i<x+w; i++)
    {
      ascii_viewport_draw_pixel(vp, i, y1, color);
    }
  }
}

//https://en.wikipedia.org/wiki/Bresenham%27s_line_algorithm#Algorithm_for_integer_arithmetic
/*
line from (x0, y0) to (x1, y1)

Let's write the line in this form: y = f(x)
1. y = mx + b
2. y = (y1 - y0) / (x1 - x0) * (x - x0) + y0
3. y = (y1 - y0) / (x1 - x0) * x - (y1 - y0) / (x1 - x0) * x0 + y0

Now let's change it to this form: 0 = f(x, y)
4. 0 = Ax + By + C

4.1 (x1 -x0)y = (y1 - y0)x - (y1 - y0)x0 + (x1 - x0) y0
4.2 0 = dy x - dx y - y1x0 + y0x0 + x1y0 -x0y0
4.3 0 = dy x - dx y + x1y0 - x0y1

5. A = dy
6. B = -dx
7. C = x1y0 - x0y1

Let's draw a line frome left to right with slope in [0, 1]
We start from (x0, y0), then move 1 pixel right and evaluate f(x0+1, y0+0.5)
If the result is positive, take y0, else take y0 + 1. Then repeat for the next pixel until x1

INTEGER ALGORITHM
Instead of computing f(x, y + 0.5) > 0 ? we can compute f(x, y + 0.5) = Error
Than increase this error by a distance, pixel by pixel. We'll see that, because we need only 
the sign of the error, we can at the end multiply everything by 2 gettin rid of the float.

f(x0, y0) is 0 by definition. So Error0 is 0
the first DeltaError is:
8. DeltaError1 = f(x0+1, y0 + 0.5) - f(x0, y0)
   = A(x0+1) + B(y0+0.5) + C - Ax0 - By0 - C = A + 0.5B = dy - 0.5dx

If we go up, the next delta will be:
9. DeltaError2 = f(x0+2, y0 + 1.5) - f(x0+1, y0+0.5)
   = A(x0+2) + B(y0+1.5) + C - A(x0+1) - B(y0+0.5) - C 
   = A + B = dy - dx

if we stay level, the next delta will be:
10. DeltaError2 = f(x0+2, y0+0.5) - f(x0+1, y0+0.5)
    = A(x0+2) + B(y0+0.5) + C - A(x0+1) - B(y+0.5) - C = A = dy

Now let's multiply our error by 2 to get rid of the 0.5. Note also that all the formulas for
errors and delta at this point have integere coefficient evaluated at integer coordinates,
this ensures our numbers will always be integers.

Here are finally our integer equations:

11. Error1 = DeltaError1 = 2dy - dx

12. if going up, Delta = dy - dx
13. if stay level, Delta = dy

Algorithm is:

plotLine(x0, y0, x1, y1)
    dx = x1 - x0
    dy = y1 - y0
    D = 2*dy - dx
    y = y0

    for x from x0 to x1
        plot(x,y)
        if D > 0
            y = y + 1
            D = D - 2*dx
        end if
        D = D + 2*dy
*/
void ascii_viewport_draw_line(viewport_handle handle, 
  float px0, float py0, float px1, float py1, float color)
{
  ascii_viewport* vp = (ascii_viewport*)handle;

  py0 *= vp->aspect_ratio;
  py1 *= vp->aspect_ratio;

  int x0 = roundf(px0);
  int y0 = roundf(py0);
  const int x1 = roundf(px1);
  const int y1 = roundf(py1);

  const int dx = abs(x1 - x0);
  const int sx = x0 < x1 ? 1 : -1;

  const int dy = -abs(y1 - y0);
  const int sy = y0 < y1 ? 1 : -1;

  int error = dx + dy;
    
  while(1)
  {
    ascii_viewport_draw_pixel(vp, x0, y0, color);
    
    if(x0 == x1 && y0 == y1) break;
    
    int e2 = 2 * error;
    if(e2 >= dy)
    {
      if(x0 == x1) break;

      error += dy;
      x0 += sx;
    }
    
    if(e2 <= dx)
    {
      if(y0 == y1) break;
      
      error += dx;
      y0 += sy;
    }
  }
}
