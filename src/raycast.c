#include "raycast.h"

#include <stdlib.h>
#include <stddef.h>
#include <string.h>
#include <math.h>
#include "config.h"

int cast_x, cast_y;

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
    if(dy != 0 && cast_y){
	transform_t adj_dx = dx / dy * dy_sign,
		    offs_y = (dy_sign == 1 ? ceil(ray->position.y) : floor(ray->position.y)) - ray->position.y,
		    offs_x = offs_y * adj_dx;
	for(int steps = 0; steps <= MAX_RAY_STEPS; ++steps){
	    chunk_pos_t block_y = ((ray->position.y + offs_y) + (dy_sign * steps)) + (dy_sign == 1 ? 0 : -1),
			block_x = ((ray->position.x + offs_x) + (chunk_pos_t)(adj_dx * steps)) + (dx_sign == 1 ? 0 : -1);
	    block_t block = get_block(chunks, chunks_size, block_x, block_y);

	    if(block == 0) continue;

	    transform_t diff_x = offs_x + (adj_dx * steps),
			diff_y = offs_y + (dy_sign * steps);
	    dst_t distance = sqrt(diff_x * diff_x + diff_y * diff_y);
	    if(distance < ray->distance){
		ray->distance = distance;
		ray->block = block;
		ray->hit_normal = (vec2_t){0, -dy_sign};
	    }
	    break;
	}
    }
    // get first vertical intersection
    if(dx != 0 && cast_x){
	transform_t adj_dy = dy / dx * dx_sign,
		    offs_x = (dx_sign == 1 ? ceil(ray->position.x) : floor(ray->position.x)) - ray->position.x,
		    offs_y = offs_x * adj_dy;
	for(int steps = 0; steps <= MAX_RAY_STEPS; ++steps){
	    chunk_pos_t block_y = ((ray->position.y + offs_y) + (chunk_pos_t)(adj_dy * steps)) + (dy_sign == 1 ? 0 : -1),
			block_x = ((ray->position.x + offs_x) + (dx_sign * steps)) + (dx_sign == 1 ? 0 : -1);
	    block_t block = get_block(chunks, chunks_size, block_x, block_y);

	    if(block == 0) continue;

	    transform_t diff_x = offs_x + (dx_sign * steps),
			diff_y = offs_y + (adj_dy * steps);
	    dst_t distance = sqrt(diff_x * diff_x + diff_y * diff_y);
	    if(distance < ray->distance){
		ray->distance = distance;
		ray->block = block;
		ray->hit_normal = (vec2_t){-dx_sign, 0};
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
