#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <string.h>
#include <math.h>
#include "chunk.h"
#include "config.h"
#include "raycast.h"
#include "madien/display.h"
#include "madien/buffer.h"

#if DEBUG
extern int cast_x, cast_y, print_debug_info;
#endif

ebuffer_t screen_buffer;

int main(int argc, char **argv){
    if(eb_init(&screen_buffer, SCREEN_W, SCREEN_H)){
        exit(1);
    }

    setup_screen(MDE_DGRAPHICS | MDE_DTRUECOLOR);

    set_initial_seed(2);
    //set_initial_seed(time(NULL));
    srand(time(NULL));

    chunk_info_t *chunks = malloc(sizeof(chunk_info_t) * LOADED_CHUNKS_SIZE);
#if LOG_CHUNKS
    FILE *f = fopen("chunk_data", "a");
#endif
    for(int x = 0; x < LOADED_CHUNKS_X; ++x){
        for(int y = 0; y < LOADED_CHUNKS_Y; ++y){
            generate_chunk(chunks + (x + LOADED_CHUNKS_X * y), x - (LOADED_CHUNKS_X / 2), y - (LOADED_CHUNKS_Y / 2));
#if LOG_CHUNKS
            fwrite(chunks[x + LOADED_CHUNKS_X * y].data, 1, CHUNK_SIZE * CHUNK_SIZE, f);
#endif
        }
    }
#if LOG_CHUNKS
    fclose(f);
#endif

    int inp = 0, draw_map = 0;
#if DEBUG
    cast_x = 1;
    cast_y = 1;
#endif
    transform_t player_angle = -M_PI / 2,
                fov = M_PI / 3;
    vec2_t      player_position = {nextafter(CHUNK_SIZE / 2.0, INFINITY), nextafter(CHUNK_SIZE / 2.0, INFINITY)};
    do{
        eb_clear(&screen_buffer, SE_SPACE);
        for(int x = 0; x < SCREEN_W; ++x){
            ray_info_t ray = {player_position, player_angle + fov / (SCREEN_W - 1) * (x - SCREEN_W / 2.0), 0, 0, {0, 0}};
#if DEBUG
            // print_debug_info = (x == SCREEN_W / 2);
            print_debug_info = 0;
#endif
            cast_ray(&ray, chunks, LOADED_CHUNKS_SIZE);
#if CORRECT_FISHEYE
            ray.distance *= cos(ray.rotation - player_angle);
#endif
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
                    if(!c) c = map[strlen(map) - 1];
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
                screen_element_t relem = SE_SPACE;
                relem.c = c;
                eb_set(&screen_buffer, x, y, relem);
            }
        }
        eb_print(screen_buffer, 0, 0);

        if(draw_map){
            for(int x = 0; x < LOADED_CHUNKS_X; ++x){
                for(int y = 0; y < LOADED_CHUNKS_Y; ++y){
                    for(int y2 = 0; y2 < CHUNK_SIZE; ++y2){
                        int off_y = SCREEN_H + CHUNK_SIZE * (chunks[x + LOADED_CHUNKS_X * y].pos_y + (LOADED_CHUNKS_Y / 2)) + y2 + 1;
                        int off_x = CHUNK_SIZE * (chunks[x + LOADED_CHUNKS_X * y].pos_x + (LOADED_CHUNKS_X / 2)) + 1;
                        printf("\x1b[%i;%iH", off_y, off_x);
                        for(int x2 = 0; x2 < CHUNK_SIZE; ++x2){
                            putchar(chunks[x + LOADED_CHUNKS_X * y].data[y2][x2] == 0 ? ' ' : '#');
                        }
                    }
                }
            }
            printf("\x1b[%i;%iHP", (int)(player_position.y + (int)(LOADED_CHUNKS_Y / 2) * CHUNK_SIZE + SCREEN_H + 1), (int)(player_position.x + (int)(LOADED_CHUNKS_X / 2) * CHUNK_SIZE) + 1);
        }

        inp = getchar();
        switch(inp){
            case 'h':
                player_angle -= ROTATION_SPEED;
                break;
            case 'l':
                player_angle += ROTATION_SPEED;
                break;
            case '7':
                player_angle -= M_PI * 0.5;
                break;
            case '9':
                player_angle += M_PI * 0.5;
                break;
            case '8':
                player_angle = -M_PI * 0.5;
                break;
            case '6':
                player_angle = 0;
                break;
            case '2':
                player_angle = M_PI * 0.5;
                break;
            case '4':
                player_angle = M_PI;
                break;
            case '+':
                fov += FOV_CHANGE;
                break;
            case '-':
                fov -= FOV_CHANGE;
                break;
            case 'w':
                player_position.x += cos(player_angle) * MOVEMENT_SPEED;
                player_position.y += sin(player_angle) * MOVEMENT_SPEED;
                break;
            case 'a':
                player_position.x += sin(player_angle) * MOVEMENT_SPEED;
                player_position.y -= cos(player_angle) * MOVEMENT_SPEED;
                break;
            case 's':
                player_position.x -= cos(player_angle) * MOVEMENT_SPEED;
                player_position.y -= sin(player_angle) * MOVEMENT_SPEED;
                break;
            case 'd':
                player_position.x -= sin(player_angle) * MOVEMENT_SPEED;
                player_position.y += cos(player_angle) * MOVEMENT_SPEED;
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
                break;
#endif
        }

#if PLAYER_POSITION_SAVE
        if((chunk_pos_t)player_position.x == player_position.x) player_position.x = nextafter(player_position.x, INFINITY);
        if((chunk_pos_t)player_position.y == player_position.y) player_position.y = nextafter(player_position.y, INFINITY);
#endif

        // load new chunks / discard old chunks
        for(int i = 0; i < LOADED_CHUNKS_SIZE; ++i){
            chunk_pos_t p_pos_x = floor(player_position.x / CHUNK_SIZE),
                        p_pos_y = floor(player_position.y / CHUNK_SIZE),
                        c_pos_x = chunks[i].pos_x,
                        c_pos_y = chunks[i].pos_y;
            // left boundary
            if(c_pos_x < p_pos_x - LOADED_CHUNKS_X / 2){
                generate_chunk(chunks + i, c_pos_x + LOADED_CHUNKS_X, c_pos_y);
            }
            // right boundary
            else if(c_pos_x > p_pos_x + LOADED_CHUNKS_X / 2){
                generate_chunk(chunks + i, c_pos_x - LOADED_CHUNKS_X, c_pos_y);
            }
            // up boundary
            else if(c_pos_y > p_pos_y + LOADED_CHUNKS_Y / 2){
                generate_chunk(chunks + i, c_pos_x, c_pos_y - LOADED_CHUNKS_Y);
            }
            // down boundary
            else if(c_pos_y < p_pos_y - LOADED_CHUNKS_Y / 2){
                generate_chunk(chunks + i, c_pos_x, c_pos_y + LOADED_CHUNKS_Y);
            }
        }
    } while(inp != 'q');

    free(chunks);
    revert_screen();
    exit(0);
}
