#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <string.h>
#include <math.h>
#include "config.h"
#include "display.h"
#include "raycast.h"

#if DEBUG
extern int cast_x, cast_y, print_debug_info;
#endif

int main(int argc, char **argv){
    setup_screen(SCREEN_W, SCREEN_H);

    set_initial_seed(time(NULL));

    chunk_info_t *chunks = malloc(sizeof(chunk_info_t) * LOADED_CHUNKS_SIZE);
    for(int x = 0; x < LOADED_CHUNKS_X; ++x){
	for(int y = 0; y < LOADED_CHUNKS_Y; ++y){
	    generate_chunk(chunks + (x + LOADED_CHUNKS_X * y), x - 2, y - 2);
	}
    }

    int inp = 0, draw_map = 0;
#if DEBUG
    cast_x = 1;
    cast_y = 1;
#endif
    transform_t player_angle = M_PI * 0.25,
                fov = M_PI * 0.5;
    vec2_t      player_position = {7.5, 7.5};
    do{
	clear_screen(' ');
	for(int x = 0; x < SCREEN_W; ++x){
	    ray_info_t ray = {player_position, player_angle + fov / (SCREEN_W - 1) * (x - SCREEN_W / 2.0), 0, 0, {0, 0}};
#if DEBUG
	    print_debug_info = (x == SCREEN_W / 2);
#endif
	    cast_ray(&ray, chunks, LOADED_CHUNKS_SIZE);
	    ray.distance *= cos(ray.rotation - player_angle);
	    dst_t l_height = (SCREEN_H * VERT_SCALE) / ray.distance,
	          l_min = (SCREEN_H / 2.0) - (l_height / 2),
		  l_max = l_min + l_height;
	    for(int y = 0; y < SCREEN_H; ++y){
		char c = ' ';
		if(y >= l_min && y < l_max){
#if DEBUG_VIEW == 0
		    char *map = ASCII_GRADIENT_MAP;
		    transform_t surface_angle = 0;
		    if(ray.hit_normal.x == 1) surface_angle = M_PI;
		    else if(ray.hit_normal.x == -1) surface_angle = 0;
		    else if(ray.hit_normal.y == 1) surface_angle = M_PI * 1.5;
		    else if(ray.hit_normal.y == -1) surface_angle = M_PI * 0.5;
		    c = map[(int)(cos(ray.rotation - surface_angle) * strlen(map))];
#elif DEBUG_VIEW == 1
		    if(ray.hit_normal.x == 1) c = 'X';
		    else if(ray.hit_normal.x == -1) c = 'x';
		    else if(ray.hit_normal.y == 1) c = 'Y';
		    else if(ray.hit_normal.y == -1) c = 'y';
#elif DEBUG_VIEW == 2
		    c = '0' + ray.block;
#endif
		}
#if DEBUG
		if(print_debug_info && (y == 0 || y == SCREEN_H - 1)) c = '#';
#endif
		set_pixel(x, y, c);
	    }
	}
	draw_screen();

	if(draw_map){
	    for(int x = 0; x < LOADED_CHUNKS_X; ++x){
		for(int y = 0; y < LOADED_CHUNKS_Y; ++y){
		    for(int y2 = 0; y2 < CHUNK_SIZE; ++y2){
			int off_y = SCREEN_H + CHUNK_SIZE * y + y2;
			int off_x = CHUNK_SIZE * x;
			printf("\x1b[%i;%iH", off_y, off_x);
			for(int x2 = 0; x2 < CHUNK_SIZE; ++x2){
			    putchar(chunks[x + LOADED_CHUNKS_X * y].data[y2][x2] == 0 ? ' ' : '#');
			}
		    }
		}
	    }
	    printf("\x1b[%i;%iHP", (int)(player_position.y + 2 * CHUNK_SIZE + SCREEN_H), (int)(player_position.x + 2 * CHUNK_SIZE));
	}

	inp = getchar();
	switch(inp){
	    case 'h':
		player_angle -= 0.01;
		break;
	    case 'l':
		player_angle += 0.01;
		break;
	    case 'w':
		player_position.x += cos(player_angle) * 0.1;
		player_position.y += sin(player_angle) * 0.1;
		break;
	    case 'a':
		player_position.x += sin(player_angle) * 0.1;
		player_position.y -= cos(player_angle) * 0.1;
		break;
	    case 's':
		player_position.x -= cos(player_angle) * 0.1;
		player_position.y -= sin(player_angle) * 0.1;
		break;
	    case 'd':
		player_position.x -= sin(player_angle) * 0.1;
		player_position.y += cos(player_angle) * 0.1;
		break;
	    case 'm':
		draw_map = !draw_map;
		break;
#if DEBUG
	    case 'x':
		cast_x = !cast_x;
		break;
	    case 'y':
		cast_y = !cast_y;
		break;
	    case 'c':
		printf("\x1b[0J");
#endif
	}
    } while(inp != 'q');

    free(chunks);
    revert_screen();
    exit(0);
}
