#ifndef DISPLAY_H__
#define DISPLAY_H__

#include <stddef.h>
#include "config.h"

#define SE_SPACE ((screen_element_t){' '})

typedef struct{
    char  c;
} screen_element_t;

void setup_screen(size_t width, size_t height);
void revert_screen();

void set_element(size_t x, size_t y, screen_element_t e);
void clear_screen(screen_element_t e);

void draw_screen();

#endif
