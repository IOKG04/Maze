#include "display.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <termios.h>

struct termios old_termios;

char *s_buffer; // extra character per line for \n
size_t s_width, s_height, s_size;

static inline size_t s_get_index(size_t x, size_t y);

// public functions
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
    s_size = (s_width + 1) * s_height;
    s_buffer = malloc(s_size);
    if(s_buffer == NULL){
	fprintf(stderr, "Failed to allocate s_buffer at %s, %d", __FILE__, __LINE__);
	tcsetattr(fileno(stdin), TCSANOW, &old_termios);
	exit(1);
    }
    memset(s_buffer, ' ', s_size);
    for(int i = 0; i < s_height; ++i){
	s_buffer[s_get_index(s_width, i)] = '\n';
    }
}
void revert_screen(){
    // reset termios
    tcsetattr(fileno(stdin), TCSANOW, &old_termios);

    // reset screen
    printf("\x1b[?25h");

    // free buffer
    free(s_buffer);
}

void set_pixel(size_t x, size_t y, char c){
    if(x > s_width || y >= s_height) return;
    s_buffer[s_get_index(x, y)] = c;
}

void draw_screen(){
    printf("\x1b[H");
    for(int i = 0; i < s_size; ++i){
	putchar(s_buffer[i]);
    }
}

// private functions
static inline size_t s_get_index(size_t x, size_t y){
    return y * (s_width + 1) + x;
}
