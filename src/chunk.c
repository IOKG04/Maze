#include "chunk.h"

#include <stdlib.h>
#include <string.h>
#include <stddef.h>
#include <stdint.h>
#include <math.h>
#include "config.h"

#define WALL_SIZE_MAX ((CHUNK_SIZE - 2) / 2)
#define WALL_CHANCE   1, 2

// seed used by chunk generation
unsigned int initial_seed;

// returns a random int between a and b
static inline int rand_lim(int min, int max);
// returns 1 with a chance of x/y
static inline uint_fast8_t x_in_y(double x, double y);

/******************\
| PUBLIC FUNCTIONS |
\******************/

// sets seed used by chunk generation
void set_initial_seed(const unsigned int seed){
    initial_seed = seed;
}
// generates a chunk at pos into chunk
void generate_chunk(chunk_info_t *chunk, const chunk_pos_t pos_x, const chunk_pos_t pos_y){
    chunk->pos_x = pos_x;
    chunk->pos_y = pos_y;
    memset(chunk->data, 0, CHUNK_SIZE * CHUNK_SIZE);

    // north wall
    if(x_in_y(WALL_CHANCE)){
	int w_size = rand_lim(1, WALL_SIZE_MAX);
	int ly = w_size;
	for(int x = 0; x < CHUNK_SIZE; ++x){
	    for(int y = 0; y < ly; ++y){
		chunk->data[y][x] = 1;
	    }
	}
    }
    // south wall
    if(x_in_y(WALL_CHANCE)){
	int w_size = rand_lim(1, WALL_SIZE_MAX);
	int ly = CHUNK_SIZE - w_size;
	for(int x = 0; x < CHUNK_SIZE; ++x){
	    for(int y = ly; y < CHUNK_SIZE; ++y){
		chunk->data[y][x] = 1;
	    }
	}
    }
    // west wall
    if(x_in_y(WALL_CHANCE)){
	int w_size = rand_lim(1, WALL_SIZE_MAX);
	int lx = CHUNK_SIZE - w_size;
	for(int x = lx; x < CHUNK_SIZE; ++x){
	    for(int y = 0; y < CHUNK_SIZE; ++y){
		chunk->data[y][x] = 1;
	    }
	}
    }
    // east wall
    if(x_in_y(WALL_CHANCE)){
	int w_size = rand_lim(1, WALL_SIZE_MAX);
	int lx = w_size;
	for(int x = 0; x < lx; ++x){
	    for(int y = 0; y < CHUNK_SIZE; ++y){
		chunk->data[y][x] = 1;
	    }
	}
    }
}


/********************\
| PPRIVATE FUNCTIONS |
\********************/

// returns a random int between a and b
static inline int rand_lim(int min, int max){
    return floor(((rand()) / (RAND_MAX - 1.0)) * (max - min + 1)) + min;
}
// returns 1 with a chance of x/y
static inline uint_fast8_t x_in_y(double x, double y){
    return ((rand() / (double)RAND_MAX) * y) <= x;
}
