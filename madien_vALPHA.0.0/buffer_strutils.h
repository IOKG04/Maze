/************************************************\
|   Functions for putting strings into buffers   |
|                                                |
| Copyright (c) 2024, https://github.com/IOKG04  |
| Licensed under MIT-Festival-Light, available   |
| at https://github.com/IOKG04/Licenses/blob/master/MIT-FL/MIT-Festival-Light |
\************************************************/

#ifndef MDE_B_STRUTILS_H__
#define MDE_B_STRUTILS_H__

#include <stddef.h>
#include "buffer.h"
#include "config.h"

// replaces all screen_element_ts, within strlim, including those after a '\0'
#define MDE_SREPLACE    0b00000001

// recommended default flags for printing a string to a buffer
#define MDE_SDEFAULT  (0)

// prints str to buf, such that the first character of str is at {offs_x, offs_y} in buf space, abiding the length limit and flags provided, with all parts besides the character being the same as e_template
void eb_str_put(ebuffer_t *restrict buf, char *restrict str, int offs_x, int offs_y, size_t strlim, mde_flags_t flags, screen_element_t e_template);

#endif
