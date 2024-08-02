// PES == Potential Error Source
// check these lines if any errors occur
#include <stdio.h>

#include "raycast.h"

#include <stdlib.h>
#include <stddef.h>
#include <string.h>
#include <math.h>
#include "config.h"

unsigned int initial_seed;

static inline void get_steps(const ray_info_t ray, transform_t *restrict x, transform_t *restrict y);
static inline block_t get_block(const chunk_info_t *restrict chunks, const size_t chunks_size, const chunk_pos_t pos_x, const chunk_pos_t pos_y);

// public functions
void cast_ray(ray_info_t *restrict ray, const chunk_info_t *restrict chunks, const size_t chunks_size){
    ray->distance = RAY_LENGTH_MAX;

    transform_t dx, dy;
    get_steps(*ray, &dx, &dy);
    transform_t dx_sign = signbit(dx) ? -1 : 1,
                dy_sign = signbit(dy) ? -1 : 1;
    
    // get first horizontal intersection
    if(dy != 0){
	transform_t adj_dx = dx / dy * dy_sign,
		    offs_y = (dy_sign == 1 ? ceil(ray->pos_y) : floor(ray->pos_y)) - ray->pos_y,
		    offs_x = offs_y * adj_dx; // PES
	for(int steps = 0; steps <= MAX_RAY_STEPS; ++steps){
	    chunk_pos_t block_y = ((ray->pos_y + offs_y) + (dy_sign * steps)) + (dy_sign == 1 ? 0 : -1),
			block_x = ((ray->pos_x + offs_x) + (adj_dx * steps)) + (dx_sign == 1 ? 0 : -1);
	    /*
	    printf("searching x: %d, y: %d\tfine x: %lf, y: %lf \n", block_x, block_y,
		    ((ray->pos_x + offs_x) + (adj_dx * steps)) + (dx_sign == 1 ? 0 : -1),
		    ((ray->pos_y + offs_y) + (dy_sign * steps)) + (dy_sign == 1 ? 0 : -1));
	    */
	    block_t block = get_block(chunks, chunks_size, block_x, block_y);

	    if(block == 0) continue;

	    transform_t diff_x = offs_x + (adj_dx * steps),
			diff_y = offs_y + (dy_sign * steps);
	    dst_t distance = sqrt(diff_x * diff_x + diff_y * diff_y);
	    // printf("horizontal: diff x: %f, y: %f, dist: %f\n", diff_x, diff_y, distance);
	    ray->distance = distance;
	    ray->block = block;
	    break;
	}
    }
    // get first vertical intersection
    if(dx != 0){
	transform_t adj_dy = dy / dx * dx_sign,
		    offs_x = (dx_sign == 1 ? ceil(ray->pos_x) : floor(ray->pos_x)) - ray->pos_x,
		    offs_y = offs_x * adj_dy; // PES
	for(int steps = 0; steps <= MAX_RAY_STEPS; ++steps){
	    chunk_pos_t block_y = ((ray->pos_y + offs_y) + (adj_dy * steps)) + (dy_sign == 1 ? 0 : -1),
			block_x = ((ray->pos_x + offs_x) + (dx_sign * steps)) + (dx_sign == 1 ? 0 : -1);
	    /*
	    printf("searching x: %d, y: %d\tfine x: %lf, y: %lf \n", block_x, block_y,
		    ((ray->pos_x + offs_x) + (dx_sign * steps)) + (dx_sign == 1 ? 0 : -1),
		    ((ray->pos_y + offs_y) + (adj_dy * steps)) + (dy_sign == 1 ? 0 : -1));
	    */
	    block_t block = get_block(chunks, chunks_size, block_x, block_y);

	    if(block == 0) continue;

	    transform_t diff_x = offs_x + (dx_sign * steps),
			diff_y = offs_y + (adj_dy * steps);
	    dst_t distance = sqrt(diff_x * diff_x + diff_y * diff_y);
	    // printf("vertical diff x: %f, y: %f, dist: %f\n", diff_x, diff_y, distance);
	    if(distance < ray->distance){
		ray->distance = distance;
		ray->block = block;
	    }
	    break;
	}
    }
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
    if(pos_x == 0 && pos_y == 0) return;
    for(size_t x = 0; x < CHUNK_SIZE; ++x){
	for(size_t y = 0; y < CHUNK_SIZE; ++y){
	    if(rand() % 4 == 0) chunk->data[x][y] = 1;
	}
    }
}

// private functions
static inline void get_steps(const ray_info_t ray, transform_t *restrict x, transform_t *restrict y){
    *x = cos(ray.rotation);
    *y = sin(ray.rotation);
}
static inline block_t get_block(const chunk_info_t *restrict chunks, const size_t chunks_size, const chunk_pos_t pos_x, const chunk_pos_t pos_y){
    chunk_pos_t c_pos_x = pos_x / CHUNK_SIZE,
		c_pos_y = pos_y / CHUNK_SIZE;
    for(size_t i = 0; i < chunks_size; ++i){
	if(chunks[i].pos_x != c_pos_x || chunks[i].pos_y != c_pos_y) continue;
	return chunks[i].data[pos_x % CHUNK_SIZE][pos_y % CHUNK_SIZE];
    }
    return 0;
}
