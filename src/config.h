#ifndef CONFIG_H__
#define CONFIG_H__

#include <stdint.h>
#include <math.h>


/*********************\
| DEBUG CONFIGURATION |
\*********************/

// whether to enable debug functionality
#define DEBUG 0
#if DEBUG
    // 0: Shaded view
    // 1: Normal view
    // 2: Block  view
    #define DEBUG_VIEW 0
    #define LOG_CHUNKS 0
#else
    #define DEBUG_VIEW 0
    #define LOG_CHUNKS 0
#endif


/************************\
| MOVEMENT CONFIGURATION |
\************************/

// speed of player for different actions
#define MOVEMENT_SPEED 0.1
#define ROTATION_SPEED 0.025
#define FOV_CHANGE     (M_PI / 18)

// whether to make sure the player isnt at an integer position
#define PLAYER_POSITION_SAVE 1


/*****************************\
| VISUALIZATION CONFIGURATION |
\*****************************/

// ascii gradient from dark gray to white
#define ASCII_GRADIENT_MAP ".-:=+*%#@"

// wall height multiplier
#define VERT_SCALE 2

// whether to correct fisheye effect
#define CORRECT_FISHEYE 1

// size of screen
#define SCREEN_H 25
#define SCREEN_W 80


/*********************\
| CHUNK CONFIGURATION |
\*********************/

// side length of a chunk
#define CHUNK_SIZE 8

// amount of chunks to load
#define LOADED_CHUNKS_X 5
#define LOADED_CHUNKS_Y 5
#define LOADED_CHUNKS_SIZE (LOADED_CHUNKS_X * LOADED_CHUNKS_Y)


/**************************\
| RAYCASTING CONFIGURATION |
\**************************/

// maximum steps and ray length for calculating ray-block intersections
#define MAX_RAY_STEPS 256
#define RAY_LENGTH_MAX INFINITY


/********************\
| TYPE CONFIGURATION |
\********************/

// common types (here so one can adjust accuracy)
typedef double  transform_t;
typedef float  dst_t;
typedef int_fast16_t chunk_pos_t;
typedef uint8_t block_t;

// 2D vector with transform_t accuracy
typedef struct{
    transform_t x, y;
} vec2_t;


/****************\
| UTILITY MACROS |
\****************/

// a modulus operator that repeats the 0..(n-1) pattern even into the negatives
#define REP_MOD(a, b) (((a % b) + b) % b)

#endif
