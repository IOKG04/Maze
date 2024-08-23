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
    #define VIEW_MODE 0
    #define LOG_CHUNKS 0
#else
    // 0: Shaded view
    // 1: Normal view
    // 2: Block  view
    #define VIEW_MODE 0
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
typedef uint16_t block_t;

/************************************************\
| block_t is a 16 bit number containing texture  |
| information for each side of a block or the    |
| block as a whole.                              |
|                                                |
| As for whole block codes, there are currently  |
| the following:                                 |
|  0: Empty, rays pass through this              |
|                                                |
| As for side block codes, every side is given   |
| 4 bits. The assignment is as follows:          |
| 0bSSSSNNNNEEEEWWWW                             |
| with S being south (y-), N being north, E      |
| being east (x+) and W being west.              |
| There are currently the following codes:       |
| 0x1: Debug color based on surface direction    |
\************************************************/

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
