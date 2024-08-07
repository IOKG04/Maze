#ifndef CONFIG_H__
#define CONFIG_H__

#include <stdint.h>

#define DEBUG_VIEW 1

#define CHUNK_SIZE 16

#define MAX_RAY_STEPS 256
#define RAY_LENGTH_MAX INFINITY

#define VERT_SCALE 3
#define SCREEN_H 25
#define SCREEN_W 80

typedef double  transform_t;
typedef float  dst_t;
typedef int_fast16_t chunk_pos_t;
typedef uint8_t block_t;

typedef struct{
    transform_t x, y;
} vec2_t;

#endif
