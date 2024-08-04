#ifndef RAYCAST_H__
#define RAYCAST_H__

#include <stddef.h>
#include "config.h"

typedef struct{
    vec2_t      position;
    transform_t rotation;

    dst_t       distance;
    block_t     block;
    vec2_t      hit_normal;
} ray_info_t;

typedef struct{
    chunk_pos_t pos_x, pos_y; // this * CHUNK_SIZE = offset
    block_t data[CHUNK_SIZE][CHUNK_SIZE];
} chunk_info_t;

void cast_ray(ray_info_t *restrict ray, const chunk_info_t *restrict chunks, const size_t chunks_size);

void set_initial_seed(const unsigned int seed);
void generate_chunk(chunk_info_t *chunk, const chunk_pos_t pos_x, const chunk_pos_t pos_y);

#endif
