#include "raycast.h"

#include <stdlib.h>
#include <stddef.h>
#include <string.h>
#include <math.h>
#include "config.h"
#include "chunk.h"

#if DEBUG
#include <stdio.h>
int cast_x, cast_y, print_debug_info;
#endif

// gets step sizes from angle
static inline void get_steps(const ray_info_t ray, transform_t *restrict x, transform_t *restrict y);
// gets block at specific positoin in chunk array
static inline block_t get_block(const chunk_info_t *restrict chunks, const size_t chunks_size, const chunk_pos_t pos_x, const chunk_pos_t pos_y);


/******************\
| PUBLIC FUNCTIONS |
\******************/

// updates ray based on what block it hits in chunks
void cast_ray(ray_info_t *restrict ray, const chunk_info_t *restrict chunks, const size_t chunks_size){
    ray->distance = RAY_LENGTH_MAX;

    transform_t dx, dy;
    get_steps(*ray, &dx, &dy);
    int_fast8_t dx_sign = signbit(dx) ? -1 : 1,
                dy_sign = signbit(dy) ? -1 : 1;
    
    // get first horizontal intersection
#if DEBUG
    if(dy != 0 && cast_y){
#else
    if(dy != 0){
#endif
	transform_t adj_dx = dx / fabs(dy),
		    offs_y = (dy_sign == 1 ? ceil(ray->position.y) : floor(ray->position.y)) - ray->position.y,
		    offs_x = fabs(offs_y) * adj_dx;
#if DEBUG
	if(print_debug_info) printf("hori adj_dx: %f, offs_x: %f\n", adj_dx, offs_x);
#endif
	for(int steps = 0; steps <= MAX_RAY_STEPS; ++steps){
	    chunk_pos_t block_y = (((chunk_pos_t)floor(ray->position.y)) + (dy_sign * steps)) + (dy_sign),
	                block_x = ((chunk_pos_t)floor((ray->position.x + offs_x) + (adj_dx * steps)));
	    block_t block = get_block(chunks, chunks_size, block_x, block_y);

#if DEBUG
	    if(print_debug_info) printf("hori test at x: %li, y: %li\n", block_x, block_y);
#endif

	    if(block == 0) continue;

	    transform_t diff_x = offs_x + (adj_dx * steps),
			diff_y = offs_y + (dy_sign * steps);
	    dst_t distance = sqrt(diff_x * diff_x + diff_y * diff_y);
#if DEBUG
	    if(print_debug_info) printf("hori hit %i at precise x: %f, y: %f, dist: %f\n", block, ray->position.x + diff_x, ray->position.y + diff_y, distance);
#endif
	    if(distance < ray->distance){
		ray->distance = distance;
		ray->block = block;
		ray->hit_normal = (vec2_t){0, -dy_sign};
	    }
	    break;
	}
    }
    // get first vertical intersection
#if DEBUG
    if(dx != 0 && cast_x){
#else
    if(dx != 0){
#endif
	transform_t adj_dy = dy / fabs(dx),
		    offs_x = (dx_sign == 1 ? ceil(ray->position.x) : floor(ray->position.x)) - ray->position.x,
		    offs_y = fabs(offs_x) * adj_dy;
#if DEBUG
	if(print_debug_info) printf("vert adj_dy: %f, offs_y: %f\n", adj_dy, offs_y);
#endif
	for(int steps = 0; steps <= MAX_RAY_STEPS; ++steps){
	    chunk_pos_t block_y = ((chunk_pos_t)floor((ray->position.y + offs_y) + (adj_dy * steps))),
	                block_x = (((chunk_pos_t)floor(ray->position.x)) + (dx_sign * steps)) + (dx_sign);
	    block_t block = get_block(chunks, chunks_size, block_x, block_y);

#if DEBUG
	    if(print_debug_info) printf("vert test at x: %li, y: %li\n", block_x, block_y);
#endif

	    if(block == 0) continue;

	    transform_t diff_x = offs_x + (dx_sign * steps),
			diff_y = offs_y + (adj_dy * steps);
	    dst_t distance = sqrt(diff_x * diff_x + diff_y * diff_y);
#if DEBUG
	    if(print_debug_info) printf("vert hit %i at precise x: %f, y: %f, dist: %f\n", block, ray->position.x + diff_x, ray->position.y + diff_y, distance);
#endif
	    if(distance < ray->distance){
		ray->distance = distance;
		ray->block = block;
		ray->hit_normal = (vec2_t){-dx_sign, 0};
	    }
	    break;
	}
    }
}


/*******************\
| PRIVATE FUNCTIONS |
\*******************/

// gets step sizes from angle
static inline void get_steps(const ray_info_t ray, transform_t *restrict x, transform_t *restrict y){
    *x = cos(ray.rotation);
    *y = sin(ray.rotation);
}
// gets block at specific positoin in chunk array
static inline block_t get_block(const chunk_info_t *restrict chunks, const size_t chunks_size, const chunk_pos_t pos_x, const chunk_pos_t pos_y){
    chunk_pos_t c_pos_x = floor(pos_x / (double)CHUNK_SIZE),
		c_pos_y = floor(pos_y / (double)CHUNK_SIZE);
    for(size_t i = 0; i < chunks_size; ++i){
	if(chunks[i].pos_x != c_pos_x || chunks[i].pos_y != c_pos_y) continue;
	size_t y = ((pos_y % CHUNK_SIZE) + CHUNK_SIZE) % CHUNK_SIZE,
	       x = ((pos_x % CHUNK_SIZE) + CHUNK_SIZE) % CHUNK_SIZE;
	return chunks[i].data[y][x];
    }
    return 0;
}
