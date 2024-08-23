/************************************************\
|          Implementations for buffer.h          |
|                                                |
| Copyright (c) 2024, https://github.com/IOKG04  |
| Licensed under MIT-Festival-Light, available   |
| at https://github.com/IOKG04/Licenses/blob/master/MIT-FL/MIT-Festival-Light |
\************************************************/

#include "buffer.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stddef.h>
#include "display.h"
#include "config.h"

// returns 1D index for {x, y} in buf
#define eb_get_index(x, y, buf) b_get_index(x, y, buf.width)
// returns size of buf
#define b_size(buf) (buf.height * buf.width)

// returns 1D index for {x, y} in buffer with width width
static inline size_t b_get_index(size_t x, size_t y, size_t width);


/******************\
| PUBLIC FUNCTIONS |
\******************/

// initializes buf to size {width, height}
int eb_init(ebuffer_t *buf, size_t width, size_t height){
    #if MDE_BUFFER_ALLOC_CHECK
        if(buf->data != NULL) return 2;
    #endif
    buf->width = width;
    buf->height = height;
    buf->data = malloc(sizeof(screen_element_t) * b_size((*buf)));
    if(buf->data == NULL) return 1;
    return 0;
}
// deinitialized members of buf
void eb_free(ebuffer_t *buf){
    #if MDE_BUFFER_ALLOC_CHECK
        if(buf->data == NULL) return;
    #endif
    free(buf->data);
    buf->data = NULL;
    buf->width = 0;
    buf->height = 0;
}

// sets element in buf at {x, y} to e
int eb_set(ebuffer_t *buf, size_t x, size_t y, screen_element_t e){
    #if MDE_BUFFER_INDEX_CHECK == 2
        if(x >= buf->width || x < 0 || y >= buf->height || y < 0) return 1;
    #endif
    size_t i = eb_get_index(x, y, (*buf));
    #if MDE_BUFFER_INDEX_CHECK == 1
        if(i < 0 || i >= buf->width * buf->height) return 1;
    #endif
    buf->data[i] = e;
    return 0;
}
// sets all elements in buf to e
void eb_clear(ebuffer_t *buf, screen_element_t e){
    for(size_t i = 0; i < b_size((*buf)); ++i){
        buf->data[i] = e;
    }
}
// draws src onto dest, such that {0, 0} in src space is {offs_x, offs_y} is dest space
void eb_draw(ebuffer_t *dest, ebuffer_t src, int offs_x, int offs_y, mde_flags_t flags){
    for(int x = 0; x < src.width; ++x){
        if(offs_x + x < 0 || offs_x + x >= dest->width) continue;
        for(int y = 0; y < src.height; ++y){
            if(offs_y + y < 0 || offs_y + y >= dest->height) continue;
            if(flags & MDE_BDRAW_OVER) eb_set(dest, offs_x + x, offs_y + y, src.data[x + src.width * y]);
            else{
                if(!SE_IS_NULL(src.data[x + src.width * y])) eb_set(dest, offs_x + x, offs_y + y, src.data[x + src.width * y]);
            }
        }
    }
}

// returns element at {x, y} in buf
screen_element_t eb_get(ebuffer_t buf, size_t x, size_t y){
    #if MDE_BUFFER_INDEX_CHECK == 2
        if(x < 0 || x >= buf.width || y < 0 || y >= buf.height) return SE_NULL;
    #endif
    size_t i = eb_get_index(x, y, buf);
    #if MDE_BUFFER_INDEX_CHECK == 1
        if(i < 0 || i >= buf.width * buf.height) return SE_NULL;
    #endif
    return buf.data[i];
}

// prints buf to terminal, such that {0, 0} in buf space is at {offs_x, offs_y} in terminal space (zero based)
void eb_print(ebuffer_t buf, int offs_x, int offs_y){
    for(int y = 0; y < buf.height; ++y){
        if(offs_y + y < 0) continue;
        printf("\x1b[%i;%iH", offs_y + y + 1, (offs_x <= 0 ? 1 : offs_x + 1));
        for(int x = 0; x < buf.width; ++x){
            if(offs_x + x < 0) continue;
            #define b_dat (buf.data[x + buf.width * y])
            #if MDE_TRUE_COLOR
                if(get_flags_screen() & MDE_DTRUECOLOR) printf("\x1b[38;2;%i;%i;%im", b_dat.r, b_dat.g, b_dat.b);
            #endif
            #if MDE_PRINT_NULL_AS_SPACE
                if(SE_IS_NULL(b_dat)) putchar(SE_SPACE.c);
                else                  putchar(b_dat.c);
            #else
                putchar(b_dat.c);
            #endif
            #undef b_dat
        }
    }
}


/*******************\
| PRIVATE FUNCTIONS |
\*******************/

// returns 1D index for {x, y} in buffer with width width
static inline size_t b_get_index(size_t x, size_t y, size_t width){
    return y * width + x;
}
