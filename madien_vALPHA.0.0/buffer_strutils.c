/************************************************\
|      Implementations for buffer_strutils.h     |
|                                                |
| Copyright (c) 2024, https://github.com/IOKG04  |
| Licensed under MIT-Festival-Light, available   |
| at https://github.com/IOKG04/Licenses/blob/master/MIT-FL/MIT-Festival-Light |
\************************************************/

#include "buffer_strutils.h"

#include <string.h>
#include "buffer.h"
#include "config.h"

// prints str to buf, such that the first character of str is at {offs_x, offs_y} in buf space, abiding the length limit and flags provided, with all parts besides the character being the same as e_template
void eb_str_put(ebuffer_t *restrict buf, char *restrict str, int offs_x, int offs_y, size_t strlim, mde_flags_t flags, screen_element_t e_template){
    if(strlim == 0) strlim = buf->width - offs_x;
    size_t str_len = strlen(str);

    for(int x = 0; x < strlim; ++x){
        if(x + offs_x < 0) continue;
        screen_element_t e_write = e_template;
        if(x < str_len) e_write.c = str[x];
        else if(!(flags & MDE_SREPLACE)) e_write = SE_NULL;

        eb_set(buf, offs_x + x, offs_y, e_write);
    }
}
