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
    chunk_info_t chunk = {0, 0,
	{
	    {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,},
	    {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,},
	    {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,},
	    {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,},
	    {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,},
	    {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,},
	    {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,},
	    {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,},
	    {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,},
	    {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,},
	    {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,},
	    {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,},
	    {1,0,0,0,0,0,0,0,0,0,2,3,4,5,0,1,},
	    {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,},
	    {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,},
	    {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,},
	}
    };

    int inp = 0;
#if DEBUG
    cast_x = 1;
    cast_y = 1;
#endif
    transform_t player_angle = M_PI * 0.25 - 0.005,
	        fov = M_PI * 0.5;
    do{
	clear_screen(' ');
	for(int x = 0; x < SCREEN_W; ++x){
	    ray_info_t ray = {{7.5, 7.5}, player_angle + fov / (SCREEN_W - 1) * (x - SCREEN_W / 2.0), 0, 0, {0, 0}};
#if DEBUG
	    print_debug_info = 0;
#endif
	    cast_ray(&ray, &chunk, 1);
	    ray.distance *= cos(ray.rotation - player_angle);
	    dst_t l_height = (SCREEN_H * VERT_SCALE) / ray.distance,
	          l_min = (SCREEN_H / 2.0) - (l_height / 2),
		  l_max = l_min + l_height;
	    for(int y = 0; y < SCREEN_H; ++y){
		char c = ' ';
		if(y >= l_min && y < l_max){
#if DEBUG_VIEW == 0
		    char *map = ".:-=+*#%@";
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
		set_pixel(x, y, c);
	    }
	}
	draw_screen();

	inp = getchar();
	if(inp == 'h') player_angle -= 0.01;
	else if(inp == 'l') player_angle += 0.01;
#if DEBUG
	else if(inp == 'x') cast_x = !cast_x;
	else if(inp == 'y') cast_y = !cast_y;
	else if(inp == 'c') printf("\x1b[0J");
#endif
    } while(inp != 'q');

    revert_screen();
    exit(0);
}
