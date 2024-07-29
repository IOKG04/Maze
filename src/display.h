#ifndef DISPLAY_H__
#define DISPLAY_H__

#include <stddef.h>

void setup_screen(size_t width, size_t height);
void revert_screen();

void set_pixel(size_t x, size_t y, char c);

void draw_screen();

#endif
