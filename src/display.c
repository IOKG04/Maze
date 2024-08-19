#include "display.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <termios.h>
#include "config.h"

// data for old termios
struct termios old_termios;

// data for buffer
screen_element_t *s_buffer;
size_t s_width, s_height;

// gets the index if a pixel in buffer space
static inline size_t s_get_index(size_t x, size_t y);


/******************\
| PUBLIC FUNCTIONS |
\******************/

// initializes buffer and terminal
void setup_screen(size_t width, size_t height){
    // set termios
    tcgetattr(fileno(stdin), &old_termios);
    struct termios new_termios = old_termios;
    new_termios.c_lflag &= ~(ICANON | ECHO);
    tcsetattr(fileno(stdin), TCSANOW, &new_termios);

    // clear screen
    printf("\x1b[?25l\x1b[2J");

    // setup buffer
    s_width = width;
    s_height = height;
    s_buffer = malloc(s_width * s_height * sizeof(screen_element_t));
    if(s_buffer == NULL){
        fprintf(stderr, "Failed to allocate s_buffer at %s, %d", __FILE__, __LINE__);
        tcsetattr(fileno(stdin), TCSANOW, &old_termios);
        exit(1);
    }
    clear_screen(SE_SPACE);
}
// undos setup_screen()
void revert_screen(){
    // reset termios
    tcsetattr(fileno(stdin), TCSANOW, &old_termios);

    // reset screen
    printf("\x1b[?25h");

    // free buffer
    free(s_buffer);
}

// sets element at a position to e
void set_element(size_t x, size_t y, screen_element_t e){
    if(x > s_width || y >= s_height) return;
    s_buffer[s_get_index(x, y)] = e;
}
// sets buffer to e
void clear_screen(screen_element_t e){
    for(int y = 0; y < s_height; ++y){
        for(int x = 0; x < s_width; ++x){
            s_buffer[s_get_index(x, y)] = e;
        }
    }
}

// prints buffer content to stdout
void draw_screen(){
    printf("\x1b[H\x1b[1m");
    for(int y = 0; y < s_height; ++y){
        for(int x = 0; x < s_width; ++x){
            putchar(s_buffer[s_get_index(x, y)].c);
        }
        putchar('\n');
    }
    printf("\x1b[0m");
}


/*******************\
| PRIVATE FUNCTIONS |
\*******************/

// gets the index if a pixel in buffer space
static inline size_t s_get_index(size_t x, size_t y){
    return y * s_width + x;
}
