#ifndef DISPLAY_H__
#define DISPLAY_H__

#include <stddef.h>
#include "config.h"

#define SE_SPACE ((screen_element_t){' '})

// element in a screen buffer
typedef struct{
    char  c;
} screen_element_t;

// initializes buffer and terminal
void setup_screen(size_t width, size_t height);
// undos setup_screen()
void revert_screen();

// sets element at a position to e
void set_element(size_t x, size_t y, screen_element_t e);
// sets buffer to e
void clear_screen(screen_element_t e);

// prints buffer content to stdout
void draw_screen();

#endif
