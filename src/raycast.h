#ifndef RAYCAST_H__
#define RAYCAST_H__

#include <stddef.h>
#include "config.h"
#include "chunk.h"

// ray with input and output variables
typedef struct{
    vec2_t      position;
    transform_t rotation;

    dst_t       distance;
    block_t     block;
    vec2_t      hit_normal;
} ray_info_t;

// updates ray based on what block it hits in chunks
void cast_ray(ray_info_t *restrict ray, const chunk_info_t *restrict chunks, const size_t chunks_size);

#endif
