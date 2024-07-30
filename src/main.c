#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include "config.h"
#include "display.h"
#include "raycast.h"

void set_px_chunk(const chunk_info_t chunk, size_t x_off, size_t y_off);

int main(int argc, char **argv){
    setup_screen(80, 25);

    set_initial_seed(4);
    chunk_info_t c = {};
    generate_chunk(&c, 0, 0);
    set_px_chunk(c, 0, 0);
    generate_chunk(&c, 1, 1);
    set_px_chunk(c, CHUNK_SIZE + 1, 0);
    generate_chunk(&c, 0, 0);
    set_px_chunk(c, 2 * CHUNK_SIZE + 2, 0);

    draw_screen();

    revert_screen();
    exit(0);
}

void set_px_chunk(const chunk_info_t chunk, size_t x_off, size_t y_off){
    for(size_t x = 0; x < CHUNK_SIZE; ++x){
	for(size_t y = 0; y < CHUNK_SIZE; ++y){
	    char c = chunk.data[x][y] == 0 ? ' ' : '#';
	    set_pixel(x + x_off, y + y_off, c);
	}
    }
}
