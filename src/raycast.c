#include "raycast.h"

#include <stdlib.h>
#include <stddef.h>
#include <string.h>
#include "config.h"

unsigned int initial_seed;

// public functions
void cast_ray(ray_info_t *restrict ray, const chunk_info_t *restrict chunks, const size_t chunks_size){
}

void set_initial_seed(const unsigned int seed){
    initial_seed = seed;
}
void generate_chunk(chunk_info_t *chunk, const chunk_pos_t pos_x, const chunk_pos_t pos_y){
    // TODO: Actually make good
    srand(initial_seed + pos_x * 3607 + pos_y * 6841);
    chunk->pos_x = pos_x;
    chunk->pos_y = pos_y;
    memset(chunk->data, 0, CHUNK_SIZE * CHUNK_SIZE);
    for(size_t x = 0; x < CHUNK_SIZE; ++x){
	for(size_t y = 0; y < CHUNK_SIZE; ++y){
	    if(rand() % 4 == 0) chunk->data[x][y] = 1;
	}
    }
}
