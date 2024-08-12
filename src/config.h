#ifndef CONFIG_H__
#define CONFIG_H__

#include <stdint.h>
#include <math.h>

// debug settings
#define DEBUG 0
#if DEBUG
    // 0: Shaded view
    // 1: Normal view
    // 2: Block  view
    #define DEBUG_VIEW 0
#else
    #define DEBUG_VIEW 0
#endif

// visualization configurations
#define ASCII_GRADIENT_MAP ".-:=+*%#@"

#define VERT_SCALE 3

#define SCREEN_H 25
#define SCREEN_W 80

// movement configurations
#define MOVEMENT_SPEED 0.1
#define ROTATION_SPEED 0.025
#define FOV_CHANGE     (M_PI / 18)

// technical configurations
#define CHUNK_SIZE 8
#define LOADED_CHUNKS_X 7
#define LOADED_CHUNKS_Y 7
#define LOADED_CHUNKS_SIZE (LOADED_CHUNKS_X * LOADED_CHUNKS_Y)

#define MAX_RAY_STEPS 256
#define RAY_LENGTH_MAX INFINITY

typedef double  transform_t;
typedef float  dst_t;
typedef int_fast16_t chunk_pos_t;
typedef uint8_t block_t;

typedef struct{
    transform_t x, y;
} vec2_t;

#endif
