#ifndef CONFIG_H__
#define CONFIG_H__

#include <stdint.h>

#define DEBUG 0
#if DEBUG
    // 0: Shaded view
    // 1: Normal view
    // 2: Block  view
    #define DEBUG_VIEW 1
#else
    #define DEBUG_VIEW 0
#endif

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
