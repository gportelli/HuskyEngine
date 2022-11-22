#ifndef _TERMINAL_FUNCTIONS_H_
#define _TERMINAL_FUNCTIONS_H_

#include "mytypes.h"

void terminal_move_cursor(uint line, uint column);

void terminal_set_bold();
void terminal_reset_bold();

void terminal_set_italic();
void terminal_reset_italic();

void terminal_set_underline();
void terminal_reset_underline();

// not working on mac os x terminal
void terminal_set_foreground_color_rgb(uint r, uint g, uint b);
void terminal_set_background_color_rgb(uint r, uint g, uint b);

void terminal_set_foreground_color_256(uint color_index);
void terminal_set_background_color_256(uint color_index);

void terminal_set_foreground_color_gray_24(uint color_index);
void terminal_set_background_color_gray_24(uint color_index);

void terminal_reset_all_style_and_color();

void terminal_hide_cursor();
void terminal_show_cursor();

void terminal_clear_screen();

#endif