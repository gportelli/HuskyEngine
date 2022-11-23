#include <math.h>
#include <unistd.h>
#include <stdio.h>
#include <time.h>

#include "includes/viewport.h"
#include "includes/terminal_functions.h"

void test()
{
  terminal_clear_screen();

  printf("\e[1;31mHello \e[2;37;41mWorld\n");
  
  terminal_reset_all_style_and_color();

  for(int i=0; i<24; i++)
  {
    terminal_set_background_color_gray_24(i);
    printf(" \n");
  }
}

void test_viewport(viewport_handle vp)
{
  viewport_info info = viewport_get_info(vp);

  const float a = info.aspect_ratio;
  const uint w = info.width, h = info.height;
  const uint max_color = w + h;
  for(int y=0; y<h; y++)
  {
    for(int x=0; x<w; x++)
    {
      viewport_draw_pixel(vp, x, y, (x + y/a) / max_color);
    }
  }
}

void test_clock(viewport_handle vp)
{
  viewport_info info = viewport_get_info(vp);

  const uint cx = info.width / 2;
  const uint cy = info.height / 2;
  const uint radius = (cx < cy ? cx : cy) - 1;
  const uint big_len = radius * 0.8;
  const uint small_len = radius * 0.5;

  const float pi = 3.1415926;

  for(;;)
  {
    time_t t = time(NULL);   
    struct tm *tm = localtime(&t);

    const float hour_rad = tm->tm_hour / 6.0 * pi;
    const float min_rad = tm->tm_min / 30.0 * pi;
    const float sec_rad = tm->tm_sec / 30.0 * pi;

    viewport_clear(vp);
    viewport_draw_ellipse(vp, cx, cy, radius, radius, 1);
    
    viewport_draw_line(vp, cx, cy, cx + sin(hour_rad) * small_len, cy - cos(hour_rad) * small_len, 1);
    viewport_draw_line(vp, cx, cy, cx + sin(min_rad) * big_len, cy - cos(min_rad) * big_len, 1);
    viewport_draw_line(vp, cx, cy, cx + sin(sec_rad) * big_len, cy - cos(sec_rad) * big_len, 1);
    
    viewport_render(vp);

    usleep(33000);
  }
}

int main()
{
  uint l=40;
  float a=0.5;

  viewport_handle myvp = viewport_init(l, l, a);

  terminal_clear_screen();
  terminal_hide_cursor();

  //test_viewport(myvp); 

  //viewport_draw_rectangle(myvp, 3, 3, 8, 8, 1);  
  test_clock(myvp);
  //viewport_draw_line(myvp, 19, 19, 39, 23, 1);
  //viewport_draw_ellipse(myvp, 19, 19, 10, 5, 1);

  viewport_render(myvp);
    
  viewport_delete(myvp);
  myvp = NULL;

  terminal_show_cursor();

  return 0;
}
