#ifndef CHUNK_H__
#define CHUNK_H__

#include "config.h"

typedef struct{
    chunk_pos_t pos_x, pos_y; // this * CHUNK_SIZE = offset
    block_t data[CHUNK_SIZE][CHUNK_SIZE];
} chunk_info_t;

void set_initial_seed(const unsigned int seed);
void generate_chunk(chunk_info_t *chunk, const chunk_pos_t pos_x, const chunk_pos_t pos_y);

#endif
