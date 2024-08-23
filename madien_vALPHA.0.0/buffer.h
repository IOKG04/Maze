/************************************************\
|       Buffers types and related functions      |
|                                                |
| Copyright (c) 2024, https://github.com/IOKG04  |
| Licensed under MIT-Festival-Light, available   |
| at https://github.com/IOKG04/Licenses/blob/master/MIT-FL/MIT-Festival-Light |
\************************************************/

#ifndef MDE_BUFFER_H__
#define MDE_BUFFER_H__

#include <stddef.h>
#include "config.h"

#if MDE_TRUE_COLOR
    #include <stdint.h>
#endif

// screen_element_t with empty space
#if MDE_TRUE_COLOR
    #define SE_SPACE ((screen_element_t){' ', 255, 255, 255})
#else
    #define SE_SPACE ((screen_element_t){' '})
#endif
// screen_element_t containing no data
#if MDE_TRUE_COLOR
    #define SE_NULL  ((screen_element_t){'\0', 0, 0, 0})
#else
    #define SE_NULL  ((screen_element_t){'\0'})
#endif

// returns 1 if e is null equivalent
#define SE_IS_NULL(e) (e.c == '\0')

// element in an element buffer
typedef struct{
    char  c;
    #if MDE_TRUE_COLOR
        uint8_t r,
                g,
                b;
    #endif
} screen_element_t;

// buffer containing screen_element_t
typedef struct{
    screen_element_t *data;
    size_t            width,
                      height;
} ebuffer_t;

// draws any data, including SE_NULL, to dest
#define MDE_BDRAW_OVER 0b00000001

// recommended default flags for _b_draw() and _b_print()
#define MDE_BDEFAULT (0)

// initializes buf to size {width, height}
int eb_init(ebuffer_t *buf, size_t width, size_t height);
// deinitialized members of buf
void eb_free(ebuffer_t *buf);

// sets element in buf at {x, y} to e
int eb_set(ebuffer_t *buf, size_t x, size_t y, screen_element_t e);
// sets all elements in buf to e
void eb_clear(ebuffer_t *buf, screen_element_t e);
// draws src onto dest, such that {0, 0} in src space is {offs_x, offs_y} is dest space
void eb_draw(ebuffer_t *dest, ebuffer_t src, int offs_x, int offs_y, mde_flags_t flags);

// returns element at {x, y} in buf
screen_element_t eb_get(ebuffer_t buf, size_t x, size_t y);

// prints buf to terminal, such that {0, 0} in buf space is at {offs_x, offs_y} in terminal space (zero based)
void eb_print(ebuffer_t buf, int offs_x, int offs_y);

#endif
