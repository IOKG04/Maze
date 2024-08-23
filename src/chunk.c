#include "chunk.h"

#include <stdlib.h>
#include <string.h>
#include <stddef.h>
#include <stdint.h>
#include <math.h>
#include "config.h"

#define HALLWAY_WIDTH 2

// enum for directions
enum direction{
    d_up    = 0b00,
    d_left  = 0b11,
    d_down  = 0b01,
    d_right = 0b10,
};

// seed used by chunk generation
unsigned int initial_seed;

// returns a random int between a and b
static inline int rand_lim(int min, int max);
// returns 1 with a chance of x/y
static inline uint_fast8_t x_in_y(double x, double y);
// returns whether a chunk at pos has a hallway in dir
uint_fast8_t has_hallway(chunk_pos_t pos_x, chunk_pos_t pos_y, enum direction dir);
// returns exit direction of chunk at pos
static inline enum direction exit_direction(chunk_pos_t pos_x, chunk_pos_t pos_y);

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
    memset(chunk->data, 0x1111, CHUNK_SIZE * CHUNK_SIZE * sizeof(block_t));
    srand(initial_seed + pos_x * 7 + pos_y * 13);

    char hallways[5] = {};
    // down hallway
    if(has_hallway(pos_x, pos_y, d_down)){
        hallways[d_down] = 1;
        ++hallways[4];
        for(int y = 0; y < CHUNK_SIZE / 2 + HALLWAY_WIDTH / 2; ++y){
            for(int x = CHUNK_SIZE / 2 - HALLWAY_WIDTH / 2; x < CHUNK_SIZE / 2 + HALLWAY_WIDTH / 2; ++x){
                chunk->data[y][x] = 0;
            }
        }
    }
    // up hallway
    if(has_hallway(pos_x, pos_y, d_up)){
        hallways[d_up] = 1;
        ++hallways[4];
        for(int y = CHUNK_SIZE / 2 - HALLWAY_WIDTH / 2; y < CHUNK_SIZE; ++y){
            for(int x = CHUNK_SIZE / 2 - HALLWAY_WIDTH / 2; x < CHUNK_SIZE / 2 + HALLWAY_WIDTH / 2; ++x){
                chunk->data[y][x] = 0;
            }
        }
    }
    // left hallway
    if(has_hallway(pos_x, pos_y, d_left)){
        hallways[d_left] = 1;
        ++hallways[4];
        for(int x = 0; x < CHUNK_SIZE / 2 + HALLWAY_WIDTH / 2; ++x){
            for(int y = CHUNK_SIZE / 2 - HALLWAY_WIDTH / 2; y < CHUNK_SIZE / 2 + HALLWAY_WIDTH / 2; ++y){
                chunk->data[y][x] = 0;
            }
        }
    }
    // right hallway
    if(has_hallway(pos_x, pos_y, d_right)){
        hallways[d_right] = 1;
        ++hallways[4];
        for(int x = CHUNK_SIZE / 2 - HALLWAY_WIDTH / 2; x < CHUNK_SIZE; ++x){
            for(int y = CHUNK_SIZE / 2 - HALLWAY_WIDTH / 2; y < CHUNK_SIZE / 2 + HALLWAY_WIDTH / 2; ++y){
                chunk->data[y][x] = 0;
            }
        }
    }
    // second connection
    if(hallways[4] < 2){
        enum direction d;
        chunk_pos_t offs_x = 4, offs_y = 4;
        while(hallways[(d = exit_direction(pos_x + offs_x, pos_y + offs_y))]){
            offs_x += initial_seed;
            offs_y -= 1;
        }
        switch(d){
            case d_up:
                for(int y = CHUNK_SIZE / 2 - HALLWAY_WIDTH / 2; y < CHUNK_SIZE; ++y){
                    for(int x = CHUNK_SIZE / 2 - HALLWAY_WIDTH / 2; x < CHUNK_SIZE / 2 + HALLWAY_WIDTH / 2; ++x){
                        chunk->data[y][x] = 0;
                    }
                }
                break;
            case d_down:
                for(int y = 0; y < CHUNK_SIZE / 2 + HALLWAY_WIDTH / 2; ++y){
                    for(int x = CHUNK_SIZE / 2 - HALLWAY_WIDTH / 2; x < CHUNK_SIZE / 2 + HALLWAY_WIDTH / 2; ++x){
                        chunk->data[y][x] = 0;
                    }
                }
                break;
            case d_left:
                for(int x = 0; x < CHUNK_SIZE / 2 + HALLWAY_WIDTH / 2; ++x){
                    for(int y = CHUNK_SIZE / 2 - HALLWAY_WIDTH / 2; y < CHUNK_SIZE / 2 + HALLWAY_WIDTH / 2; ++y){
                        chunk->data[y][x] = 0;
                    }
                }
                break;
            case d_right:
                for(int x = CHUNK_SIZE / 2 - HALLWAY_WIDTH / 2; x < CHUNK_SIZE; ++x){
                    for(int y = CHUNK_SIZE / 2 - HALLWAY_WIDTH / 2; y < CHUNK_SIZE / 2 + HALLWAY_WIDTH / 2; ++y){
                        chunk->data[y][x] = 0;
                    }
                }
                break;
        }
    }
}


/********************\
| PPRIVATE FUNCTIONS |
\********************/

// returns a random int between a and b
static inline int rand_lim(int min, int max){
    return floor(((rand()) / (RAND_MAX - 1.0)) * (max - min)) + min;
}
// returns 1 with a chance of x/y
static inline uint_fast8_t x_in_y(double x, double y){
    return ((rand() / (double)RAND_MAX) * y) <= x;
}
// returns whether a chunk at pos has a hallway in dir
uint_fast8_t has_hallway(chunk_pos_t pos_x, chunk_pos_t pos_y, enum direction dir){
    switch(dir){
        case d_down:
            if(exit_direction(pos_x, pos_y - 1) == d_up) return 1;
            break;
        case d_up:
            if(exit_direction(pos_x, pos_y + 1) == d_down) return 1;
            break;
        case d_right:
            if(exit_direction(pos_x + 1, pos_y) == d_left) return 1;
            break;
        case d_left:
            if(exit_direction(pos_x - 1, pos_y) == d_right) return 1;
            break;
    }
    return dir == exit_direction(pos_x, pos_y);
}
// returns exit direction of chunk at pos
static inline enum direction exit_direction(chunk_pos_t pos_x, chunk_pos_t pos_y){
    srand(initial_seed + 17 * pos_x + 31 * pos_y);
    return rand_lim(0, 4);
}
