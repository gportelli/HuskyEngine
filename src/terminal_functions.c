#include "includes/terminal_functions.h"

#include "stdio.h"

// ANSI escape codes
// https://gist.github.com/fnky/458719343aabd01cfb17a3a4f7296797


void terminal_move_cursor(uint line, uint column)
{
  // Note that line and columns start from 1, not 0

  // ESC[{line};{column}H  move cursor to line column
  printf("\e[%d;%dH", line, column);
}

void terminal_set_bold()
{
  printf("\e[1m");
}

void terminal_reset_bold()
{
  printf("\e[22m");
}

void terminal_set_italic()
{
  printf("\e[3m");
}

void terminal_reset_italic()
{
  printf("\e[23m");
}

void terminal_set_underline()
{
  printf("\e[4m");
}

void terminal_reset_underline()
{
  printf("\e[24m");
}

// not working on mac os x terminal
void terminal_set_foreground_color_rgb(uint r, uint g, uint b)
{
  printf("\e[38;2;%d;%d;%dm", r, g, b);
}

// not working on mac os x terminal
void terminal_set_background_color_rgb(uint r, uint g, uint b)
{
  printf("\e[48;2;%d;%d;%dm", r, g, b);
}

void terminal_set_foreground_color_256(uint color_index)
{
  printf("\e[38;5;%dm", color_index);
}

void terminal_set_background_color_256(uint color_index)
{
  printf("\e[48;5;%dm", color_index);
}

void terminal_set_foreground_color_gray_24(uint color_index)
{
  printf("\e[38;5;%dm", 232 + color_index);
}

void terminal_set_background_color_gray_24(uint color_index)
{
  printf("\e[48;5;%dm", 232 + color_index);
}

void terminal_reset_all_style_and_color()
{
  printf("\e[0m");
}

void terminal_hide_cursor()
{
  printf("\e[?25l");
}

void terminal_show_cursor()
{
  printf("\e[?25h");
}

void terminal_clear_screen()
{
  terminal_move_cursor(1, 1);
  terminal_reset_all_style_and_color();

  // ESC[2J erase entire screen
  printf("\e[2J");
}